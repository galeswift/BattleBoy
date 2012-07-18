#pragma once

class Actor
{
public:
	Actor() : pos() {};
	Actor(BoyLib::Vector2 _pos, BoyLib::Vector2 _vel);
	virtual void draw(Boy::Graphics *g) {};
	void update(float dt);
	BoyLib::Vector2 pos;
	BoyLib::Vector2 vel;
};

class Unit : public Actor
{
public:
	Unit() {};
	Unit(BoyLib::Vector2 _pos, BoyLib::Vector2 _vel) : Actor(_pos, _vel) {} ;
	virtual void draw(Boy::Graphics *g);
};