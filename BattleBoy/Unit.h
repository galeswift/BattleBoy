#pragma once

class Unit : public Actor
{
public:
	Unit() {};
	Unit(BoyLib::Vector2 _pos) : Actor( _pos ) {} ;
	Unit(BoyLib::Vector2 _pos, float _speed) : Actor(_pos, _speed) {} ;
	virtual void draw(Boy::Graphics *g);
	void drawHealth(Boy::Graphics *g);	
	void update(float dt);
	void setTeamIdx(int idx){ teamIdx = idx; }
	int getTeamIdx() { return teamIdx; }
	void takeDamage(Unit* attacker);
	float calcDamage(Unit* attacker);
	virtual void init();
	Steering* getSteering() {return steering;}
	void setHealth(float value){ health = value; }
	float getHealth() { return health; }
	void setMaxHealth(float value) { maxHealth = value; }
	float getMaxHealth() { return maxHealth; }
	float getRange() { return range; }
	float getDamage() { return damage; }
	float getAttackRate() { return attackRate; }
	StateManager* getStateManager() { return mStateManager; }
	const std::vector<EUnitDamageType>& getDamageTypes() {return damageTypes;}

protected:
	float range, damage, health, maxHealth, attackRate;
	int teamIdx;
	std::vector<EUnitDamageType> damageTypes;
	std::vector<EUnitDamageType> vulnerabilityTypes;
	std::vector<EUnitDamageType> resistanceTypes;
	Steering* steering;
	Boy::Image *mImage;
	StateManager* mStateManager;
};

class Unit_Building : public Unit
{
public:
	Unit_Building() : Unit() {};
	Unit_Building(BoyLib::Vector2 _pos) : Unit(_pos) { } ;
	virtual void draw(Boy::Graphics *g);
	void init();
};

class Unit_Mob : public Unit
{
public:
	Unit_Mob() : Unit() {};
	Unit_Mob(BoyLib::Vector2 _pos, float _speed) : Unit(_pos, _speed) {} ;
	virtual void draw(Boy::Graphics *g);
	virtual void init();
};

class Unit_Rock : public Unit_Mob
{
public:
	Unit_Rock() : Unit_Mob() { };
	Unit_Rock(BoyLib::Vector2 _pos, float _speed) : Unit_Mob(_pos, _speed) {} ;
	virtual void draw(Boy::Graphics *g);
	void init();
};

class Unit_Paper : public Unit_Mob
{
public:
	Unit_Paper() : Unit_Mob() { };
	Unit_Paper(BoyLib::Vector2 _pos, float _speed) : Unit_Mob(_pos, _speed) { } ;
	virtual void draw(Boy::Graphics *g);
	void init();
};

class Unit_Scissors : public Unit_Mob
{
public:
	Unit_Scissors() : Unit_Mob() { };
	Unit_Scissors(BoyLib::Vector2 _pos, float _speed) : Unit_Mob(_pos, _speed) { } ;
	virtual void draw(Boy::Graphics *g);
	void init();
};