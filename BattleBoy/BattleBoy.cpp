#include "Globals.h"

BattleBoy *BattleBoy::gInstance = NULL;

BattleBoy::BattleBoy()
{
	mMap = NULL;
	mLoadComplete = false;
	mRole = Networking::ROLE_None;
	mNetInterface = NULL;
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
	mMap = new Map();
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
	// start listening to the keyboard:
	Boy::Environment::instance()->getKeyboard(0)->addListener(this);

	// fetch whatever resources we need:
	Boy::ResourceManager *rm = Boy::Environment::instance()->getResourceManager();
	
	mFont = rm->getFont("FONT_MAIN");
	
	// set the load complete flag (this will trigger 
	// the start of the game in the update method):
	mLoadComplete = true;

	/*mShipImage = rm->getImage("IMAGE_SHIP");
	mThrustImage = rm->getImage("IMAGE_THRUST");
	mBoomSound = rm->getSound("SOUND_BOOM");
	mFireSound = rm->getSound("SOUND_FIRE");
	mThrustSound = rm->getSound("SOUND_THRUST");*/

	int w = Boy::Environment::screenWidth();
	int h = Boy::Environment::screenHeight();

	Unit_Building *newBuilding = new Unit_Building(BoyLib::Vector2(w/2.0f,float(h-100)));
	newBuilding->initStats();
	newBuilding->setTeamIdx(0);
	mActors.push_back(newBuilding);

	newBuilding = new Unit_Building(BoyLib::Vector2(w/2.0f,100.0));
	newBuilding->initStats();
	newBuilding->setTeamIdx(1);
	mActors.push_back(newBuilding);

	Player *newPlayer = new Player();
	newPlayer->init();
	mPlayers.push_back(newPlayer);

	newPlayer = new Player();
	newPlayer->init();
	mPlayers.push_back(newPlayer);
}

void BattleBoy::update(float dt)
{
	for( std::vector<Actor*>::iterator it = mActors.begin(); it != mActors.end() ; )
	{
		if ((*it)->isDestroyed())
		{
			it = mActors.erase(it);
		}
		else
		{
			(*it)->update(dt);
			++it;
		}
	}

	int playerIdxCount = 0;
	for( std::vector<Player*>::iterator it = mPlayers.begin(); it != mPlayers.end(); ++it)
	{
		// Update the player
		(*it)->update(dt);

		// Spawn AI Units
		if ((*it)->isAI())
		{			
			ESpawnType unitType = ESpawnType(rand() % ESpawnType_MAX + 1);
			spawnUnit(unitType, playerIdxCount);
		}
		playerIdxCount++;
	}
}

void BattleBoy::draw(Boy::Graphics *g)
{
	if( mLoadComplete ) 
	{
		mMap->draw(g);

		for( std::vector<Actor*>::iterator it = mActors.begin(); it != mActors.end() ; ++it )
		{
			(*it)->draw(g);
		}

		// draw resources
		drawResources(g);

		//draw debug text
		drawDebugText(g);
	}
}

void BattleBoy::drawResources(Boy::Graphics *g)
{
	char P1CreditsText[100];
	sprintf_s(P1CreditsText, "%i", mPlayers[0]->getCredits());
	char P2CreditsText[100];
	sprintf_s(P2CreditsText, "%i", mPlayers[1]->getCredits());

	g->setColorizationEnabled(true);
	g->setColor(0xffffffff);
	g->pushTransform();
	g->translate(50.0f,50.0f);
	mFont->drawString(g,P2CreditsText,0.5f);
	g->translate(0.0f,50.0f);
	mFont->drawString(g,P1CreditsText,0.5f);
	g->popTransform();
	g->setColorizationEnabled(false);
}

