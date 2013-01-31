#include "Globals.h"
#include "Graph/HandyGraphFunctions.h"
#include "Time/PrecisionTimer.h"
BattleMap::BattleMap(BattleBoy* owner) : 
				mOwner(owner),
                m_dCellWidth(0),
                m_dCellHeight(0),
                m_iCellsX(0),
                m_iCellsY(0),
                m_icxClient(0),
                m_icyClient(0),                
				m_iSourceCell(0),
                m_iTargetCell(0),
                m_dCostToTarget(0.0),
                m_pGraph(NULL)
{

}

void BattleMap::init()
{
	// Load the test map
	mMapData = BoyLib::tgaLoad("res/Map_1Lane.tga");
	createGraph( mMapData->width, mMapData->height);

	//save the terrain
	for(int i=0 ; i<mMapData->width; i++ )
	{
		for(int j=0 ; j<mMapData->height ; j++ )
		{
			int t = i + mMapData->width * j;
			BoyLib::tColorInfo* tData = (BoyLib::tColorInfo*)(mMapData->imageData);
			BoyLib::tColorInfo tValue = tData[t];		
			
			if( tValue.r == TT_PATH )
			{
				m_TerrainType[t] = tValue.g;

				updateGraphFromBrush(tValue.g, t);
			}
			else if( tValue.r == TT_SPAWN )
			{
				switch( tValue.g )
				{
				case ST_PLAYER1:
					m_iSourceCell = t;
					break;
				case ST_PLAYER2:
					m_iTargetCell = t;
					break;
				}
			}
		}
	}

	m_CurrentAlgorithm = AT_ASTAR;
	updateAlgorithm();
}

void BattleMap::draw( Boy::Graphics* g )
{
	//return;

	//render all the cells
	for (int nd=0; nd<m_pGraph->NumNodes(); ++nd)
	{
		int left   = (int)(m_pGraph->GetNode(nd).Pos().x - m_dCellWidth/2.0);
		int top    = (int)(m_pGraph->GetNode(nd).Pos().y - m_dCellHeight/2.0);
		int right  = (int)(1+m_pGraph->GetNode(nd).Pos().x + m_dCellWidth/2.0);
		int bottom = (int)(1+m_pGraph->GetNode(nd).Pos().y + m_dCellHeight/2.0);

		g->setColorizationEnabled(true);
		switch (m_TerrainType[nd])
		{
		case BT_NORMAL:
			g->setColor(0xff00ffff);
			break;
		case BT_OBSTACLE:
			g->setColor(0xffff0000);
			break;
		default:
			g->setColor(0xff000fff);
			break;

		}//end switch

		g->fillRect((float)left, (float)top, (float)m_dCellWidth, (float)m_dCellHeight); 
	}
	//draw the path (if any)  
	if (m_Path.size() > 0)
	{
		g->setColor(0xff000f0f);

		std::list<int>::iterator it = m_Path.begin();
		std::list<int>::iterator nxt = it; ++nxt;

		for (it; nxt != m_Path.end(); ++it, ++nxt)
		{
		  g->drawLine((float)m_pGraph->GetNode(*it).Pos().x,(float)m_pGraph->GetNode(*it).Pos().y , (float)m_pGraph->GetNode(*nxt).Pos().x, (float) m_pGraph->GetNode(*nxt).Pos().y);
		}
	}
	g->setColorizationEnabled(false);

}

void BattleMap::createGraph(int numCellsX, int numCellsY)
{
	m_icxClient = Boy::Environment::screenWidth();
	m_icyClient = Boy::Environment::screenHeight();

	//initialize the terrain vector with normal terrain
	m_TerrainType.assign(numCellsY * numCellsX, BT_NORMAL);

	m_iCellsX     = numCellsX;
	m_iCellsY     = numCellsY;
	m_dCellWidth  = (double)m_icxClient / (double)numCellsX;
	m_dCellHeight = (double)m_icyClient / (double)numCellsY;

	//delete any old graph
	delete m_pGraph;

	//create the graph
	m_pGraph = new NavGraph(false);//not a digraph	

	debug_con<<"Creating navigation grid..."<<"";
	GraphHelper_CreateGrid(*m_pGraph, m_icxClient, m_icyClient, numCellsY, numCellsX);
	debug_con<<"Finished creating navigation grid."<<"";

	//initialize source and target indexes to mid top and bottom of grid 
	vecToIndex(BoyLib::Vector2(m_icxClient/2.0f, (float)m_dCellHeight*2.0f), m_iTargetCell);
	vecToIndex(BoyLib::Vector2(m_icxClient/2.0f, m_icyClient - (float)m_dCellHeight*2.0f), m_iSourceCell);

	m_Path.clear();
	m_SubTree.clear();

	m_CurrentAlgorithm = AT_NONE;
	m_dTimeTaken = 0;
}

