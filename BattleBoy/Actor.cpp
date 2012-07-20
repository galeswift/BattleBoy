#include "Globals.h"

void Actor::update(float dt)
{	
	if (destinations.size() > 0)
	{		
		// Currently hardcode attack range as 10
		if (dist(pos.x,pos.y,destinations[0].x,destinations[0].y) < 10)
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
	g->drawLine(int(pos.x-  size/2.0f),	int(pos.y + size/2.0f),	int(pos.x - size/2.0f), int(pos.y - size/2.0f));

	
	g->popTransform();
}