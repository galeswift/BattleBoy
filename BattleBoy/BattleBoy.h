#pragma once

enum ESpawnType
{
	ESpawnType_NONE = 0,
	ESpawnType_ROCK = 1,
	ESpawnType_PAPER = 2,
	ESpawnType_SCISSORS = 3
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

	// implementation of KeyboardListener:
	virtual void keyUp(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods);
	virtual void keyDown(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods);
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

	// The units that are currently active
	std::vector<Unit*> mUnits;

	// All buildings in this game
	std::vector<Building*> mBuildings;

	// All SpawnPoints in this game
	std::vector<SpawnPoint*> mPlayerSpawnPoints;

	// If we are a server/client, this will hold the network interface for it
	Networking::NetworkInterface *mNetInterface;

	// The networking role of this game type (Authority for server, simulated proxy for clients
	Networking::ENetworkingRole mRole;

	// The target ip address that the client is trying to connect to.  Parsed from the --client command line argument
	std::string mPendingAddress;	
};
