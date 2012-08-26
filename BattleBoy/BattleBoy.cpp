#include "Globals.h"

BattleBoy *BattleBoy::gInstance = NULL;

BattleBoy::BattleBoy()
{
	mBoard = NULL;
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
	mBoard = new Board();
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

	mKeyToSpawnInfo['Q'] = SpawnInfo(ESpawnType_ROCK,0);
	mKeyToSpawnInfo['W'] = SpawnInfo(ESpawnType_PAPER,0);
	mKeyToSpawnInfo['E'] = SpawnInfo(ESpawnType_SCISSORS,0);
	
	mKeyToSpawnInfo['A'] = SpawnInfo(ESpawnType_ROCK,1);
	mKeyToSpawnInfo['S'] = SpawnInfo(ESpawnType_PAPER,1);
	mKeyToSpawnInfo['D'] = SpawnInfo(ESpawnType_SCISSORS,1);
	
	int w = Boy::Environment::screenWidth();
	int h = Boy::Environment::screenHeight();

	PlayerCredits = 0;
	AICredits = 0;

	bAutoPlay = false;
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
}

void BattleBoy::update(float dt)
{
	for( std::vector<Actor*>::iterator it = mActors.begin(); it != mActors.end() ; )
	{
		if ((*it)->isDestroyed())
		{
			// TODO MAKE THIS WORK!
			it = mActors.erase(it);
		}
		else
		{
			(*it)->update(dt);
			++it;
		}
	}

	// HACK Play AI versus AI
	if (bAutoPlay)
	{
		// HACK Should add based on time, not every tick
		PlayerCredits += 1;
		AICredits += 1;

		// HACK Need unit costs (with some variation)
		bool bUseRandomCosts = true; // Set to false for no randomness for debugging
		int UnitCost = 500;
		if (bUseRandomCosts)
		{
			UnitCost = rand() % 200 + 400;
		}

		// HACK AI
		if (PlayerCredits > UnitCost)
		{
			PlayerCredits -= UnitCost;
			ESpawnType unitType = ESpawnType(rand() % 3 + 4);
			spawnUnit(unitType, 0);
		}

		if (bUseRandomCosts)
		{
			UnitCost = rand() % 200 + 400;
		}

		if (AICredits > UnitCost)
		{
			AICredits -= UnitCost;
			ESpawnType unitType = ESpawnType(rand() % 3 + 1);
			spawnUnit(unitType,1);
		}
	}
}

void BattleBoy::draw(Boy::Graphics *g)
{
	if( mLoadComplete ) 
	{
		mBoard->draw(g);

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
	char AICreditsText[100];
	sprintf_s(AICreditsText, "%i", AICredits);
	char PlayerCreditsText[100];
	sprintf_s(PlayerCreditsText, "%i", PlayerCredits);

	g->setColorizationEnabled(true);
	g->setColor(0xffffffff);
	g->pushTransform();
	g->translate(50.0f,50.0f);
	mFont->drawString(g,AICreditsText,0.5f);
	g->translate(0.0f,50.0f);
	mFont->drawString(g,PlayerCreditsText,0.5f);
	g->popTransform();
	g->setColorizationEnabled(false);
}

void BattleBoy::drawDebugText(Boy::Graphics *g)
{
	char KDebugText[100];
	sprintf_s(KDebugText, "K to kill all units");
	char VDebugText[100];
	sprintf_s(VDebugText, "V to start autoplay");
	char RDebugText[100];
	sprintf_s(RDebugText, "R to restart");

	int h = Boy::Environment::screenHeight();

	g->setColorizationEnabled(true);
	g->setColor(0xffffffff);
	g->pushTransform();
	g->translate(50.0f,h - 150.0f);
	mFont->drawString(g,KDebugText,0.25f);
	g->translate(0.0f,50.0f);
	mFont->drawString(g,VDebugText,0.25f);
	g->translate(0.0f,50.0f);
	mFont->drawString(g,RDebugText,0.25f);
	g->popTransform();
	g->setColorizationEnabled(false);
}

void BattleBoy::keyUp(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods)
{
	int w = Boy::Environment::screenWidth();
	int h = Boy::Environment::screenHeight();

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
		case 'V':
			setAutoplay(!bAutoPlay);
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
		if (dynamic_cast<Unit *>(*it))
		{
			(*it)->setDestroyed( true );
		}
	}
}

void BattleBoy::setAutoplay(bool bOn)
{
	bAutoPlay = bOn;
}

void BattleBoy::restart()
{
	setAutoplay(false);
	AICredits = 0;
	PlayerCredits = 0;
	mActors.empty();
}

void BattleBoy::spawnUnit(ESpawnType unitType, int teamIdx)
{
	Unit *newUnit = NULL;
	BoyLib::Vector2 spawnPos = getSpawnPos(teamIdx);
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
		mActors.push_back(newUnit);	
	}
}

const BoyLib::Vector2 BattleBoy::getSpawnPos( int playerIdx )
{
	int w = Boy::Environment::screenWidth();
	int h = Boy::Environment::screenHeight();
	BoyLib::Vector2 result;
	switch(playerIdx)
	{
	case 0:
		result = BoyLib::Vector2(w/2.0f,float(h-150));
	case 1:
		result = BoyLib::Vector2(w/2.0f,100.0);
	}
	return result;
}