void BattleBoy::drawDebugText(Boy::Graphics *g)
{
	char KDebugText[100];
	sprintf_s(KDebugText, "K to kill all units");
	char TDebugText[100];
	sprintf_s(TDebugText, "T toggle vs AI(TOP)");
	char VDebugText[100];
	sprintf_s(VDebugText, "V to toggle autoplay");
	char RDebugText[100];
	sprintf_s(RDebugText, "R to restart");
	char PlayerDebugText[100];
	sprintf_s(PlayerDebugText, "<---- Player");
	char AIDebugText[100];
	sprintf_s(AIDebugText, "<---- AI");

	int h = Boy::Environment::screenHeight();

	g->setColorizationEnabled(true);
	g->setColor(0xffffffff);

	g->pushTransform();
	g->translate(50.0f,h - 200.0f);
	mFont->drawString(g,KDebugText,0.25f);
	g->translate(0.0f,50.0f);
	mFont->drawString(g,TDebugText,0.25f);
	g->translate(0.0f,50.0f);
	mFont->drawString(g,VDebugText,0.25f);
	g->translate(0.0f,50.0f);
	mFont->drawString(g,RDebugText,0.25f);
	g->popTransform();
	
	for( std::vector<Actor*>::iterator it = mActors.begin(); it != mActors.end() ; ++it )
	{
		Unit_Building* building = dynamic_cast<Unit_Building*>(*it);
		if (building)
		{
			g->pushTransform();				
			g->translate(building->getPos().x, building->getPos().y);
			g->translate(100.0f,30.0f);
			if (mPlayers[building->getTeamIdx()]->isAI())
			{
				mFont->drawString(g,AIDebugText,0.25f);
			}
			else
			{
				mFont->drawString(g,PlayerDebugText,0.25f);
			}
			g->popTransform();
		}
	}

	g->setColorizationEnabled(false);
}

void BattleBoy::keyUp(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods)
{
	// Look through our keybindings and see if we have a result
	std::map<wchar_t,SpawnInfo>::iterator keyBinding = mKeyToSpawnInfo.find(unicode);
	SpawnInfo spawnInfo;
	if( keyBinding != mKeyToSpawnInfo.end() )
	{
		spawnInfo = (*keyBinding).second;
	}

	if( spawnInfo.type != ESpawnType_NONE )
	{
		spawnUnit(spawnInfo.type, spawnInfo.playerIdx);
	}

	//CHEATS
	switch (unicode)
	{
		case 'K':
			killAllUnitsCheat();
			break;
		case 'T':
			mPlayers[1]->setIsAI(!mPlayers[1]->isAI());
			break;
		case 'V':
			mPlayers[0]->setIsAI(!mPlayers[0]->isAI());
			mPlayers[1]->setIsAI(mPlayers[0]->isAI());
			break;
		case 'R':
			restart();
			break;
	}
}

void BattleBoy::keyDown(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods)
{
}

void BattleBoy::killAllUnitsCheat()
{
	for( std::vector<Actor*>::iterator it = mActors.begin(); it != mActors.end() ; ++it )
	{
		if (dynamic_cast<Unit *>(*it) && !dynamic_cast<Unit_Building *>(*it))
		{
			(*it)->setDestroyed( true );
		}
	}
}

void BattleBoy::restart()
{
	mPlayers[0]->init();
	mPlayers[1]->init();
	killAllUnitsCheat();
}

void BattleBoy::spawnUnit(ESpawnType unitType, int teamIdx)
{
	Unit *newUnit = NULL;
	// HACK Need unit costs (with some variation)
	int UnitCost = 20;
	int CurrentCredits = mPlayers[teamIdx]->getCredits();
	if (CurrentCredits > UnitCost)
	{
		mPlayers[teamIdx]->setCredits(CurrentCredits - UnitCost);

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
			newUnit->initStats();
			newUnit->setOwningGame(this);
			newUnit->setTeamIdx(teamIdx);
			newUnit->getSteering()->arriveOn();
			newUnit->getSteering()->separationOn();
			newUnit->getSteering()->setTarget(targetPos);
			mActors.push_back(newUnit);	
		}
	}
}

const BoyLib::Vector2 BattleBoy::getSpawnPos( int playerIdx )
{
	int w = Boy::Environment::screenWidth();
	int h = Boy::Environment::screenHeight();
	BoyLib::Vector2 variance( randf(-5.0f,5.0f),randf(-5.0f,5.0f));
	BoyLib::Vector2 result;
	switch(playerIdx)
	{
	case 0:
		result = variance + BoyLib::Vector2(w/2.0f,float(h-150));
		break;
		break;
	case 1:
		result = BoyLib::Vector2(w/2.0f,150.0);
		break;
	}
	return result;
}