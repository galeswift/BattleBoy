#include "Globals.h"

void Actor::update(float dt)
{	
	if (destinations.size() > 0)
	{		
		if (dist(pos.x,pos.y,destinations[0].x,destinations[0].y) < range)
		{
			destinations.erase(destinations.begin());
		}
		if (destinations.size() > 0)
		{
			dir = (destinations.front() - pos).normalize();
			vel = dir*speed;
			pos += vel * dt;
		}
	}
}

void Actor::InitDamageRange()
{
	damage = 0;
	range = 0;
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

void Unit_Rock::InitDamageRange()
{
	damage = 10;
	range = 30;
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

void Unit_Paper::InitDamageRange()
{
	damage = 10;
	range = 60;
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

void Unit_Scissors::InitDamageRange()
{
	damage = 10;
	range = 90;
}
