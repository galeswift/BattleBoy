#include "Globals.h"

BattleBoy *BattleBoy::gInstance = NULL;

BattleBoy::BattleBoy()
{
	mBoard = NULL;
	mLoadComplete = false;
	mPendingSpawnType = ESpawnType_NONE;
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

	mKeyToSpawnType['q'] = ESpawnType_AI_ROCK;
	mKeyToSpawnType['w'] = ESpawnType_AI_PAPER;
	mKeyToSpawnType['e'] = ESpawnType_AI_SCISSORS;
	mKeyToSpawnType['a'] = ESpawnType_PLAYER_ROCK;
	mKeyToSpawnType['s'] = ESpawnType_PLAYER_PAPER;
	mKeyToSpawnType['d'] = ESpawnType_PLAYER_SCISSORS;
	
	int w = Boy::Environment::screenWidth();
	int h = Boy::Environment::screenHeight();
	mActors.push_back( new SpawnPoint(BoyLib::Vector2(w/2.0f,float(h-150))) );
	mActors.back()->Team = ESpawnType_Player;
	mActors.push_back( new SpawnPoint(BoyLib::Vector2(w/2.0f,150.0)) );
	mActors.back()->Team = ESpawnType_AI;
	mActors.push_back( new Building(BoyLib::Vector2(w/2.0f,float(h-100))) );
	mActors.back()->Team = ESpawnType_Player;
	mActors.push_back( new Building(BoyLib::Vector2(w/2.0f,100.0)) );
	mActors.back()->Team = ESpawnType_AI;

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
}

void BattleBoy::update(float dt)
{
	for( std::vector<Actor*>::iterator it = mActors.begin(); it != mActors.end() ; ++it )
	{
		if ((*it)->bDead)
		{
			// TODO MAKE THIS WORK!
			//it = mActors.erase(it);
		}
		else
		{
			(*it)->update(dt, mActors);
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
			SpawnUnit(unitType);
		}

		if (bUseRandomCosts)
		{
			UnitCost = rand() % 200 + 400;
		}

		if (AICredits > UnitCost)
		{
			AICredits -= UnitCost;
			ESpawnType unitType = ESpawnType(rand() % 3 + 1);
			SpawnUnit(unitType);
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

		for( std::vector<Actor*>::iterator it = mActors.begin(); it != mActors.end() ; ++it )
		{
			(*it)->drawHealth(g);
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

	SpawnUnit(mPendingSpawnType);

	mPendingSpawnType = ESpawnType_NONE;

	//CHEATS
	switch (unicode)
	{
		case 'K':
			KillAllUnitsCheat();
			break;
		case 'V':
			SetAutoplay(!bAutoPlay);
			break;
		case 'R':
			Restart();
			break;
	}
}

void BattleBoy::keyDown(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods)
{
	// Look through our keybindings and see if we have a result
	std::map<wchar_t,ESpawnType>::iterator keyBinding = mKeyToSpawnType.find(unicode);
	if( keyBinding != mKeyToSpawnType.end() )
	{
		mPendingSpawnType = (*keyBinding).second;
	}
}

BoyLib::Vector2 BattleBoy::getBuildingInfo(int whichPlayer)
{
	std::vector<Actor *> buildings;

	for( std::vector<Actor*>::iterator it = BattleBoy::mActors.begin(); it != BattleBoy::mActors.end() ; ++it )
	{
		if (dynamic_cast<Building *>(*it))
		{
			buildings.push_back(*it);
		}
	}

	return buildings[whichPlayer]->pos;
}

void BattleBoy::KillAllUnitsCheat()
{
	for( std::vector<Actor*>::iterator it = mActors.begin(); it != mActors.end() ; ++it )
	{
		if (dynamic_cast<Unit *>(*it))
		{
			(*it)->bDead = true;
		}
	}
}

void BattleBoy::SetAutoplay(bool bOn)
{
	bAutoPlay = bOn;
}

void BattleBoy::Restart()
{
	SetAutoplay(false);
	AICredits = 0;
	PlayerCredits = 0;
	KillAllUnitsCheat();
	for( std::vector<Actor*>::iterator it = mActors.begin(); it != mActors.end() ; ++it )
	{
		(*it)->Health = (*it)->MaxHealth;
	}
}

void BattleBoy::SpawnUnit(ESpawnType Unit)
{
	switch (Unit)
	{
		case ESpawnType_AI_ROCK:
			mActors.push_back( new Unit_Rock(mActors[ESpawnType_AI]->pos, 100) );
			mActors.back()->Team = ESpawnType_AI;
			mActors.back()->SetDestination( getBuildingInfo(ESpawnType_Player) );
			break;
		case ESpawnType_AI_PAPER:
			mActors.push_back( new Unit_Paper(mActors[ESpawnType_AI]->pos, 100) );
			mActors.back()->Team = ESpawnType_AI;
			mActors.back()->SetDestination( getBuildingInfo(ESpawnType_Player) );
			break;
		case ESpawnType_AI_SCISSORS:
			mActors.push_back( new Unit_Scissors(mActors[ESpawnType_AI]->pos, 100) );
			mActors.back()->Team = ESpawnType_AI;
			mActors.back()->SetDestination( getBuildingInfo(ESpawnType_Player) );
			break;
		case ESpawnType_PLAYER_ROCK:
			mActors.push_back( new Unit_Rock(mActors[ESpawnType_Player]->pos, 100) );
			mActors.back()->Team = ESpawnType_Player;
			mActors.back()->SetDestination( getBuildingInfo(ESpawnType_AI) );
			break;
		case ESpawnType_PLAYER_PAPER:
			mActors.push_back( new Unit_Paper(mActors[ESpawnType_Player]->pos, 100) );
			mActors.back()->Team = ESpawnType_Player;
			mActors.back()->SetDestination( getBuildingInfo(ESpawnType_AI) );
			break;
		case ESpawnType_PLAYER_SCISSORS:
			mActors.push_back( new Unit_Scissors(mActors[ESpawnType_Player]->pos, 100) );
			mActors.back()->Team = ESpawnType_Player;
			mActors.back()->SetDestination( getBuildingInfo(ESpawnType_AI) );
			break;
		default:
			break; 
	}
}