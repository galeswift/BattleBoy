#include "Globals.h"

void Unit::init()
{
	Actor::init();

	mStateManager = new StateManager(this);
	steering = new Steering(this);
	damage = 10.0f;
	range = 100.0f;
	maxHealth = 50.0f;
	health = maxHealth;
	attackRate = 0.5f;
	mImage = NULL;
}

void Unit::update(float dt)
{
	if( steering )
	{
		steering->update(dt);
	}

	//if no steering force is produced decelerate the player by applying a
	//braking force
	if (steering->force().magnitude() <= 0.0f)
	{
		const float brakingRate = 0.0f; 

		setVel( getVel() * brakingRate );                                     
	}

 
	if( mStateManager )
	{
		mStateManager->update(dt);
	}

	Actor::update(dt);
}

void Unit::takeDamage(Unit* attacker)
{
	float damage = calcDamage( attacker );
	health -= damage;
	if (health <= 0)
	{
		health = 0;
		setDestroyed(true);
	}
}

float Unit::calcDamage(Unit* attacker)
{
	float modifier = 1.0f;
	for(unsigned int d = 0; d < attacker->getDamageTypes().size(); d++)
	{
		for(unsigned int v = 0; v < vulnerabilityTypes.size(); v++)
		{
			if (attacker->getDamageTypes()[d] == vulnerabilityTypes[v])
			{
				modifier += 0.5f;
			}
		}
		for(unsigned int r = 0; r < resistanceTypes.size(); r++)
		{
			if (attacker->getDamageTypes()[d] == resistanceTypes[r])
			{
				modifier -= 0.5f;
			}
		}
	}
	modifier = std::max<float>(0.0f, modifier);
	return (attacker->getDamage()*modifier);
}

void Unit::draw(Boy::Graphics *g)
{
	Actor::draw(g);

	if( mStateManager )
	{
		mStateManager->draw(g);
	}

	drawHealth(g);

	g->setColorizationEnabled(true);

	if (getTeamIdx() == 0)
	{
		g->setColor(0xff00ffff);
	}
	else
	{
		g->setColor(0xffff0000);
	}
	
	if (mImage != NULL)
	{
		g->pushTransform();
			// TODO Rotation
			g->rotateDeg((teamIdx - 1)*180.0f);
			g->translate(pos.x,pos.y);
			if (getTeamIdx() == 0)
			{
				g->drawImage(Boy::Environment::getImage("IMAGE_BLUETEAM"));
			}
			else
			{
				g->drawImage(Boy::Environment::getImage("IMAGE_REDTEAM"));
			}
			g->drawCircle((int)pos.x,(int)pos.y,getRange(), 8);
			g->drawImage(mImage);
		g->popTransform();
	}
}

void Unit::drawHealth(Boy::Graphics *g)
{
	// draw status
	g->setColorizationEnabled(true);
	g->setColor(0xff00ff00);
	float healthPercentage = health/maxHealth;
	if (healthPercentage < 0.25f)
	{
		g->setColor(0xffff0000);
	}
	else if (healthPercentage < 0.50)
	{
		g->setColor(0xffff8c00);
	}
	Boy::ResourceManager *rm = Boy::Environment::instance()->getResourceManager();
	Boy::Font *mFont = rm->getFont("FONT_MAIN");

	char healthText[100];
	int h1 = int(health);            // Get the integer part
	float f1 = health - h1;     // Get fractional part
	int h2 = int(f1 * 100);   // Turn into integer
	int mh1 = int(maxHealth);            // Get the integer part
	float f2 = maxHealth - mh1;     // Get fractional part
	int mh2 = int(f2 * 100);   // Turn into integer

	//sprintf_s(healthText, "%d.%04d/%d.%04d", h1, h2, mh1, mh2);
	sprintf_s(healthText, "%dl%d", h1, mh1);
	
	g->pushTransform();
	g->translate(pos.x - size,pos.y - size/2.0f);
	mFont->drawString(g,healthText,0.25f);
	g->popTransform();

	g->setColorizationEnabled(false);
}

void Unit_Building::draw(Boy::Graphics *g)
{	
	Unit::draw(g);

	int x = int(pos.x);
	int y = int(pos.y);
	
	g->pushTransform();
	g->drawCircle(x,y,size/2.0f,12);	
	g->popTransform();	
	
	if (isDestroyed())
	{
		g->pushTransform();
		g->setColorizationEnabled(true);
		g->setColor(0xffff0000);
		g->drawLine(pos.x - size/2.0f,	pos.y - size/2.0f,	pos.x + size/2.0f,	pos.y + size/2.0f);
		g->drawLine(pos.x + size/2.0f,	pos.y - size/2.0f, pos.x - size/2.0f, pos.y + size/2.0f);
		g->popTransform();	
	}

	g->setColorizationEnabled(false);
}

void Unit_Building::init()
{
	Unit::init();
	maxHealth = 1000.0f;
	health = maxHealth;
	size = 50.0;
}

void Unit_Mob::init()
{
	Unit::init();
	mStateManager->pushState(new State_Moving(this));
}

void Unit_Mob::draw(Boy::Graphics *g)
{
	Unit::draw(g);
}

void Unit_Rock::draw(Boy::Graphics *g)
{
	Unit_Mob::draw(g);

	g->setColorizationEnabled(false);
}

void Unit_Rock::init()
{
	Unit_Mob::init();

	damageTypes.push_back(EUnitDamageType_Rock);
	vulnerabilityTypes.push_back(EUnitDamageType_Paper);
	resistanceTypes.push_back(EUnitDamageType_Scissors);
	mImage = Boy::Environment::getImage("IMAGE_ROCK");
}

void Unit_Paper::draw(Boy::Graphics *g)
{
	Unit_Mob::draw(g);

	g->setColorizationEnabled(false);
}

void Unit_Paper::init()
{
	Unit_Mob::init();

	damageTypes.push_back(EUnitDamageType_Paper);
	vulnerabilityTypes.push_back(EUnitDamageType_Scissors);
	resistanceTypes.push_back(EUnitDamageType_Rock);
	mImage = Boy::Environment::getImage("IMAGE_PAPER");
}

void Unit_Scissors::draw(Boy::Graphics *g)
{
	Unit_Mob::draw(g);

	g->setColorizationEnabled(false);
}

void Unit_Scissors::init()
{
	Unit_Mob::init();

	damageTypes.push_back(EUnitDamageType_Scissors);
	vulnerabilityTypes.push_back(EUnitDamageType_Rock);
	resistanceTypes.push_back(EUnitDamageType_Paper);
	mImage = Boy::Environment::getImage("IMAGE_SCISSORS");
}