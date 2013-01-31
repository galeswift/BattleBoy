#include "Globals.h"

#define PARTICLE_MIN_SPEED 100.0f
#define PARTICLE_MAX_SPEED 200.0f

Explosion::Explosion(float x, float y, float radius, int numParticles, Boy::Color color) : 
	mNumParticles(numParticles),
	mColor(color)
{
	if( mNumParticles >= MAX_PARTICLES )
	{
		mNumParticles = MAX_PARTICLES - 1;
	}

	// initialize the particles:
	for (int i=0 ; i<mNumParticles ; i++)
	{
		mParticlePositions[i] = rotate(
			BoyLib::Vector2(0, radius),
			randf(0,6.28f));
		mParticleVelocities[i] = 
			mParticlePositions[i].normalize() * randf(PARTICLE_MIN_SPEED, PARTICLE_MAX_SPEED);
		mParticlePositions[i].x += x;
		mParticlePositions[i].y += y;
	}

	mImage = Boy::Environment::getImage("IMAGE_BULLET");
	mStartTime = Boy::Environment::instance()->getTime();
	mDuration = 1.0f;
}

void Explosion::draw(Boy::Graphics *g)
{
	g->setColor(mColor);
	g->setColorizationEnabled(true); // required for alpha to affect rendering
	float alpha = 1 - (Boy::Environment::instance()->getTime() - mStartTime) / mDuration;
	g->setAlpha(alpha);
	g->setDrawMode(Boy::Graphics::DRAWMODE_ADDITIVE);

	for (int i=0 ; i<mNumParticles ; i++)
	{
		g->pushTransform();
			g->translate(
				mParticlePositions[i].x,
				mParticlePositions[i].y);
			g->drawImage(mImage);
		g->popTransform();
	}

	g->setColorizationEnabled(false);
	g->setDrawMode(Boy::Graphics::DRAWMODE_NORMAL);
}

void Explosion::update(float dt)
{
	// move particles:
	for (int i=0 ; i<mNumParticles ; i++)
	{
		mParticlePositions[i] += mParticleVelocities[i] * dt;
		mParticleVelocities[i] *= 1 - (2.7f * dt);
	}

	if( isOver() )
	{
		setDestroyed(true);
	}
}

bool Explosion::isOver()
{
	return Boy::Environment::instance()->getTime() - mStartTime > mDuration;
}
