#include "Globals.h"

void Actor::update(float dt, std::vector<Actor*> Units)
{
	// Only move if we are not attacking
	if (!attack(dt, Units))
	{
		move(dt);
	}
}

bool Actor::move(float dt)
{	
	bool result = false;
	if (destinations.size() > 0)
	{		
		if (dist(pos.x,pos.y,destinations[0].x,destinations[0].y) < Range)
		{
			destinations.erase(destinations.begin());
		}
		if (destinations.size() > 0)
		{
			dir = (destinations.front() - pos).normalize();
			vel = dir*speed;
			pos += vel * dt;
			result = true;
		}
	}
	return result;
}

bool Actor::attack(float dt, std::vector<Actor*> Units)
{
	bool result = TimeSinceLastAttack < AttackRate;

	if (Damage > 0 && !result)
	{
		for( std::vector<Actor*>::iterator it = Units.begin(); it != Units.end() ; ++it )
		{
			if (dist(pos.x, pos.y, (*it)->pos.x, (*it)->pos.y) < Range)
			{
				if (!(*it)->bInvulnerable && !(*it)->bDead && (*it)->Team != Team)
				{
					(*it)->TakeDamage(ModifyDamage(Damage, DamageTypes, (*it)->VulnerabilityTypes, (*it)->ResistanceTypes));
					TimeSinceLastAttack = 0;
					result = true;
					break;
				}
			}
		}
	}
	else
	{
		TimeSinceLastAttack += dt;
	}


	return result;
}

void Actor::TakeDamage(float damageTaken)
{
	Health -= damageTaken;
	if (Health <= 0)
	{
		bDead = true;
	}
}

float Actor::ModifyDamage(float damage, std::vector<EUnitDamageType> AttackingDamageTypes, std::vector<EUnitDamageType> DefendingVulnerabilityTypes, std::vector<EUnitDamageType> DefendingResistanceTypes)
{
	float modifier = 1.0f;
	for(int d = 0; d < AttackingDamageTypes.size(); d++)
	{
		for(int v = 0; v < DefendingVulnerabilityTypes.size(); v++)
		{
			if (AttackingDamageTypes[d] == DefendingVulnerabilityTypes[v])
			{
				modifier += 0.5f;
			}
		}
		for(int r = 0; r < DefendingResistanceTypes.size(); r++)
		{
			if (AttackingDamageTypes[d] == DefendingResistanceTypes[r])
			{
				modifier -= 0.5f;
			}
		}
	}
	modifier = std::max<float>(0.0f, modifier);
	return (damage*modifier);
}

void Actor::InitStats()
{
	Damage = 0;
	Range = 0;
	MaxHealth = 500.0f;
	Health = MaxHealth;
	bDead = false;
	bInvulnerable = false;
	TimeSinceLastAttack = 0.0f;
	AttackRate = 0.0f;
}

void Actor::SetDestination(BoyLib::Vector2 dest)
{
	destinations.push_back(dest);
}

void Actor::drawHealth(Boy::Graphics *g)
{
	// Holy Hacks Batman! Remove this when you fix BattleBoy::update dead check removal!
	if (bDead || bInvulnerable)
	{
		return;
	}

	float size = 30;
	//g->pushTransform();

	//BoyLib::Vector2 topleft, topright, bottomright, bottomleft;

	//g->drawLine(int(pos.x - size/2.0f),	int(pos.y - size/2.0f),	int(pos.x + size/2.0f),	int(pos.y - size/2.0f));
	//g->drawLine(int(pos.x + size/2.0f),	int(pos.y - size/2.0f), int(pos.x + size/2.0f), int(pos.y + size/2.0f));

	//g->drawLine(int(pos.x + size/2.0f),	int(pos.y + size/2.0f), int(pos.x - size/2.0f), int(pos.y + size/2.0f));
	//g->drawLine(int(pos.x - size/2.0f),	int(pos.y + size/2.0f),	int(pos.x - size/2.0f), int(pos.y - size/2.0f));

	// draw status
	g->setColorizationEnabled(true);
	g->setColor(0xff00ff00);
	float healthPercentage = Health/MaxHealth;
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
	int h1 = int(Health);            // Get the integer part
	float f1 = Health - h1;     // Get fractional part
	int h2 = int(f1 * 100);   // Turn into integer
	int mh1 = int(MaxHealth);            // Get the integer part
	float f2 = MaxHealth - mh1;     // Get fractional part
	int mh2 = int(f2 * 100);   // Turn into integer

	//sprintf_s(healthText, "%d.%04d/%d.%04d", h1, h2, mh1, mh2);
	sprintf_s(healthText, "%dl%d", h1, mh1);
	
	g->pushTransform();
	g->translate(pos.x + size/2.5f,pos.y - size/2.5f );
	mFont->drawString(g,healthText,0.25f);
	g->popTransform();

	g->setColorizationEnabled(false);
	
}

