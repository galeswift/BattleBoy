#include "Globals.h"

BattleBoy *BattleBoy::gInstance = NULL;

BattleBoy::BattleBoy()
{
	mLoadComplete = false;
	bPaused = false;
	mRole = Networking::ROLE_None;
	mDebugDrawMode = EDebugDrawMode_ALL;
	mNetInterface = NULL;
	mGui = NULL;
}

BattleBoy::~BattleBoy()
{
	// stop listening to the keyboard:
	Boy::Environment::instance()->getKeyboard(0)->removeListener(this);
}

BattleBoy *BattleBoy::instance()
{
	if (gInstance==NULL)
	{
		gInstance = new BattleBoy();
	}

	return gInstance;
}

void BattleBoy::destroy()
{
	// destroy the singleton:
	assert(gInstance!=NULL);
	delete gInstance;
	gInstance = NULL;
}

void BattleBoy::init(int argc, char* argv[])
{
	parseCommandArgs(argc, argv);
	if( mRole == Networking::ROLE_Authority)
	{
		printf("Starting server...\n");
		mNetInterface = new Networking::NetworkInterfaceServer();
	}
	else if( mRole == Networking::ROLE_SimulatedProxy )
	{
		printf("Client initialized, trying to connect to %s\n",mPendingAddress.c_str());
		mNetInterface = new Networking::NetworkInterfaceClient(mPendingAddress);
	}
	
	if( mNetInterface != NULL )
	{
		mNetInterface->Init();
	}
	else
	{
		printf("Starting offline game...\n");
	}

	mKeyToSpawnInfo['Q'] = SpawnInfo(ESpawnType_ROCK,1);
	mKeyToSpawnInfo['W'] = SpawnInfo(ESpawnType_PAPER,1);
	mKeyToSpawnInfo['E'] = SpawnInfo(ESpawnType_SCISSORS,1);
	
	mKeyToSpawnInfo['A'] = SpawnInfo(ESpawnType_ROCK,0);
	mKeyToSpawnInfo['S'] = SpawnInfo(ESpawnType_PAPER,0);
	mKeyToSpawnInfo['D'] = SpawnInfo(ESpawnType_SCISSORS,0);

	mKeyToCommand['P'] = &BattleBoy::togglePause;
	mKeyToCommand['O'] = &BattleBoy::toggleDebugDraw;
	mKeyToCommand['R'] = &BattleBoy::reset;

	initGui();
}

void BattleBoy::initGui()
{
	mGui = new BattleGui(this);
	mGui->init();

	// Start listening to the mouse
	Boy::Environment::instance()->getKeyboard(0)->addListener(mGui);
	Boy::Environment::instance()->getMouse(0)->addListener(mGui);
}

void BattleBoy::togglePause()
{
	bPaused = !bPaused;
}

void BattleBoy::toggleDebugDraw()
{
	mDebugDrawMode++;

	if( mDebugDrawMode == EDebugDrawMode_MAX )
	{
		mDebugDrawMode = EDebugDrawMode_NONE;
	}
}

void BattleBoy::reset()
{
	// Clear the actor array
	mActors.clear();

	// Add the controllers
	Controller_Player *newPlayer= new Controller_Player();
	newPlayer->setTeamIdx(0);
	addActor(newPlayer);

	Controller_AI *newAI= new Controller_AI();
	newAI->setTeamIdx(1);
	addActor(newAI);

	// Add the buildings
	Unit_Building *newBuilding = new Unit_Building(BoyLib::Vector2(125.0f,670.0f));
	newBuilding->setTeamIdx(0);
	addActor(newBuilding);

	newBuilding = new Unit_Building(BoyLib::Vector2(850.0f,100.0f));
	newBuilding->setTeamIdx(1);
	addActor(newBuilding);
}

void BattleBoy::parseCommandArgs(int argc, char* argv[])
{
	if(CommandOptions::cmdOptionExists(argv, argv + argc, "--server"))
	{
		mRole = Networking::ROLE_Authority;
	}
	else if( CommandOptions::cmdOptionExists(argv, argv + argc, "--client"))
	{
		mRole = Networking::ROLE_SimulatedProxy;
		mPendingAddress = CommandOptions::getCmdOption(argv, argv+argc, "--client");
	}
}

void BattleBoy::load()
{
	// load the common resource group:
	Boy::ResourceManager *rm = Boy::Environment::instance()->getResourceManager();
	rm->parseResourceFile("res/resources.xml",NULL);
	rm->loadResourceGroup("common");
}


void BattleBoy::loadComplete()
{
	debug_con<<"Debug console created."<<"";
	// start listening to the keyboard:
	Boy::Environment::instance()->getKeyboard(0)->addListener(this);

	// fetch whatever resources we need:
	Boy::ResourceManager *rm = Boy::Environment::instance()->getResourceManager();
	
	mFont = rm->getFont("FONT_MAIN");
	
	// set the load complete flag (this will trigger 
	// the start of the game in the update method):
	mLoadComplete = true;

	// Reset the game
	reset();
}

