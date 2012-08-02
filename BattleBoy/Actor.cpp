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
		// Currently hardcode destination reached as 10
		if (dist(pos.x,pos.y,destinations[0].x,destinations[0].y) < 10)
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
	bool result = false;

	if (TimeSinceLastAttack > AttackRate)
	{
		TimeSinceLastAttack = 0;
		for( std::vector<Actor*>::iterator it = Units.begin(); it != Units.end() ; ++it )
		{
			if (dist(pos.x, pos.y, (*it)->pos.x, (*it)->pos.y) < (*it)->Range)
			{
				(*it)->TakeDamage(Damage);
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

void Actor::SetDestination(BoyLib::Vector2 dest)
{
	destinations.push_back(dest);
}

void Unit::draw(Boy::Graphics *g)
{
	float size = 30;
	g->pushTransform();

	//BoyLib::Vector2 topleft, topright, bottomright, bottomleft;

	g->drawLine(int(pos.x - size/2.0f),	int(pos.y - size/2.0f),	int(pos.x + size/2.0f),	int(pos.y - size/2.0f));
	g->drawLine(int(pos.x + size/2.0f),	int(pos.y - size/2.0f), int(pos.x + size/2.0f), int(pos.y + size/2.0f));

	g->drawLine(int(pos.x + size/2.0f),	int(pos.y + size/2.0f), int(pos.x - size/2.0f), int(pos.y + size/2.0f));
	g->drawLine(int(pos.x - size/2.0f),	int(pos.y + size/2.0f),	int(pos.x - size/2.0f), int(pos.y - size/2.0f));

	// draw status
	g->setColorizationEnabled(true);
	g->setColor(0x00ff00ff);
	float healthPercentage = Health/MaxHealth;
	if (healthPercentage < 0.25f)
	{
		g->setColor(0xff0000ff);
	}
	else if (healthPercentage < 0.50)
	{
		g->setColor(0xff8c00ff);
	}
	g->translate(pos.x + size/2.5f,pos.y - size/2.5f );
	Boy::ResourceManager *rm = Boy::Environment::instance()->getResourceManager();
	Boy::Font *mFont = rm->getFont("FONT_MAIN");

	char healthText[100];
	int h1 = int(Health);            // Get the integer part
	float f1 = Health - h1;     // Get fractional part
	int h2 = int(f1 * 100);   // Turn into integer
	int mh1 = int(MaxHealth);            // Get the integer part
	float f2 = MaxHealth - mh1;     // Get fractional part
	int mh2 = int(f2 * 100);   // Turn into integer

	sprintf_s(healthText, "%d.%04d/%d.%04d", h1, h2, mh1, mh2);
	mFont->drawString(g,healthText,0.25f);
	g->setColorizationEnabled(false);
	
	g->popTransform();
}

void Building::draw(Boy::Graphics *g)
{
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
}

void Unit_Rock::draw(Boy::Graphics *g)
{
	float size = 30;
	g->pushTransform();

	//BoyLib::Vector2 topleft, topright, bottomright, bottomleft;

	g->drawLine(int(pos.x),	int(pos.y - size/2.0f),	int(pos.x + size/2.0f),	int(pos.y));
	g->drawLine(int(pos.x + size/2.0f),	int(pos.y), int(pos.x), int(pos.y + size/2.0f));

	g->drawLine(int(pos.x),	int(pos.y + size/2.0f), int(pos.x - size/2.0f), int(pos.y));
	g->drawLine(int(pos.x - size/2.0f),	int(pos.y),	int(pos.x), int(pos.y - size/2.0f));

	
	g->popTransform();
}

void Unit_Paper::draw(Boy::Graphics *g)
{
	float size = 30;
	g->pushTransform();

	//BoyLib::Vector2 topleft, topright, bottomright, bottomleft;

	g->drawLine(int(pos.x - size/2.0f),	int(pos.y - size/2.0f),	int(pos.x + size/2.0f),	int(pos.y - size/2.0f));
	g->drawLine(int(pos.x + size/2.0f),	int(pos.y - size/2.0f), int(pos.x + size/2.0f), int(pos.y + size/2.0f));

	g->drawLine(int(pos.x + size/2.0f),	int(pos.y + size/2.0f), int(pos.x - size/2.0f), int(pos.y + size/2.0f));
	g->drawLine(int(pos.x - size/2.0f),	int(pos.y + size/2.0f),	int(pos.x - size/2.0f), int(pos.y - size/2.0f));

	
	g->popTransform();
}

void Unit_Scissors::draw(Boy::Graphics *g)
{
	float size = 30;
	g->pushTransform();

	//BoyLib::Vector2 topleft, topright, bottomright, bottomleft;

	g->drawLine(int(pos.x - size/2.0f),	int(pos.y - size/2.0f),	int(pos.x + size/2.0f),	int(pos.y + size/2.0f));
	g->drawLine(int(pos.x + size/2.0f),	int(pos.y - size/2.0f), int(pos.x - size/2.0f), int(pos.y + size/2.0f));
	
	g->popTransform();
}