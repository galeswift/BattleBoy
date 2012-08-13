#pragma once

class Actor
{
public:
	Actor() : pos() , speed() { InitDamageRange(); };
	Actor(BoyLib::Vector2 _pos) : pos(_pos) { InitDamageRange(); };
	Actor(BoyLib::Vector2 _pos, float _speed) : pos(_pos), speed(_speed) { InitDamageRange(); };
	virtual void draw(Boy::Graphics *g) {};
	void update(float dt);
	BoyLib::Vector2 pos;
	float speed;
	std::vector<BoyLib::Vector2> destinations;
	BoyLib::Vector2 dir;
	BoyLib::Vector2 vel;
	void SetDestination(BoyLib::Vector2 dest);
	int damage;
	int range;
	void InitDamageRange();
};

class Unit : public Actor
{
public:
	Unit() {};
	Unit(BoyLib::Vector2 _pos, float _speed) : Actor(_pos, _speed) {} ;
	virtual void draw(Boy::Graphics *g);
};

class Building : public Actor
{
public:
	Building() {};
	Building(BoyLib::Vector2 _pos) : Actor(_pos) {} ;
	virtual void draw(Boy::Graphics *g);
};

class SpawnPoint : public Actor
{
public:
	SpawnPoint() {};
	SpawnPoint(BoyLib::Vector2 _pos) : Actor(_pos) {} ;
};

class Unit_Rock : public Unit
{
public:
	Unit_Rock() { InitDamageRange(); };
	Unit_Rock(BoyLib::Vector2 _pos, float _speed) : Unit(_pos, _speed) { InitDamageRange(); } ;
	virtual void draw(Boy::Graphics *g);
	void InitDamageRange();
};

class Unit_Paper : public Unit
{
public:
	Unit_Paper() { InitDamageRange(); };
	Unit_Paper(BoyLib::Vector2 _pos, float _speed) : Unit(_pos, _speed) { InitDamageRange(); } ;
	virtual void draw(Boy::Graphics *g);
	void InitDamageRange();
};

class Unit_Scissors : public Unit
{
public:
	Unit_Scissors() { InitDamageRange(); };
	Unit_Scissors(BoyLib::Vector2 _pos, float _speed) : Unit(_pos, _speed) { InitDamageRange(); } ;
	virtual void draw(Boy::Graphics *g);
	void InitDamageRange();
};