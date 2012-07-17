#include "BattleBoy.h"

#include "Board.h"
#include <assert.h>
#include "Boy/Environment.h"
#include "Boy/GamePad.h"
#include "Boy/Graphics.h"
#include "Boy/Mouse.h"
#include "Boy/ResourceManager.h"
#include "Boy/SoundPlayer.h"
#include <string>

BattleBoy *BattleBoy::gInstance = NULL;

BattleBoy::BattleBoy()
{
	mBoard = NULL;
	mLoadComplete = false;
	mPendingSpawnType = "NONE";
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

void BattleBoy::init()
{
	mBoard = new Board();
	mKeyToSpawnType['q'] = "Spawn Melee";
	mKeyToSpawnType['w'] = "Spawn Ranged";
	mKeyToSpawnType['e'] = "Spawn Flyer";
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
}

void BattleBoy::draw(Boy::Graphics *g)
{
	if( mLoadComplete ) 
	{
		// draw status
		g->setColorizationEnabled(true);
		g->setColor(0xffffffff);
		g->pushTransform();
		g->translate(50,50);
		mFont->drawString(g,mPendingSpawnType,0.5f);
		g->popTransform();
		g->setColorizationEnabled(false);


	}
}

void BattleBoy::keyUp(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods)
{
	mPendingSpawnType = "NONE";
}

void BattleBoy::keyDown(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods)
{
	// Look through our keybindings and see if we have a result
	std::map<wchar_t,Boy::UString>::iterator keyBinding = mKeyToSpawnType.find(unicode);
	if( keyBinding != mKeyToSpawnType.end() )
	{
		mPendingSpawnType = (*keyBinding).second;
	}
}