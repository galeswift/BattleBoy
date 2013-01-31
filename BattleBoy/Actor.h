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
	virtual void init();
	void setOwningGame(BattleBoy *_game){ game=_game; }
	BattleBoy* getGame() { return game; }
	const BoyLib::Vector2& getPos(){ return pos; }
	const BoyLib::Vector2& getDir(){ return dir; }
	const BoyLib::Vector2& getVel(){ return vel; }
	const BoyLib::Vector2& getAccel(){ return accel; }
	float getSize(){ return size; }

	float getMaxAccel(){ return 500.0f; }
	float getMaxSpeed(){ return 200.0f; }

	void setAccel(const BoyLib::Vector2& inAccel) { accel = inAccel; }
	void setVel(const BoyLib::Vector2& inVel) { vel = inVel; }

	void setDestroyed(bool value){ bDestroyed = value; }
	bool isDestroyed() { return bDestroyed; }

	void setShouldErase(bool value){ bShouldErase = value; }
	bool shouldErase() { return bShouldErase; }

	virtual bool collidedWith(Actor* other);
protected:
	BattleBoy *game;
	BoyLib::Vector2 pos, dir, vel, accel;
	float size, speed;
	bool bDestroyed;
	bool bShouldErase;
};

class Projectile : public Actor
{
public:
	Projectile(Unit* instigator, BoyLib::Vector2 _pos, BoyLib::Vector2& dir) : Actor(_pos), mDir(dir), mInstigator(instigator) {}
	virtual void init();
	virtual void update(float dt);
	virtual void draw(Boy::Graphics *g);	
private:
	BoyLib::Vector2& mDir;
	Unit* mInstigator;
};