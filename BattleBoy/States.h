#pragma once

class owner;

class State
{
public:
	State(Unit* owner) {mOwner=owner;}
	virtual void begin() = 0;
	virtual void update(float dt) = 0;
	virtual void end() = 0;
	virtual void draw(Boy::Graphics* g) = 0;

protected:
	Unit* mOwner;
};

class StateManager
{
public:
	StateManager(Unit* owner) {mOwner=owner;}
	virtual void pushState( State* state );
	virtual void popState();
	virtual void update(float dt);
	virtual void draw(Boy::Graphics *g);
	std::string getCurrentStateName() const;
	
private:
	std::vector<State*> mStack;
	Unit* mOwner;
};

class State_Idle : public State
{
public:
	State_Idle(Unit *owner) : State(owner){};
	virtual void begin(){};
	virtual void update(float dt) {};
	virtual void end() {};
	virtual void draw(Boy::Graphics* g) {};
};

class State_Moving : public State
{
public:
	State_Moving(Unit *owner) : State(owner){};
	virtual void begin();
	virtual void update(float dt);
	virtual void end();
	virtual void draw(Boy::Graphics* g);
private:
	BoyLib::Vector2 mDest;
};

class State_Attacking : public State
{
public:
	State_Attacking(Unit *owner, Unit* enemy=NULL) : State(owner), mCurrentTarget(enemy){};
	virtual void begin();
	virtual void update(float dt);
	virtual void end() {};
	virtual void draw(Boy::Graphics* g);
private:
	Unit* mCurrentTarget;
	float mAttackDelay;
};
