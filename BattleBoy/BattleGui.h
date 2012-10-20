#pragma once

class BattleBoy;

class BattleGui : public Boy::KeyboardListener, public Boy::MouseListener
{
public:
	BattleGui(BattleBoy* owner) {mOwner = owner;}
	void init();
	void update(float dt);
	
	// implementation of KeyboardListener:
	virtual void keyUp(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods);
	virtual void keyDown(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods);
	
	// Implementation of mouseListener
	virtual void mouseMove(Boy::Mouse *mouse);
	virtual void mouseButtonDown(Boy::Mouse *mouse, Boy::Mouse::Button button, int clickCount);
	virtual void mouseButtonUp(Boy::Mouse *mouse, Boy::Mouse::Button button);
	virtual void mouseWheel(Boy::Mouse *mouse, int wheelDelta);
	virtual void mouseEnter(Boy::Mouse *mouse);
	virtual void mouseLeave(Boy::Mouse *mouse);

private:
	BattleBoy* mOwner;
};