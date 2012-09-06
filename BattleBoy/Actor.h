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
	virtual void update(float dt);
	virtual void initStats();
	void setOwningGame(BattleBoy *_game){ game=_game; }
	BattleBoy* getGame() { return game; }
	const BoyLib::Vector2& getPos(){ return pos; }
	const BoyLib::Vector2& getDir(){ return dir; }
	const BoyLib::Vector2& getVel(){ return vel; }
	const BoyLib::Vector2& getAccel(){ return accel; }
	float getMaxAccel(){ return 500.0f; }
	float getMaxSpeed(){ return 200.0f; }

	void setAccel(const BoyLib::Vector2& inAccel) { accel = inAccel; }
	void setVel(const BoyLib::Vector2& inVel) { vel = inVel; }

	void setDestroyed(bool value){ bDestroyed = value; }
	bool isDestroyed() {return bDestroyed; }
protected:
	BattleBoy *game;
	BoyLib::Vector2 pos, dir, vel, accel;
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
	bool attack(float dt);
	void setTeamIdx(int idx){ teamIdx = idx; }
	int getTeamIdx() { return teamIdx; }
	void takeDamage(float damageTaken);
	float modifyDamage(float damage, std::vector<EUnitDamageType> attackingDamageType, std::vector<EUnitDamageType> defendingVulnerabilityTypes, std::vector<EUnitDamageType> defendingResistanceTypes);
	virtual void initStats();
	Steering* getSteering() {return steering;}
	void setHealth(float value){ health = value; }
	float getHealth() { return health; }
	void setMaxHealth(float value) { maxHealth = value; }
	float getMaxHealth() { return maxHealth; }
protected:
	float timeSinceLastAttack;
	float range, damage, health, maxHealth, attackRate;
	int teamIdx;
	std::vector<EUnitDamageType> damageTypes;
	std::vector<EUnitDamageType> vulnerabilityTypes;
	std::vector<EUnitDamageType> resistanceTypes;
	Steering* steering;
	Boy::Image *mImage;
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