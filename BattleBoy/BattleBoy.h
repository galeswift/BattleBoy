#pragma once

enum ESpawnType
{
	ESpawnType_NONE = 0,
	ESpawnType_ROCK = 1,
	ESpawnType_PAPER = 2,
	ESpawnType_SCISSORS = 3,
	ESpawnType_MAX = 3, // DON'T INCLUDE ESpawnType_NONE
};

enum EGameType
{
	EGameType_Versus = 0,
	EGameType_Coop = 1,
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
	
	// Server/client support
	virtual void initNetworking();

	// Cheats and spawning
	virtual void initKeyBindings();

	// Load our pathfinding map
	virtual void initMapData();

	// Crazy Eddie's gui
	virtual void initGui();
	// Pause/unpause the game
	virtual void togglePause();
	// Cycle through the various debug draw modes
	virtual void toggleDebugDraw();
	// Return the current debug draw mode
	virtual int getDebugDrawMode() { return mDebugDrawMode; }
	// Reset the game
	virtual void reset();

	// Parse command line
	virtual void parseCommandArgs(int argc, char* argv[]);
	virtual void load();
	virtual void loadComplete();
	virtual void update(float dt);
	virtual void draw(Boy::Graphics *g);
	virtual void addActor(Actor* a);

	// return true if this actor is outside the bounds of the window
	virtual bool isOutOfBounds(Actor* a);

	// implementation of KeyboardListener:
	virtual void keyUp(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods);
	virtual void keyDown(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods);
	virtual std::vector<Actor*>& getActors() {return mActors;};

	virtual void spawnUnit(ESpawnType unitType, int teamIdx);
	const BoyLib::Vector2 getSpawnPos( int playerIdx );

	Unit* closestEnemy(Unit* unit, float range = -1);

	virtual EGameType getGameType() { return mGameType; }
	virtual void setGameType(EGameType gameType) { mGameType = gameType; }

private:
	Boy::Font *mFont;
	bool mLoadComplete;
	BattleGui* mGui;
	BattleMap* mMap;

	typedef void (BattleBoy::*keyCommand)(void);
	std::map<wchar_t,keyCommand> mKeyToCommand;
	std::map<wchar_t,SpawnInfo> mKeyToSpawnInfo;

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

	// What kind of game it is, coop or pvp
	EGameType mGameType;
};