//--------------------------- UpdateGraphFromBrush ----------------------------
//
//  given a brush and a node index, this method updates the graph appropriately
//  (by removing/adding nodes or changing the costs of the node's edges)
//-----------------------------------------------------------------------------
void BattleMap::updateGraphFromBrush(int brush, int cellIndex)
{
  //set the terrain type in the terrain index
  m_TerrainType[cellIndex] = brush;

  //if current brush is an obstacle then this node must be removed
  //from the graph
 /* if (brush == BT_OBSTACLE)
  {
    m_pGraph->RemoveNode(cellIndex);
  }
  else*/
  {
    //make the node active again if it is currently inactive
    if (!m_pGraph->isNodePresent(cellIndex))
    {
      int y = cellIndex / m_iCellsY;
      int x = cellIndex - (y*m_iCellsY);

      m_pGraph->AddNode(NavGraph::NodeType(cellIndex, Vector2D(x*m_dCellWidth + m_dCellWidth/2.0,
                                                               y*m_dCellHeight+m_dCellHeight/2.0)));

      GraphHelper_AddAllNeighboursToGridNode(*m_pGraph, y, x, m_iCellsX, m_iCellsY);
    }

    //set the edge costs in the graph
    WeightNavGraphNodeEdges(*m_pGraph, cellIndex, getTerrainCost((EPathType)brush));                            
  }
}

//--------------------- PointToIndex -------------------------------------
//
//  converts a POINTS into an index into the graph
//------------------------------------------------------------------------
bool BattleMap::vecToIndex(const BoyLib::Vector2& v, int& nodeIndex)
{
  //convert p to an index into the graph
  int x = (int)((double)(v.x)/m_dCellWidth);  
  int y = (int)((double)(v.y)/m_dCellHeight); 
  
  //make sure the values are legal
  if ( (x>m_iCellsX) || (y>m_iCellsY) )
  {
    nodeIndex = -1;

    return false;
  }

  nodeIndex = y*m_iCellsX+x;

  return true;
}

bool BattleMap::indexToVec(BoyLib::Vector2& v, int& nodeIndex)
{
	if( nodeIndex > m_iCellsX * m_iCellsY )
	{
		return false;
	}

	float x = (float)(nodeIndex % m_iCellsX);
	float y = (float)(nodeIndex / m_iCellsX);
	
	v.x = (float)(x * m_dCellWidth + m_dCellWidth/2.0f);
	v.y = (float)(y * m_dCellHeight + m_dCellHeight/2.0f);
	return true;
}
//----------------- GetTerrainCost ---------------------------------------
//
//  returns the cost of the terrain represented by the current brush type
//------------------------------------------------------------------------
double BattleMap::getTerrainCost(const EPathType brush)
{
  const double cost_normal = 1.0;
  const double cost_water  = 2.0;
  const double cost_mud    = 1.5;

  switch (brush)
  {
    case BT_NORMAL: return cost_normal;
    case BT_WATER:  return cost_water;
    case BT_MUD:    return cost_mud;
    default:     return MaxDouble;
  };
}

//--------------------------- UpdateAlgorithm ---------------------------------
void BattleMap::updateAlgorithm()
{
	//update any current algorithm
	switch(m_CurrentAlgorithm)
	{
	case AT_NONE:
		break;
	case AT_DFS:
		createPathDFS(); 
		break;
	case AT_BFS:
		createPathBFS(); 
		break;
	case AT_DIJKSTRA:
		createPathDijkstra(); 
		break;
	case AT_ASTAR:
		createPathAStar(); 
		break;
	default: 
		break;
	}
}

