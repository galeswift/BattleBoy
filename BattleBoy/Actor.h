#pragma once

enum EUnitDamageType
{
	EUnitDamageType_Rock = 0,
	EUnitDamageType_Paper = 1,
	EUnitDamageType_Scissors = 2,
};

class BattleBoy;

class Actor
{
public:
	Actor() : pos() , speed(){ };
	Actor(BoyLib::Vector2 _pos) : pos(_pos) { };
	Actor(BoyLib::Vector2 _pos, float _speed) : pos(_pos), speed(_speed) {};
	virtual void draw(Boy::Graphics *g) {};
	virtual void update(float dt) {};
	virtual void initStats();
	void setOwningGame(BattleBoy *_game){ game=_game; }

	const BoyLib::Vector2& getPos(){ return pos; }
	const BoyLib::Vector2& getDir(){ return dir; }
	const BoyLib::Vector2& getVel(){ return vel; }
	void setDestroyed(bool value){ bDestroyed = value; };
	bool isDestroyed() {return bDestroyed; }
protected:
	BattleBoy *game;
	BoyLib::Vector2 pos, dir, vel;
	float size, speed;
	bool bDestroyed;
};

class Unit : public Actor
{
public:
	Unit() {};
	Unit(BoyLib::Vector2 _pos) : Actor( _pos ) {} ;
	Unit(BoyLib::Vector2 _pos, float _speed) : Actor(_pos, _speed) {} ;
	virtual void draw(Boy::Graphics *g);
	void drawHealth(Boy::Graphics *g);	
	void update(float dt);
	bool move(float dt);
	bool attack(float dt);
	void setDestination(BoyLib::Vector2 dest);
	void setTeamIdx(int idx){ teamIdx = idx; }
	int getTeamIdx() { return teamIdx; }
	void takeDamage(float damageTaken);
	float modifyDamage(float damage, std::vector<EUnitDamageType> attackingDamageType, std::vector<EUnitDamageType> defendingVulnerabilityTypes, std::vector<EUnitDamageType> defendingResistanceTypes);
	virtual void initStats();
protected:
	float timeSinceLastAttack;
	float range, damage, health, maxHealth, attackRate;
	int teamIdx;
	std::vector<EUnitDamageType> damageTypes;
	std::vector<EUnitDamageType> vulnerabilityTypes;
	std::vector<EUnitDamageType> resistanceTypes;
	std::vector<BoyLib::Vector2> destinations;
};

class Unit_Building : public Unit
{
public:
	Unit_Building() : Unit() {};
	Unit_Building(BoyLib::Vector2 _pos) : Unit(_pos) { } ;
	virtual void draw(Boy::Graphics *g);
	void initStats();
};

class Unit_Rock : public Unit
{
public:
	Unit_Rock() : Unit() { };
	Unit_Rock(BoyLib::Vector2 _pos, float _speed) : Unit(_pos, _speed) {} ;
	virtual void draw(Boy::Graphics *g);
	void initStats();
};

class Unit_Paper : public Unit
{
public:
	Unit_Paper() : Unit() { };
	Unit_Paper(BoyLib::Vector2 _pos, float _speed) : Unit(_pos, _speed) { } ;
	virtual void draw(Boy::Graphics *g);
	void initStats();
};

class Unit_Scissors : public Unit
{
public:
	Unit_Scissors() : Unit() { };
	Unit_Scissors(BoyLib::Vector2 _pos, float _speed) : Unit(_pos, _speed) { } ;
	virtual void draw(Boy::Graphics *g);
	void initStats();
};