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
	setShouldErase(true);
	game = NULL;
}

void Projectile::init()
{
	Actor::init();
	vel = mDir.normalize() * getMaxSpeed();
	size = 5;
}

void Projectile::draw(Boy::Graphics* g)
{
	Actor::draw(g);

	g->pushTransform();
	g->drawCircle(pos.x, pos.y, 5, 5);
	g->popTransform();	
}

void Projectile::update(float dt)
{
	BattleBoy* game = BattleBoy::instance();
		
	Actor::update(dt);
	
	if( game->isOutOfBounds(this) )
	{
		setDestroyed(true);
	}
}