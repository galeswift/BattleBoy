#pragma once

#define MAX_PARTICLES (50)

class Explosion : public Actor
{
public:

	Explosion(float x, float y, float radius, int numParticles = 20, Boy::Color color = 0xFFFFFFFF);

	bool isOver();
	void draw(Boy::Graphics *g);
	void update(float dt);

public:

	Boy::Image *mImage;
	float mStartTime;
	float mDuration;
	Boy::Color mColor;
	int mNumParticles;

	BoyLib::Vector2 mParticlePositions[MAX_PARTICLES];
	BoyLib::Vector2 mParticleVelocities[MAX_PARTICLES];

};

