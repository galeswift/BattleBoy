#pragma once

class Actor
{
public:
	Actor() : pos() , speed() {};
	Actor(BoyLib::Vector2 _pos, float _speed) : pos(_pos), speed(_speed) {};
	virtual void draw(Boy::Graphics *g) {};
	void update(float dt);
	BoyLib::Vector2 pos;
	float speed;
	std::vector<BoyLib::Vector2> destinations;
	BoyLib::Vector2 dir;
	BoyLib::Vector2 vel;
	void SetDestination(BoyLib::Vector2 dest);
};

class Unit : public Actor
{
public:
	Unit() {};
	Unit(BoyLib::Vector2 _pos, float _speed) : Actor(_pos, _speed) {} ;
	virtual void draw(Boy::Graphics *g);
};