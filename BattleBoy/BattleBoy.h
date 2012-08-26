#pragma once

enum ESpawnType
{
	ESpawnType_NONE = 0,
	ESpawnType_ROCK = 1,
	ESpawnType_PAPER = 2,
	ESpawnType_SCISSORS = 3,
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

	virtual void killAllUnitsCheat();
	virtual void setAutoplay(bool bOn);
	virtual void restart();
	virtual std::vector<Actor*>& getActors() {return mActors;};
	
	virtual void spawnUnit(ESpawnType unitType, int teamIdx);
	const BoyLib::Vector2 getSpawnPos( int playerIdx );
private:
	Boy::Font *mFont;
	bool mLoadComplete;

	std::map<wchar_t,SpawnInfo> mKeyToSpawnInfo;
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
