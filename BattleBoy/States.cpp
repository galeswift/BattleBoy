#include "Globals.h"

Actor* State::getOwner() 
{
	return mOwner;
}

Unit* State::getUnitOwner() 
{
	return dynamic_cast<Unit*>( getOwner() );
}

Actor* StateManager::getOwner() 
{
	return mOwner;
}

void StateManager::pushState( State* state )
{
	if( !mStack.empty() )
	{
		mStack.back()->end();
	}

	state->setStateManager(this);
	state->begin();
	mStack.push_back(state);
}

void StateManager::popState()
{
	if( !mStack.empty() )
	{
		mStack.back()->end();
		mStack.pop_back();
	}

	if( !mStack.empty() )
	{
		mStack.back()->begin();
	}
}

void StateManager::update(float dt)
{
	if( !mStack.empty() )
	{
		mStack.back()->update(dt);
	}
}

void StateManager::draw(Boy::Graphics* g)
{
	if( !mStack.empty() )
	{
		mStack.back()->draw(g);

		BattleBoy* game = BattleBoy::instance();
		if( game->getDebugDrawMode() == EDebugDrawMode_STATES )
		{
			g->pushTransform();
			g->translate(getOwner()->getPos().x,getOwner()->getPos().y);
			g->translate(0,-50);
			g->drawString(getCurrentStateName(), 24);
			g->popTransform();
		}
	}
}

std::string StateManager::getCurrentStateName() const
 {
	std::string result = "none";
	
	if( !mStack.empty() )
	{
		result = (typeid(*mStack.back()).name());

		//remove the 'class ' part from the front of the string
		if (result.size() > 5)
		{
			result.erase(0, 6);
		}
	}

	return result;
}

void State_Moving::begin()
{
	getUnitOwner()->getSteering()->seekOn();
	getUnitOwner()->getSteering()->separationOn();
	mPath = BattleBoy::getMap()->getPath(getUnitOwner()->getPos(), mFinalDest);
	setNextWaypoint(true);
}

// if bInit is true, don't remove the first path 
void State_Moving::setNextWaypoint(bool bInit)
{
	if( mPath.size() > 1 )
	{
		if( !bInit )
		{
			mPath.erase(mPath.begin());
		}

		mCurrentDest = *mPath.begin();
		getUnitOwner()->getSteering()->setTarget(mCurrentDest);
	}
	else
	{
		getStateManager()->popState();
	}
}

void State_Moving::end()
{
	getUnitOwner()->getSteering()->seekOff();
	getUnitOwner()->getSteering()->separationOff();
}

void State_Moving::draw(Boy::Graphics* g)
{
	BattleBoy* game = BattleBoy::instance();

	if( game->getDebugDrawMode() == EDebugDrawMode_STATES )
	{
		for(std::list<BoyLib::Vector2>::iterator it = mPath.begin() ; it != mPath.end() ; it++ )
		{	
			g->setColorizationEnabled(true);
			g->pushTransform();
			g->fillRect((*it).x, (*it).y, 5, 5);
			g->popTransform();
		}

		g->pushTransform();
		g->setColorizationEnabled(false);
		g->translate(getOwner()->getPos().x, getOwner()->getPos().y);
		g->popTransform();

		g->pushTransform();
		
		if (getUnitOwner()->getTeamIdx() == 0)
		{
			g->setColor(0xff0fffff);
		}
		else
		{
			g->setColor(0xfffff000);
		}
	
		g->setColorizationEnabled(true);
		g->drawLine(getOwner()->getPos().x, getOwner()->getPos().y,  getOwner()->getPos().x + getUnitOwner()->getSteering()->force().x, getOwner()->getPos().y + getUnitOwner()->getSteering()->force().y);
		g->fillRect(mCurrentDest.x, mCurrentDest.y, 5,5);
		g->setColorizationEnabled(false);
		g->popTransform();
	}
}

void State_Moving::update(float dt)
{
	BattleBoy* game = BattleBoy::instance();
	Unit* enemy = game->closestEnemy(getUnitOwner(), getUnitOwner()->getRange());
	
	if( enemy != NULL )
	{
		getUnitOwner()->getStateManager()->pushState( new State_Attacking( getOwner(), enemy ) );
	}
	else 
	{
		BoyLib::Vector2 to = getUnitOwner()->getPos() - mCurrentDest;

		if (to.magnitudeSquared() < 20.0f * 20.0f)
		{
			setNextWaypoint();
		}
	}
}

void State_Attacking::begin()
{
	mAttackDelay = 0;
}

void State_Attacking::draw(Boy::Graphics* g)
{
	if( mCurrentTarget != NULL)
	{
		BattleBoy* game = BattleBoy::instance();
		if( game->getDebugDrawMode() == EDebugDrawMode_STATES )
		{
			g->pushTransform();	
			g->drawLine(getOwner()->getPos().x,getOwner()->getPos().y, mCurrentTarget->getPos().x, mCurrentTarget->getPos().y);
			g->popTransform();
		}
	}
}

void State_Attacking::update(float dt)
{
	BattleBoy* game = BattleBoy::instance();

	if( !mCurrentTarget || mCurrentTarget->isDestroyed() )
	{
		mCurrentTarget = NULL;
		Unit* enemy = game->closestEnemy(getUnitOwner(), getUnitOwner()->getRange());
		if( enemy != NULL )
		{
			mCurrentTarget = enemy;
		}
	}

	if( mCurrentTarget )
	{
		mAttackDelay -= dt;
		if( mAttackDelay <= 0 )
		{
			mAttackDelay = getUnitOwner()->getAttackRate();
			game->addActor(new Projectile( getUnitOwner(), getOwner()->getPos(),  mCurrentTarget->getPos() - getOwner()->getPos() ));
		}
	}
	else
	{
		getUnitOwner()->getStateManager()->popState();
	}
}

void State_Spawning::begin()
{
	mSpawnDelay = 0;
}

void State_Spawning::update(float dt)
{
	BattleBoy* game = BattleBoy::instance();

	mSpawnDelay -= dt;
	if( mSpawnDelay <= 0 )
	{
		// temp spawn rate until resources are put in
		mSpawnDelay = 2.0f + randf(-1.0, 1.0);
		game->spawnUnit(ESpawnType(randi(ESpawnType_ROCK, ESpawnType_SCISSORS)), dynamic_cast<Controller_AI*>(getOwner())->getTeamIdx());
	}
}

