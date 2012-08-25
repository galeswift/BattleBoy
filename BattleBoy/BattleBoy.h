#pragma once

enum ESpawnType
{
	ESpawnType_NONE = 0,
	ESpawnType_AI_ROCK = 1,
	ESpawnType_AI_PAPER = 2,
	ESpawnType_AI_SCISSORS = 3,
	ESpawnType_PLAYER_ROCK = 4,
	ESpawnType_PLAYER_PAPER = 5,
	ESpawnType_PLAYER_SCISSORS = 6
};

class BattleBoy: public Boy::Game, public Boy::KeyboardListener
{
public:

	BattleBoy();
	virtual ~BattleBoy();

	// singleton:
	static BattleBoy *instance();
	static void destroy();

	// implementation of Game:
	virtual void init(int argc, char* argv[]);
	virtual void parseCommandArgs(int argc, char* argv[]);
	virtual void load();
	virtual void loadComplete();
	virtual void update(float dt);
	virtual void draw(Boy::Graphics *g);
	virtual void drawResources(Boy::Graphics *g);
	virtual void drawDebugText(Boy::Graphics *g);

	// implementation of KeyboardListener:
	virtual void keyUp(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods);
	virtual void keyDown(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods);

	virtual void KillAllUnitsCheat();
	virtual void SetAutoplay(bool bOn);
	virtual void Restart();

	virtual void SpawnUnit(ESpawnType Unit);

	BoyLib::Vector2 getBuildingInfo(int whichPlayer);
private:
	Boy::Font *mFont;
	bool mLoadComplete;
	ESpawnType mPendingSpawnType;

	std::map<wchar_t,ESpawnType> mKeyToSpawnType;
private:
	// Static instance of the battleboy class
	static BattleBoy *gInstance;

	// The main board actor
	Board *mBoard;

	// All Actors in game
	std::vector<Actor*> mActors;

	// If we are a server/client, this will hold the network interface for it
	Networking::NetworkInterface *mNetInterface;

	// The networking role of this game type (Authority for server, simulated proxy for clients
	Networking::ENetworkingRole mRole;

	// The target ip address that the client is trying to connect to.  Parsed from the --client command line argument
	std::string mPendingAddress;

	// HACK Refactor into Players and AI
	int PlayerCredits;
	int AICredits;

	// HACK to play AI versus AI only
	bool bAutoPlay;
};
