#pragma once

class State;

class StateManager
{
public:
	StateManager(Actor* owner) {mOwner=owner;}
	Actor* getOwner();
	virtual void pushState( State* state );
	virtual void popState();
	virtual void update(float dt);
	virtual void draw(Boy::Graphics *g);
	std::string getCurrentStateName() const;
	
private:
	std::vector<State*> mStack;
	Actor* mOwner;
};

class State
{
public:
	State(Actor* owner) {mOwner=owner;}
	virtual void begin() = 0;
	virtual void update(float dt) = 0;
	virtual void end() = 0;
	virtual void draw(Boy::Graphics* g) = 0;
	Actor* getOwner();
	Unit* getUnitOwner();
	void setStateManager( StateManager* stateManager ) { mStateManager = stateManager; }
	StateManager* getStateManager() { return mStateManager; }
protected:
	Actor* mOwner;
	StateManager* mStateManager;
};

class State_Idle : public State
{
public:
	State_Idle(Actor *owner) : State(owner){};
	virtual void begin(){};
	virtual void update(float dt) {};
	virtual void end() {};
	virtual void draw(Boy::Graphics* g) {};
};

class State_Moving : public State
{
public:
	State_Moving(Actor *owner, BoyLib::Vector2 target) : State(owner), mFinalDest(target){};
	virtual void begin();
	virtual void update(float dt);
	virtual void end();
	virtual void draw(Boy::Graphics* g);
	virtual void setNextWaypoint(bool bInit = false);
private:
	BoyLib::Vector2 mFinalDest;
	BoyLib::Vector2 mCurrentDest;
	std::list<BoyLib::Vector2> mPath;
};

class State_Attacking : public State
{
public:
	State_Attacking(Actor *owner, Unit* enemy=NULL) : State(owner), mCurrentTarget(enemy){};
	virtual void begin();
	virtual void update(float dt);
	virtual void end() {};
	virtual void draw(Boy::Graphics* g);
private:
	Unit* mCurrentTarget;
	float mAttackDelay;
};

class State_Spawning : public State
{
public:
	State_Spawning(Actor *owner) : State(owner){};
	virtual void begin();
	virtual void update(float dt);
	virtual void end() {};
	virtual void draw(Boy::Graphics* g) {};
private:
	float mSpawnDelay;
};
