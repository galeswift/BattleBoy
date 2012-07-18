#pragma once

class Actor
{
public:
	Actor() : pos() {};
	Actor(BoyLib::Vector2 _pos ) : pos(_pos) {};
	virtual void draw(Boy::Graphics *g) {};
	BoyLib::Vector2 pos;
};

class Unit : public Actor
{
public:
	Unit() {};
	Unit(BoyLib::Vector2 _pos ) : Actor( _pos) {} ;
	virtual void draw(Boy::Graphics *g);
};