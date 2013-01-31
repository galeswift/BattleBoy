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
	size = 20;
	setDestroyed(false);
	setShouldErase(true);
	game = NULL;
}

bool Actor::collidedWith(Actor* other)
{
	return dist(getPos().x,getPos().y,other->getPos().x,other->getPos().y) < getSize()+other->getSize();
}

void Actor::draw(Boy::Graphics* g)
{
	if( game && game->getDebugDrawMode() == EDebugDrawMode_STATES )
	{
		g->pushTransform();
		g->drawCircle(pos.x, pos.y, getSize(), 5);
		g->popTransform();
	}
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
	else
	{
		// check for collisions
		for( std::vector<Actor*>::iterator it = game->getActors().begin(); it != game->getActors().end(); it++ )
		{
			if( (*it)->collidedWith(this) )
			{
				Unit* otherUnit = dynamic_cast<Unit*>(*it);
				if( otherUnit && otherUnit->getTeamIdx() != mInstigator->getTeamIdx() )
				{
					game->addActor(new Explosion(getPos().x, getPos().y, 2, 5, otherUnit->getTeamIdx() == 0 ? 0xff00ffff : 0xffff0000));
					otherUnit->takeDamage(mInstigator);
					setDestroyed(true);
					break;
				}
			}
		}
	}
}
