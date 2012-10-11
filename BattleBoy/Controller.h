#pragma once

class Controller : public Actor
{
public:
	Controller() {};
	virtual void init();
	void setTeamIdx(int idx){ teamIdx = idx; }
	int getTeamIdx(){ return teamIdx; }
protected:	
	int teamIdx;
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
	void update(float dt);
	void init();
protected:	
	StateManager* mStateManager;
};

