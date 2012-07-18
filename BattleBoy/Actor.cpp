#include "Globals.h"

Actor::Actor(BoyLib::Vector2 _pos, BoyLib::Vector2 _vel) {
  pos = _pos;
  vel = _vel;
}

void Actor::update(float dt)
{
	pos += vel*dt;
}

void Unit::draw(Boy::Graphics *g)
{
	float size = 30;
	g->pushTransform();

	//BoyLib::Vector2 topleft, topright, bottomright, bottomleft;

	g->drawLine(pos.x - size/2.0f , pos.y - size/2.0f, pos.x+size/2.0f, pos.y - size/2.0f);
	g->drawLine(pos.x + size/2.0f, pos.y - size/2.0f, pos.x+size/2.0f, pos.y + size/2.0f);

	g->drawLine(pos.x+size/2.0f, pos.y + size/2.0f, pos.x-size/2.0f, pos.y+size/2.0f);
	g->drawLine(pos.x-size/2.0f, pos.y+size/2.0f, pos.x - size/2.0f , pos.y - size/2.0f);

	
	g->popTransform();
}