std::list<BoyLib::Vector2> BattleMap::getPath(const BoyLib::Vector2& from, const BoyLib::Vector2& to)
{
	//reset path and tree records
	m_SubTree.clear();
	m_Path.clear();

	//initialize source and target indexes to mid top and bottom of grid 
	vecToIndex(from, m_iSourceCell);
	vecToIndex(to, m_iTargetCell);

	updateAlgorithm();

	std::list<BoyLib::Vector2> result;
	BoyLib::Vector2 point;

	// Convert the points into vectors
	for( std::list<int>::iterator it = m_Path.begin() ; it != m_Path.end() ; it++ )
	{
		if( indexToVec(point, *it) )
		{
			result.push_back(point);
		}
		else
		{
			result.clear();
			break;
		}
	}

	return result;
}

//------------------------- CreatePathDFS --------------------------------
//
//  uses DFS to find a path between the start and target cells.
//  Stores the path as a series of node indexes in m_Path.
//------------------------------------------------------------------------
void BattleMap::createPathDFS()
{
	//set current algorithm
	m_CurrentAlgorithm = AT_DFS;

	//clear any existing path
	m_Path.clear();
	m_SubTree.clear();

	//create and start a timer
	PrecisionTimer timer; timer.Start();

	//do the search
	Graph_SearchDFS<NavGraph> DFS(*m_pGraph, m_iSourceCell, m_iTargetCell);

	//record the time taken  
	m_dTimeTaken = timer.TimeElapsed();

	//now grab the path (if one has been found)
	if (DFS.Found())
	{
		m_Path = DFS.GetPathToTarget();
	}

	m_SubTree = DFS.GetSearchTree();

	m_dCostToTarget = 0.0;
}


//------------------------- CreatePathBFS --------------------------------
//
//  uses BFS to find a path between the start and target cells.
//  Stores the path as a series of node indexes in m_Path.
//------------------------------------------------------------------------
void BattleMap::createPathBFS()
{
	//set current algorithm
	m_CurrentAlgorithm = AT_BFS;

	//clear any existing path
	m_Path.clear();
	m_SubTree.clear();

	//create and start a timer
	PrecisionTimer timer; timer.Start();

	//do the search
	Graph_SearchBFS<NavGraph> BFS(*m_pGraph, m_iSourceCell, m_iTargetCell);

	//record the time taken  
	m_dTimeTaken = timer.TimeElapsed();

	//now grab the path (if one has been found)
	if (BFS.Found())
	{
		m_Path = BFS.GetPathToTarget();
	}

	m_SubTree = BFS.GetSearchTree();

	m_dCostToTarget = 0.0;
}

//-------------------------- CreatePathDijkstra --------------------------
//
//  creates a path from m_iSourceCell to m_iTargetCell using Dijkstra's algorithm
//------------------------------------------------------------------------
void BattleMap::createPathDijkstra()
{
	//set current algorithm
	m_CurrentAlgorithm = AT_DIJKSTRA;

	//create and start a timer
	PrecisionTimer timer; timer.Start();

	Graph_SearchDijkstra<NavGraph> djk(*m_pGraph, m_iSourceCell, m_iTargetCell);

	//record the time taken  
	m_dTimeTaken = timer.TimeElapsed();

	m_Path = djk.GetPathToTarget();

	m_SubTree = djk.GetSPT();

	m_dCostToTarget = djk.GetCostToTarget();
}

//--------------------------- CreatePathAStar ---------------------------
//------------------------------------------------------------------------
void BattleMap::createPathAStar()
{
	//set current algorithm
	m_CurrentAlgorithm = AT_ASTAR;

	//create and start a timer
	PrecisionTimer timer; timer.Start();

	//create a couple of typedefs so the code will sit comfortably on the page   
	typedef Graph_SearchAStar<NavGraph, Heuristic_Euclid> AStarSearch;

	//create an instance of the A* search using the Euclidean heuristic
	AStarSearch AStar(*m_pGraph, m_iSourceCell, m_iTargetCell);

	//record the time taken  
	m_dTimeTaken = timer.TimeElapsed();
	m_Path = AStar.GetPathToTarget();
	m_SubTree = AStar.GetSPT();
	m_dCostToTarget = AStar.GetCostToTarget();
}