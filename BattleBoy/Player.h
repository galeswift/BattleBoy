#pragma once

class Player : public Actor
{
public:
	Player() {};
	virtual void update(float dt);
	virtual void init();
	void setIsAI(bool value){ bIsAI = value; };
	bool isAI() { return bIsAI; }
	void setCredits(int value){ credits = value; };
	int getCredits() { return credits; }
protected:
	int credits;
	float creditsTime;
	float creditsRate;
	int creditsIncrement;
	bool bIsAI;
};