void BattleBoy::update(float dt)
{
	if( bPaused )
	{
		return;
	}
	
	if( mGui )
	{
		mGui->update(dt);
	}

	for( unsigned int i=0 ; i<mActors.size() ; )
	{
		Actor* actor = mActors[i];
		if (actor->isDestroyed() && actor->shouldErase())
		{
			mActors.erase(mActors.begin() + i);
		}
		else
		{
			actor->update(dt);
			i++;
		}
	}
}

void BattleBoy::draw(Boy::Graphics *g)
{
	if( mLoadComplete ) 
	{
		for( std::vector<Actor*>::iterator it = mActors.begin(); it != mActors.end() ; ++it )
		{
			(*it)->draw(g);
		}
	}
}

void BattleBoy::addActor(Actor* a)
{
	mActors.push_back(a);
	a->init();
}

// return true if this actor is outside the bounds of the window
bool BattleBoy::isOutOfBounds(Actor* a)
{
	bool result = false;

	int w = Boy::Environment::screenWidth();
	int h = Boy::Environment::screenHeight();
	
	if( a->getPos().x < -a->getSize() || a->getPos().x > w + a->getSize() ||
		a->getPos().y < -a->getSize() || a->getPos().y > h + a->getSize() )
	{
		result = true;
	}

	return result;
}


void BattleBoy::keyUp(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods)
{	
	mPendingSpawnInfo.type = ESpawnType_NONE;

	std::map<wchar_t,keyCommand>::iterator keyBinding = mKeyToCommand.find(unicode);
	if( keyBinding != mKeyToCommand.end() )
	{
		keyCommand func = keyBinding->second;
		(this->*func)();
	}

	// Look through our keybindings and see if we have a result
	std::map<wchar_t,SpawnInfo>::iterator spawnBinding = mKeyToSpawnInfo.find(unicode);
	if( spawnBinding != mKeyToSpawnInfo.end() )
	{
		mPendingSpawnInfo = (*spawnBinding).second;
	}

	if( mPendingSpawnInfo.type != ESpawnType_NONE )
	{
		spawnUnit(mPendingSpawnInfo.type, mPendingSpawnInfo.playerIdx);
	}
}

void BattleBoy::keyDown(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods)
{
}

void BattleBoy::spawnUnit(ESpawnType unitType, int teamIdx)
{
	Unit *newUnit = NULL;

	BoyLib::Vector2 spawnPos = getSpawnPos(teamIdx);
	BoyLib::Vector2 targetPos = getSpawnPos(teamIdx == 0 ? 1 : 0);

	switch (unitType)
	{
		case ESpawnType_ROCK:
			newUnit = new Unit_Rock(spawnPos, 100) ;		
			break;
		case ESpawnType_PAPER:
			newUnit = new Unit_Paper(spawnPos, 100) ;
			break;
		case ESpawnType_SCISSORS:
			newUnit = new Unit_Scissors(spawnPos, 100) ;
			break;
		default:
			break; 
	}

	if( newUnit != NULL )
	{
		addActor(newUnit);	
		newUnit->setOwningGame(this);
		newUnit->setTeamIdx(teamIdx);
		newUnit->getSteering()->setTarget(targetPos);
	}
}

const BoyLib::Vector2 BattleBoy::getSpawnPos( int playerIdx )
{
	int w = Boy::Environment::screenWidth();
	int h = Boy::Environment::screenHeight();
	BoyLib::Vector2 variance( randf(-15.0f,15.0f),randf(-15.0f,15.0f));
	BoyLib::Vector2 result;
	switch(playerIdx)
	{
	case 0:
		result = variance + BoyLib::Vector2(125.0f,670.0f);
		break;
		break;
	case 1:
		result = variance + BoyLib::Vector2(850.0f,100.0f);
		break;
	}
	return result;
}


Unit* BattleBoy::closestEnemy(Unit* unit, float range)
{
	Unit* result = NULL;

	bool bFound = false;
	float closestDist=-1;
	for( std::vector<Actor*>::iterator it = mActors.begin(); it != mActors.end(); it++ )
	{
		Unit* enemyUnit = dynamic_cast<Unit*>( *it );
		if( enemyUnit && !enemyUnit->isDestroyed() && enemyUnit->getTeamIdx() != unit->getTeamIdx() )
		{
			float enemyDist = dist(unit->getPos().x, unit->getPos().y, enemyUnit->getPos().x, enemyUnit->getPos().y);
			if( range < 0 ||
				(enemyDist < unit->getRange() && (closestDist <= 0 || enemyDist < closestDist)))
			{
				closestDist = enemyDist;
				result = enemyUnit;
				break;
			}
		}
	}

	return result;
}