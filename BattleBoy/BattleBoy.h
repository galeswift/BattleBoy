#pragma once

#include "Boy/Environment.h"
#include "Boy/Font.h"
#include "Boy/Game.h"
#include "Boy/Graphics.h"
#include "Boy/KeyboardListener.h"
#include "BoyLib/BoyUtil.h"
#include "BoyLib/Vector2.h"

class BattleBoy: public Boy::Game, public Boy::KeyboardListener
{
public:

	BattleBoy();
	virtual ~BattleBoy();

	// singleton:
	static BattleBoy *instance();
	static void destroy();

	// implementation of Game:
	virtual void init();
	virtual void load();
	virtual void loadComplete();
	virtual void update(float dt);
	virtual void draw(Boy::Graphics *g);

	// implementation of KeyboardListener:
	virtual void keyUp(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods);
	virtual void keyDown(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods);

private:
	static BattleBoy *gInstance;
};
