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

	//int w = Boy::Environment::screenWidth();
	//int h = Boy::Environment::screenHeight();

	Unit_Building *newBuilding = new Unit_Building(BoyLib::Vector2(125.0f,670.0f));
	newBuilding->initStats();
	newBuilding->setTeamIdx(0);
	mActors.push_back(newBuilding);

	newBuilding = new Unit_Building(BoyLib::Vector2(850.0f,100.0f));
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
		if ((*it)->isDestroyed() && !dynamic_cast<Unit_Building*>(*it))
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
			spawnUnit(unitType, playerIdxCount, rand()%3, 6);
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
	g->translate(220.0f,200.0f);
	mFont->drawString(g,P2CreditsText,0.5f);
	g->translate(0.0f,40.0f);
	mFont->drawString(g,P1CreditsText,0.5f);
	g->popTransform();
	g->setColorizationEnabled(false);
}

void BattleBoy::drawDebugText(Boy::Graphics *g)
{
	char TUDebugText[100];
	sprintf_s(TUDebugText,	"QWE pick top Unit");
	char BUDebugText[100];
	sprintf_s(BUDebugText,	"ASD  pick bot Unit");
	char LSDebugText[100];
	sprintf_s(LSDebugText,	"123    lane  spawn");
	char KDebugText[100];
	sprintf_s(KDebugText,	"K     kill all");
	char HDebugText[100];
	sprintf_s(HDebugText,	"H     heal all");
	char TDebugText[100];
	sprintf_s(TDebugText,	"T     AI(TOP)");
	char VDebugText[100];
	sprintf_s(VDebugText,	"V     autoplay");
	char RDebugText[100];
	sprintf_s(RDebugText,	"R     restart");
	char PlayerDebugText[100];
	sprintf_s(PlayerDebugText,	"Player");
	char AIDebugText[100];
	sprintf_s(AIDebugText,		"  AI  ");
	char NSDebugText[100];
	sprintf_s(NSDebugText, "Next Spawn");

	int h = Boy::Environment::screenHeight();

	g->setColorizationEnabled(true);
	g->setColor(0xffffffff);

	g->pushTransform();
	g->translate(500.0f,460.0f);
	mFont->drawString(g,TUDebugText,0.25f);
	g->translate(0.0f,20.0f);
	mFont->drawString(g,BUDebugText,0.25f);
	g->translate(0.0f,20.0f);
	mFont->drawString(g,LSDebugText,0.25f);
	g->translate(0.0f,20.0f);
	mFont->drawString(g,KDebugText,0.25f);
	g->translate(0.0f,20.0f);
	mFont->drawString(g,HDebugText,0.25f);
	g->translate(0.0f,20.0f);
	mFont->drawString(g,TDebugText,0.25f);
	g->translate(0.0f,20.0f);
	mFont->drawString(g,VDebugText,0.25f);
	g->translate(0.0f,20.0f);
	mFont->drawString(g,RDebugText,0.25f);
	g->popTransform();

	// Draw next spawn
	g->pushTransform();
	g->translate(175.0f,300.0f);
	mFont->drawString(g,NSDebugText,0.25f);	
	g->popTransform();

	g->pushTransform();
	g->translate(265.0f,340.0f);
	if (mPendingSpawnInfo.playerIdx == 0)
	{
		g->drawImage(Boy::Environment::getImage("IMAGE_BLUETEAM"));
	}
	else
	{
		g->drawImage(Boy::Environment::getImage("IMAGE_REDTEAM"));
	}
	switch (mPendingSpawnInfo.type)
	{
		case ESpawnType_ROCK:
			g->drawImage(Boy::Environment::getImage("IMAGE_ROCK"));
			break;
		case ESpawnType_PAPER:
			g->drawImage(Boy::Environment::getImage("IMAGE_PAPER"));
			break;
		case ESpawnType_SCISSORS:
			g->drawImage(Boy::Environment::getImage("IMAGE_SCISSORS"));
			break;
		default:
			break; 
	}
	g->popTransform();
	
	for( std::vector<Actor*>::iterator it = mActors.begin(); it != mActors.end() ; ++it )
	{
		Unit_Building* building = dynamic_cast<Unit_Building*>(*it);
		if (building)
		{
			g->pushTransform();				
			g->translate(building->getPos().x, building->getPos().y);
			if (building->getTeamIdx() == 0)
			{
				g->translate(-50.0f,40.0f);
			}
			else
			{
				g->translate(-50.0f,-40.0f);
			}
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
	if( keyBinding != mKeyToSpawnInfo.end() )
	{
		mPendingSpawnInfo = (*keyBinding).second;
	}

	bool bSpawnUnits = false;
	int lane = 0;
	switch (unicode)
	{
		case '1':
			lane = 0; //Top
			bSpawnUnits = true;
			break;
		case '2':
			lane = 1; //Mid
			bSpawnUnits = true;
			break;
		case '3':
			lane = 2; //Bot
			bSpawnUnits = true;
			break;
	}

	if( bSpawnUnits && mPendingSpawnInfo.type != ESpawnType_NONE )
	{
		spawnUnit(mPendingSpawnInfo.type, mPendingSpawnInfo.playerIdx, lane, 6);
	}

	//CHEATS
	switch (unicode)
	{
		case 'K':
			killAllUnitsCheat();
			break;
		case 'H':
			healAllUnitsCheat();
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

void BattleBoy::healAllUnitsCheat()
{
	for( std::vector<Actor*>::iterator it = mActors.begin(); it != mActors.end() ; ++it )
	{
		Unit* unit = dynamic_cast<Unit *>(*it);
		if (unit)
		{
			unit->setHealth(unit->getMaxHealth());
			unit->setDestroyed(false);
		}
	}
}

void BattleBoy::restart()
{
	mPlayers[0]->init();
	mPlayers[1]->init();
	killAllUnitsCheat();
	healAllUnitsCheat();
}

void BattleBoy::spawnUnit(ESpawnType unitType, int teamIdx, int lane, int amount)
{
	Unit *newUnit = NULL;
	// HACK Need unit costs (with some variation)
	int UnitCost = 180;
	int CurrentCredits = mPlayers[teamIdx]->getCredits();
	if (CurrentCredits >= UnitCost)
	{
		mPlayers[teamIdx]->setCredits(CurrentCredits - UnitCost);

		Unit *newUnit = NULL;

		for (int i = 0; i < amount; i++)
		{		
			BoyLib::Vector2 spawnPos = getSpawnPos(teamIdx);
			BoyLib::Vector2 targetPos = getSpawnPos(teamIdx == 0 ? 1 : 0);
			// HACK until can do proper pathing/destinations
			if(lane == 0)
			{
				targetPos = BoyLib::Vector2(135.0f,105.0f);
			}
			else if (lane == 2)
			{
				targetPos = BoyLib::Vector2(845.0f,650.0f);
			}

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