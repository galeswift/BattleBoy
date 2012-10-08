#include "Globals.h"

void Actor::update(float dt )
{
	accel.truncate(getMaxAccel());

	vel += accel * dt;
	vel.truncate(getMaxSpeed());
	pos += vel * dt;
}

void Actor::init()
{
	size = 50;
	setDestroyed(false);
	game = NULL;
}