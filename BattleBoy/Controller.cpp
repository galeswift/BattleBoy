#include "Globals.h"

void Controller_AI::init()
{
	Controller::init();

	mStateManager = new StateManager(this);
}