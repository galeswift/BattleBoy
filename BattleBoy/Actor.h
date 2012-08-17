#pragma once

enum EUnitTeam
{
	ESpawnType_Player = 0,
	ESpawnType_AI = 1,
};

class Actor
{
public:
	Actor() : pos() , speed() { InitStats(); };
	Actor(BoyLib::Vector2 _pos) : pos(_pos) { InitStats(); };
	Actor(BoyLib::Vector2 _pos, float _speed) : pos(_pos), speed(_speed) { InitStats(); };
	virtual void draw(Boy::Graphics *g) {};
	void drawHealth(Boy::Graphics *g);	
	void update(float dt, std::vector<Actor*> Units);
	bool move(float dt);
	bool attack(float dt, std::vector<Actor*> Units);
	BoyLib::Vector2 pos;
	EUnitTeam Team;
	float speed;
	float Range;
	float Damage;
	float Health;
	float MaxHealth;
	float AttackRate;
	bool bDead;
	std::vector<BoyLib::Vector2> destinations;
	BoyLib::Vector2 dir;
	BoyLib::Vector2 vel;
	void SetDestination(BoyLib::Vector2 dest);
	void TakeDamage(float damageTaken);
	bool bInvulnerable;
	void InitStats();
	float TimeSinceLastAttack;
};

class Unit : public Actor
{
public:
	Unit() {};
	Unit(BoyLib::Vector2 _pos, float _speed) : Actor(_pos, _speed) {} ;
	virtual void draw(Boy::Graphics *g){};
};

class Building : public Actor
{
public:
	Building() { InitStats(); };
	Building(BoyLib::Vector2 _pos) : Actor(_pos) { InitStats(); } ;
	virtual void draw(Boy::Graphics *g);
	void InitStats();
};

class SpawnPoint : public Actor
{
public:
	SpawnPoint() { InitStats(); };
	SpawnPoint(BoyLib::Vector2 _pos) : Actor(_pos) { InitStats(); } ;
	void InitStats();
};

class Unit_Rock : public Unit
{
public:
	Unit_Rock() { InitStats(); };
	Unit_Rock(BoyLib::Vector2 _pos, float _speed) : Unit(_pos, _speed) { InitStats(); } ;
	virtual void draw(Boy::Graphics *g);
	void InitStats();
};

class Unit_Paper : public Unit
{
public:
	Unit_Paper() { InitStats(); };
	Unit_Paper(BoyLib::Vector2 _pos, float _speed) : Unit(_pos, _speed) { InitStats(); } ;
	virtual void draw(Boy::Graphics *g);
	void InitStats();
};

class Unit_Scissors : public Unit
{
public:
	Unit_Scissors() { InitStats(); };
	Unit_Scissors(BoyLib::Vector2 _pos, float _speed) : Unit(_pos, _speed) { InitStats(); } ;
	virtual void draw(Boy::Graphics *g);
	void InitStats();
};