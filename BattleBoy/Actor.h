#pragma once

enum EUnitTeam
{
	ESpawnType_Players = 0,
	ESpawnType_AI = 1,
};

class Actor
{
public:
	Actor() : pos() , speed() {};
	Actor(BoyLib::Vector2 _pos) : pos(_pos) {};
	Actor(BoyLib::Vector2 _pos, float _speed) : pos(_pos), speed(_speed) {};
	virtual void draw(Boy::Graphics *g) {};
	void update(float dt, std::vector<Unit*> Units);
	bool move(float dt);
	bool attack(float dt, std::vector<Unit*> Units);
	BoyLib::Vector2 pos;
	EUnitTeam Team;
	float speed;
	float Range;
	float Damage;
	float Health;
	float AttackRate;
	bool bDead = false;
	std::vector<BoyLib::Vector2> destinations;
	BoyLib::Vector2 dir;
	BoyLib::Vector2 vel;
	void SetDestination(BoyLib::Vector2 dest);
	void TakeDamage(float damageTaken);
private:
	float TimeSinceLastAttack; 
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
	Unit_Rock() {};
	Unit_Rock(BoyLib::Vector2 _pos, float _speed) : Unit(_pos, _speed) {} ;
	virtual void draw(Boy::Graphics *g);
};

class Unit_Paper : public Unit
{
public:
	Unit_Paper() {};
	Unit_Paper(BoyLib::Vector2 _pos, float _speed) : Unit(_pos, _speed) {} ;
	virtual void draw(Boy::Graphics *g);
};

class Unit_Scissors : public Unit
{
public:
	Unit_Scissors() {};
	Unit_Scissors(BoyLib::Vector2 _pos, float _speed) : Unit(_pos, _speed) {} ;
	virtual void draw(Boy::Graphics *g);
};