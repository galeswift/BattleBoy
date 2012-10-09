#pragma once

enum ESpawnType
{
	ESpawnType_NONE = 0,
	ESpawnType_ROCK = 1,
	ESpawnType_PAPER = 2,
	ESpawnType_SCISSORS = 3,
	ESpawnType_MAX = 3, // DON'T INCLUDE ESpawnType_NONE
};

enum EDrawMode
{
	EDebugDrawMode_NONE = 0,
	EDebugDrawMode_ALL = 1,
	EDebugDrawMode_MAX = 2,
};

struct SpawnInfo
{
	SpawnInfo() : type(ESpawnType_NONE), playerIdx(-1){};
	SpawnInfo( ESpawnType _type, int _playerIdx ) : type(_type), playerIdx(_playerIdx) {};
	ESpawnType type;
	int playerIdx;
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
	// Pause/unpause the game
	virtual void togglePause();
	// Cycle through the various debug draw modes
	virtual void toggleDebugDraw();
	// Return the current debug draw mode
	virtual int getDebugDrawMode() { return mDebugDrawMode; }

	// Parse command line
	virtual void parseCommandArgs(int argc, char* argv[]);
	virtual void load();
	virtual void loadComplete();
	virtual void update(float dt);
	virtual void draw(Boy::Graphics *g);
	virtual void addActor(Actor* a);

	// implementation of KeyboardListener:
	virtual void keyUp(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods);
	virtual void keyDown(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods);
	virtual std::vector<Actor*>& getActors() {return mActors;};
	
	virtual void spawnUnit(ESpawnType unitType, int teamIdx);
	const BoyLib::Vector2 getSpawnPos( int playerIdx );

	Unit* closestEnemy(Unit* unit, float range = -1);

private:
	Boy::Font *mFont;
	bool mLoadComplete;

	typedef void (BattleBoy::*keyCommand)(void);
	std::map<wchar_t,keyCommand> mKeyToCommand;
	std::map<wchar_t,SpawnInfo> mKeyToSpawnInfo;
private:
	// Static instance of the battleboy class
	static BattleBoy *gInstance;

	// Whether the game is paused or not
	bool bPaused;

	// Debug draw mode
	int mDebugDrawMode;

	// All Actors in game
	std::vector<Actor*> mActors;

	// If we are a server/client, this will hold the network interface for it
	Networking::NetworkInterface *mNetInterface;

	// The networking role of this game type (Authority for server, simulated proxy for clients
	Networking::ENetworkingRole mRole;

	// The target ip address that the client is trying to connect to.  Parsed from the --client command line argument
	std::string mPendingAddress;

	// What spawnInfo to spawn next
	SpawnInfo mPendingSpawnInfo;
};
