#pragma once

#define NUM_PARTICLES 20

class Explosion : public Actor
{
public:

	Explosion(float x, float y, float radius);

	bool isOver();
	void draw(Boy::Graphics *g);
	void update(float dt);

public:

	Boy::Image *mImage;
	float mStartTime;
	float mDuration;

	BoyLib::Vector2 mParticlePositions[NUM_PARTICLES];
	BoyLib::Vector2 mParticleVelocities[NUM_PARTICLES];

};

