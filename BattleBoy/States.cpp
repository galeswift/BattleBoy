#include "Globals.h"


void StateManager::pushState( State* state )
{
	if( !mStack.empty() )
	{
		mStack.back()->end();
	}

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
	}
}

void State_Moving::begin()
{
	mOwner->getSteering()->arriveOn();
	mOwner->getSteering()->separationOn();
	mDest = mOwner->getSteering()->target();
}

void State_Moving::end()
{
	mOwner->getSteering()->arriveOff();
}

void State_Moving::update(float dt)
{
	BattleBoy* game = BattleBoy::instance();

	Unit* enemy = game->closestEnemy(mOwner, mOwner->getRange());
	if( enemy != NULL )
	{
		mOwner->getStateManager()->pushState( new State_Attacking( mOwner ) );
	}
}

void State_Attacking::begin()
{
	mAttackDelay = mOwner->getAttackRate();
	BattleBoy* game = BattleBoy::instance();
	
	Unit* enemy = game->closestEnemy(mOwner, mOwner->getRange());
	if( enemy != NULL )
	{
		mCurrentTarget = enemy;
	}
}

void State_Attacking::draw(Boy::Graphics* g)
{
	if( mCurrentTarget != NULL)
	{
		g->pushTransform();
			
		g->translate(mOwner->getPos().x,mOwner->getPos().y);
		g->drawLine(mOwner->getPos().x,mOwner->getPos().y, mCurrentTarget->getPos().x, mCurrentTarget->getPos().y);
		g->popTransform();
	}
}

void State_Attacking::update(float dt)
{
	if( mCurrentTarget )
	{
		mAttackDelay -= dt;
		if( mAttackDelay <= 0 )
		{
			mAttackDelay = mOwner->getAttackRate();
			mCurrentTarget->takeDamage(mOwner);
		}

		if( mCurrentTarget->isDestroyed() )
		{
			mCurrentTarget = NULL;
			BattleBoy* game = BattleBoy::instance();
			Unit* enemy = game->closestEnemy(mOwner, mOwner->getRange());
			if( enemy != NULL )
			{
				mCurrentTarget = enemy;
			}
		}
	}
	else
	{
		mOwner->getStateManager()->popState();
	}
}
