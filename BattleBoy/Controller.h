#pragma once

class Controller : public Actor
{
public:
	Controller() {};
	virtual void init() {};
};

class Controller_Player : public Controller
{
public:
	Controller_Player() : Controller() {};
};

class Controller_AI : public Controller
{
public:
	Controller_AI() : Controller() {};
	StateManager* getStateManager() { return mStateManager; }
	void init();
protected:	
	StateManager* mStateManager;
};

