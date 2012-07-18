#pragma once


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
	Boy::Font *mFont;
	bool mLoadComplete;
	Boy::UString mPendingSpawnType;

	std::map<wchar_t,Boy::UString> mKeyToSpawnType;
private:
	static BattleBoy *gInstance;
	Board *mBoard;
	std::vector<Unit*> mUnits;
};