void Building::draw(Boy::Graphics *g)
{	
	g->setColorizationEnabled(true);

	if (Team == ESpawnType_Player)
	{
		g->setColor(0xff00ffff);
	}
	else
	{
		g->setColor(0xffff0000);
	}

	float size = 25.0;
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

void Building::InitStats()
{
	Damage = 0.0f;
	Range = 0.0f;
	MaxHealth = 1000.0f;
	Health = MaxHealth;
	bDead = false;
	bInvulnerable = false;
	TimeSinceLastAttack = 0.0f;
	AttackRate = 0.0f;
}

void SpawnPoint::InitStats()
{
	Damage = 0.0f;
	Range = 0.0f;
	MaxHealth = 50000.0f;
	Health = MaxHealth;
	bDead = false;
	bInvulnerable = true;
	TimeSinceLastAttack = 0.0f;
	AttackRate = 0.0f;
}

void Unit_Rock::draw(Boy::Graphics *g)
{
	// Holy Hacks Batman! Remove this when you fix BattleBoy::update dead check removal!
	if (bDead)
	{
		return;
	}

	float size = 30;
	
	g->setColorizationEnabled(true);

	if (Team == ESpawnType_Player)
	{
		g->setColor(0xff00ffff);
	}
	else
	{
		g->setColor(0xffff0000);
	}

	g->pushTransform();

	//BoyLib::Vector2 topleft, topright, bottomright, bottomleft;

	g->drawLine(int(pos.x),	int(pos.y - size/2.0f),	int(pos.x + size/2.0f),	int(pos.y));
	g->drawLine(int(pos.x + size/2.0f),	int(pos.y), int(pos.x), int(pos.y + size/2.0f));

	g->drawLine(int(pos.x),	int(pos.y + size/2.0f), int(pos.x - size/2.0f), int(pos.y));
	g->drawLine(int(pos.x - size/2.0f),	int(pos.y),	int(pos.x), int(pos.y - size/2.0f));

	g->popTransform();	

	g->setColorizationEnabled(false);
}

void Unit_Rock::InitStats()
{
	Damage = 10.0f;
	Range = 30.0f;
	MaxHealth = 50.0f;
	Health = MaxHealth;
	bDead = false;
	bInvulnerable = false;
	AttackRate = 0.5f;
	TimeSinceLastAttack = AttackRate;
	DamageTypes.push_back(EUnitDamageType_Rock);
	VulnerabilityTypes.push_back(EUnitDamageType_Paper);
	ResistanceTypes.push_back(EUnitDamageType_Scissors);
}

void Unit_Paper::draw(Boy::Graphics *g)
{
	// Holy Hacks Batman! Remove this when you fix BattleBoy::update dead check removal!
	if (bDead)
	{
		return;
	}
	
	g->setColorizationEnabled(true);

	if (Team == ESpawnType_Player)
	{
		g->setColor(0xff00ffff);
	}
	else
	{
		g->setColor(0xffff0000);
	}

	float size = 30;
	g->pushTransform();

	//BoyLib::Vector2 topleft, topright, bottomright, bottomleft;

	g->drawLine(int(pos.x - size/2.0f),	int(pos.y - size/2.0f),	int(pos.x + size/2.0f),	int(pos.y - size/2.0f));
	g->drawLine(int(pos.x + size/2.0f),	int(pos.y - size/2.0f), int(pos.x + size/2.0f), int(pos.y + size/2.0f));

	g->drawLine(int(pos.x + size/2.0f),	int(pos.y + size/2.0f), int(pos.x - size/2.0f), int(pos.y + size/2.0f));
	g->drawLine(int(pos.x - size/2.0f),	int(pos.y + size/2.0f),	int(pos.x - size/2.0f), int(pos.y - size/2.0f));

	
	g->popTransform();

	g->setColorizationEnabled(false);
}

void Unit_Paper::InitStats()
{
	Damage = 10.0f;
	Range = 60.0f;
	MaxHealth = 50.0f;
	Health = MaxHealth;
	bDead = false;
	bInvulnerable = false;
	AttackRate = 0.5f;
	TimeSinceLastAttack = AttackRate;
	DamageTypes.push_back(EUnitDamageType_Paper);
	VulnerabilityTypes.push_back(EUnitDamageType_Scissors);
	ResistanceTypes.push_back(EUnitDamageType_Rock);
}

void Unit_Scissors::draw(Boy::Graphics *g)
{
	// Holy Hacks Batman! Remove this when you fix BattleBoy::update dead check removal!
	if (bDead)
	{
		return;
	}
	
	g->setColorizationEnabled(true);

	if (Team == ESpawnType_Player)
	{
		g->setColor(0xff00ffff);
	}
	else
	{
		g->setColor(0xffff0000);
	}

	float size = 30;
	g->pushTransform();

	//BoyLib::Vector2 topleft, topright, bottomright, bottomleft;

	g->drawLine(int(pos.x - size/2.0f),	int(pos.y - size/2.0f),	int(pos.x + size/2.0f),	int(pos.y + size/2.0f));
	g->drawLine(int(pos.x + size/2.0f),	int(pos.y - size/2.0f), int(pos.x - size/2.0f), int(pos.y + size/2.0f));
	
	g->popTransform();

	g->setColorizationEnabled(false);
}

void Unit_Scissors::InitStats()
{
	Damage = 10.0f;
	Range = 90.0f;
	MaxHealth = 50.0f;
	Health = MaxHealth;
	bDead = false;
	bInvulnerable = false;
	AttackRate = 0.5f;
	TimeSinceLastAttack = AttackRate;
	DamageTypes.push_back(EUnitDamageType_Scissors);
	VulnerabilityTypes.push_back(EUnitDamageType_Rock);
	ResistanceTypes.push_back(EUnitDamageType_Paper);
}