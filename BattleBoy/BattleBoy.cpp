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

	mKeyToSpawnType['q'] = ESpawnType_ROCK;
	mKeyToSpawnType['w'] = ESpawnType_PAPER;
	mKeyToSpawnType['e'] = ESpawnType_SCISSORS;
	
	int w = Boy::Environment::screenWidth();
	int h = Boy::Environment::screenHeight();
	mBuildings.push_back( new Building(BoyLib::Vector2(w/2.0f,float(h-100))) );
	mBuildings.push_back( new Building(BoyLib::Vector2(w/2.0f,100.0)) );
	mPlayerSpawnPoints.push_back( new SpawnPoint(BoyLib::Vector2(w/2.0f,float(h-150))) );
	mPlayerSpawnPoints.push_back( new SpawnPoint(BoyLib::Vector2(w/2.0f,150.0)) );
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
	for( std::vector<Unit*>::iterator it = mUnits.begin(); it != mUnits.end() ; ++it )
	{
		if ((*it)->bDead)
		{
			mUnits.erase(it);
			it--;
		}
		else
		{
			(*it)->update(dt, mUnits);
		}
	}
}

void BattleBoy::draw(Boy::Graphics *g)
{
	if( mLoadComplete ) 
	{
		mBoard->draw(g);

		for( std::vector<Unit*>::iterator it = mUnits.begin(); it != mUnits.end() ; ++it )
		{
			(*it)->draw(g);
		}

		for( std::vector<Building*>::iterator it = mBuildings.begin(); it != mBuildings.end() ; ++it )
		{
			(*it)->draw(g);
		}

		// draw status
		g->setColorizationEnabled(true);
		g->setColor(0xffffffff);
		g->pushTransform();
		g->translate(50,50);
		//mFont->drawString(g,mPendingSpawnType,0.5f);
		g->popTransform();
		g->setColorizationEnabled(false);

	}
}

void BattleBoy::keyUp(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods)
{
	int w = Boy::Environment::screenWidth();
	int h = Boy::Environment::screenHeight();
	
	// TODO: make real player enum
	int player = rand() % 2;

	switch (mPendingSpawnType)
	{
		case ESpawnType_ROCK:
			mUnits.push_back( new Unit_Rock(mPlayerSpawnPoints[player]->pos, 100) );
			break;
		case ESpawnType_PAPER:
			mUnits.push_back( new Unit_Paper(mPlayerSpawnPoints[player]->pos, 100) );
			break;
		case ESpawnType_SCISSORS:
			mUnits.push_back( new Unit_Scissors(mPlayerSpawnPoints[player]->pos, 100) );
			break;
		default:
			mUnits.push_back( new Unit(mPlayerSpawnPoints[player]->pos, 100) );
	}

	mUnits.back()->Team = EUnitTeam(player);
	mUnits.back()->Range = 50.0f;
	mUnits.back()->Damage = 2.0f;
	mUnits.back()->Health = 500.0f;
	mUnits.back()->SetDestination( mBuildings[abs(player - 1)]->pos );
	mPendingSpawnType = ESpawnType_NONE;
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