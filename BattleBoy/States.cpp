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
		if( game->getDebugDrawMode() == EDebugDrawMode_ALL )
		{
			g->pushTransform();
			g->translate(getOwner()->getPos().x,getOwner()->getPos().y);
			char stateText[100] = {};
			Boy::ResourceManager *rm = Boy::Environment::instance()->getResourceManager();
			Boy::Font *mFont = rm->getFont("FONT_MAIN");
			g->translate(0,-50);
			sprintf_s(stateText,"%s", getCurrentStateName().c_str());
			mFont->drawString(g, stateText, 0.15f);
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
	getUnitOwner()->getSteering()->arriveOn();
	getUnitOwner()->getSteering()->separationOn();
	mDest = getUnitOwner()->getSteering()->target();
}

void State_Moving::end()
{
	getUnitOwner()->getSteering()->arriveOff();
	getUnitOwner()->getSteering()->separationOff();
}

void State_Moving::draw(Boy::Graphics* g)
{
	BattleBoy* game = BattleBoy::instance();

	if( game->getDebugDrawMode() == EDebugDrawMode_ALL )
	{
		g->pushTransform();
		g->setColorizationEnabled(true);

		if (getUnitOwner()->getTeamIdx() == 0)
		{
			g->setColor(0xff0fffff);
		}
		else
		{
			g->setColor(0xfffff000);
		}
		
		g->drawLine(getOwner()->getPos().x, getOwner()->getPos().y,  getOwner()->getPos().x + getUnitOwner()->getSteering()->force().x, getOwner()->getPos().y + getUnitOwner()->getSteering()->force().y);
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
}

void State_Attacking::begin()
{
	mAttackDelay = getUnitOwner()->getAttackRate();
}

void State_Attacking::draw(Boy::Graphics* g)
{
	if( mCurrentTarget != NULL)
	{
		BattleBoy* game = BattleBoy::instance();
		if( game->getDebugDrawMode() == EDebugDrawMode_ALL )
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
			mCurrentTarget->takeDamage(getUnitOwner());
			game->addActor(new Projectile( getOwner()->getPos(),  mCurrentTarget->getPos() - getOwner()->getPos() ));
		}
	}
	else
	{
		getUnitOwner()->getStateManager()->popState();
	}
}
