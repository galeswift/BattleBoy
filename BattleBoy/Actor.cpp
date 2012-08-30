#include "Globals.h"

void Actor::update(float dt )
{
	accel.truncate(getMaxAccel());

	vel += accel * dt;
	vel.truncate(getMaxSpeed());
	pos += vel * dt;
}

void Actor::initStats()
{
	size = 15;
	setDestroyed(false);
	game = NULL;
}

void Unit::update(float dt)
{
	if( steering )
	{
		steering->update(dt);
	}

	Actor::update(dt);
	/*if (!attack(dt))
	{
		move(dt);
	}*/
}

bool Unit::attack(float dt)
{
	bool result = timeSinceLastAttack < attackRate;

	if (damage > 0 && !result)
	{
		for( std::vector<Actor*>::iterator it = game->getActors().begin(); it != game->getActors().end() ; ++it )
		{
			Unit* unit = dynamic_cast<Unit*>(*it);
			if( unit )
			{
				if (dist(getPos().x, getPos().y, unit->getPos().x, unit->getPos().y) < range)
				{
					if (unit->getTeamIdx() != getTeamIdx())
					{
						unit->takeDamage(modifyDamage(damage, damageTypes, unit->vulnerabilityTypes, unit->resistanceTypes));
						timeSinceLastAttack = 0;
						result = true;
						break;
					}
				}
			}
		}
	}
	else
	{
		timeSinceLastAttack += dt;
	}


	return result;
}

void Unit::takeDamage(float damageTaken)
{
	health -= damageTaken;
	if (health <= 0)
	{
		setDestroyed(true);
	}
}

float Unit::modifyDamage(float damage, std::vector<EUnitDamageType> attackingDamageTypes, std::vector<EUnitDamageType> defendingVulnerabilityTypes, std::vector<EUnitDamageType> defendingResistanceTypes)
{
	float modifier = 1.0f;
	for(unsigned int d = 0; d < attackingDamageTypes.size(); d++)
	{
		for(unsigned int v = 0; v < defendingVulnerabilityTypes.size(); v++)
		{
			if (attackingDamageTypes[d] == defendingVulnerabilityTypes[v])
			{
				modifier += 0.5f;
			}
		}
		for(unsigned int r = 0; r < defendingResistanceTypes.size(); r++)
		{
			if (attackingDamageTypes[d] == defendingResistanceTypes[r])
			{
				modifier -= 0.5f;
			}
		}
	}
	modifier = std::max<float>(0.0f, modifier);
	return (damage*modifier);
}

void Unit::draw(Boy::Graphics *g)
{
	Actor::draw(g);

	g->setColorizationEnabled(true);

	if (getTeamIdx() == 0)
	{
		g->setColor(0xff00ffff);
	}
	else
	{
		g->setColor(0xffff0000);
	}
	drawHealth(g);

	
	if (mImage != NULL)
	{
		g->pushTransform();
			// TODO Rotation
			g->rotateDeg((teamIdx - 1)*180.0f);
			g->translate(pos.x,pos.y);
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
	g->translate(pos.x + size/2.5f,pos.y - size/2.5f );
	mFont->drawString(g,healthText,0.25f);
	g->popTransform();

	g->setColorizationEnabled(false);
}

void Unit::initStats()
{
	Actor::initStats();

	steering = new Steering(this);
	damage = 10.0f;
	range = 30.0f;
	maxHealth = 50.0f;
	health = maxHealth;
	attackRate = 0.5f;
	timeSinceLastAttack = attackRate;
	mImage = NULL;
}

void Unit_Building::draw(Boy::Graphics *g)
{	
	Unit::draw(g);

	g->pushTransform();

	int segments = 100;
	int lastX = 0;
	int lastY = 0;
	int x = int(pos.x);
	int y = int(pos.y);

    for( int n = 0; n <= segments; ++n )
	{
        float const t = 2.0f*3.14f*float(n)/float(segments);	
		x = int(pos.x + sin(t)*size);
		y = int(pos.y + cos(t)*size);
		if (lastX != 0 && lastY != 0)
		{
			g->drawLine(x, y, lastX, lastY);
		}
		lastX = x;
		lastY = y;
    }
	
	g->popTransform();

	g->setColorizationEnabled(false);
}

void Unit_Building::initStats()
{
	Unit::initStats();
	maxHealth = 1000.0f;
	health = maxHealth;
	size = 25.0;
}

void Unit_Rock::draw(Boy::Graphics *g)
{
	Unit::draw(g);

	g->setColorizationEnabled(false);
}

void Unit_Rock::initStats()
{
	Unit::initStats();

	damageTypes.push_back(EUnitDamageType_Rock);
	vulnerabilityTypes.push_back(EUnitDamageType_Paper);
	resistanceTypes.push_back(EUnitDamageType_Scissors);
	mImage = Boy::Environment::getImage("IMAGE_ROCK");
}

void Unit_Paper::draw(Boy::Graphics *g)
{
	Unit::draw(g);

	g->setColorizationEnabled(false);
}

void Unit_Paper::initStats()
{
	Unit::initStats();

	damageTypes.push_back(EUnitDamageType_Paper);
	vulnerabilityTypes.push_back(EUnitDamageType_Scissors);
	resistanceTypes.push_back(EUnitDamageType_Rock);
	mImage = Boy::Environment::getImage("IMAGE_PAPER");
}

void Unit_Scissors::draw(Boy::Graphics *g)
{
	Unit::draw(g);

	g->setColorizationEnabled(false);
}

void Unit_Scissors::initStats()
{
	Unit::initStats();

	damageTypes.push_back(EUnitDamageType_Scissors);
	vulnerabilityTypes.push_back(EUnitDamageType_Rock);
	resistanceTypes.push_back(EUnitDamageType_Paper);
	mImage = Boy::Environment::getImage("IMAGE_SCISSORS");
}