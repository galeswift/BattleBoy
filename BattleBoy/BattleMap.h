#pragma once

class BattleBoy;

class BattleMap
{
public:	
	
	enum ETileType
	{
		TT_SPAWN = 0,
		TT_PATH = 64,
	};

	enum ESpawnType
	{
		ST_NONE	   = 0,
		ST_PLAYER1 = 64,
		ST_PLAYER2 = 128,
		ST_PLAYER3 = 192,
		ST_PLAYER4 = 255,
	};

	enum EPathType
	{
		BT_NORMAL   = 0,
		BT_OBSTACLE = 64,
		BT_MUD = 128, // not used, but weighted stronger against
		BT_WATER = 192,
	};

	enum EAlgorithmType
	{
		AT_NONE,
		AT_ASTAR,
		AT_BFS,
		AT_DFS,
		AT_DIJKSTRA
	};

	BattleMap(BattleBoy* owner);
	virtual void init();
	void draw( Boy::Graphics* g );
	void createGraph(int numCellsX, int numCellsY);

	//converts a vector to an index into the graph. Returns false if p
	//is invalid
	bool vecToIndex(const BoyLib::Vector2& v, int& NodeIndex);
	void updateGraphFromBrush(int brush, int cellIndex);
	double getTerrainCost(const EPathType brush);
	//this calls the appropriate algorithm	
	void  updateAlgorithm();
private:
	// Owning game
	BattleBoy* mOwner;
	
	// The current map
	BoyLib::tImageTGA* mMapData;

	//create a typedef for the graph type
	typedef SparseGraph<NavGraphNode<void*>, GraphEdge> NavGraph;
	NavGraph* m_pGraph;

	//this vector will store any path returned from a graph search
	std::list<int> m_Path;

	//this vector of edges is used to store any subtree returned from 
	//any of the graph algorithms (such as an SPT)
	std::vector<const GraphEdge*> m_SubTree;

	//the terrain type of each cell
	std::vector<int> m_TerrainType;
	
	//the currently selected algorithm
	int m_CurrentAlgorithm;

	//number of cells vertically and horizontally
	int m_iCellsX;
	int m_iCellsY;

	//local record of the client area
	int m_icxClient;
	int m_icyClient;
	//the indices of the source and target cells
	int m_iSourceCell;
	int m_iTargetCell;

	//the dimensions of the cells
	double m_dCellWidth;
	double m_dCellHeight;

	//the total cost of the path from target to source
	double  m_dCostToTarget;

	//holds the time taken for the most currently used algorithm to complete
	double m_dTimeTaken;

	//the algorithms
	void createPathDFS();
	void createPathBFS();
	void createPathDijkstra();
	void createPathAStar();
};