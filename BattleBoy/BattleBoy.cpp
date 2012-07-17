#include "BattleBoy.h"

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
	/*mShipImage = rm->getImage("IMAGE_SHIP");
	mThrustImage = rm->getImage("IMAGE_THRUST");
	mFont = rm->getFont("FONT_MAIN");
	mBoomSound = rm->getSound("SOUND_BOOM");
	mFireSound = rm->getSound("SOUND_FIRE");
	mThrustSound = rm->getSound("SOUND_THRUST");*/
}

void BattleBoy::update(float dt)
{
}

void BattleBoy::draw(Boy::Graphics *g)
{
}

void BattleBoy::keyUp(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods)
{
}

void BattleBoy::keyDown(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods)
{
}