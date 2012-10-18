#include "Globals.h"

void Controller::init()
{
	Actor::init();
	setShouldErase(false);
}

void Controller_AI::init()
{
	Controller::init();

	mStateManager = new StateManager(this);
	mStateManager->pushState(new State_Spawning(this));
}

void Controller_AI::update(float dt)
{ 
	if( mStateManager )
	{
		mStateManager->update(dt);
	}

	Actor::update(dt);
}