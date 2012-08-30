#pragma once

class Unit;

class Steering
{
private:
	enum behavior_type
	{
		NONE               = 0X00000,
		SEEK               = 0X00002,
		FLEE               = 0X00004,
		ARRIVE             = 0X00008,
		WANDER             = 0X00010,
		COHESION           = 0X00020,
		SEPARATION         = 0X00040,
		ALLIGNMENT         = 0X00080,
		OBSTACLE_AVOIDANCE = 0X00100,
		WALL_AVOIDANCE     = 0X00200,
		FOLLOW_PATH        = 0X00400,
		PURSUIT            = 0X00800,
		EVADE              = 0X01000,
		INTERPOSE          = 0X02000,
		HIDE               = 0X04000,
		FLOCK              = 0X08000,
		OFFSET_PURSUIT     = 0X10000,
	};
	//Arrive makes use of these to determine how quickly a vehicle
	//should decelerate to its target
	enum Deceleration{slow = 3, normal = 2, fast = 1};

public:
	Steering(Unit* _owner) : owner(_owner), m_iFlags(0), m_dMultSeparation(100), m_bTagged(false) {};
	void update(float dt);
	BoyLib::Vector2 sumForces();	
	BoyLib::Vector2 seek(const BoyLib::Vector2& target);
	BoyLib::Vector2 arrive(BoyLib::Vector2 target, Deceleration decel);
	BoyLib::Vector2 separation();

	bool accumulateForce(BoyLib::Vector2 &sf, BoyLib::Vector2 forceToAdd);

	//this function tests if a specific bit of m_iFlags is set
	bool on(behavior_type bt){return (m_iFlags & bt) == bt;}
	
	BoyLib::Vector2  target()const{return m_vTarget;}
	void setTarget(const BoyLib::Vector2 t){m_vTarget = t;}
	void findNeighbours();

	// Steering functions
	void seekOn(){m_iFlags |= SEEK;}
	void seekOff()  {if(on(SEEK))   m_iFlags ^=SEEK;}
	void arriveOn(){m_iFlags |= ARRIVE;}
	void arriveOff(){if(on(ARRIVE)) m_iFlags ^=ARRIVE;}
	void separationOn(){m_iFlags |= SEPARATION;}
	void separationOff(){if(on(SEPARATION)) m_iFlags ^=SEPARATION;}

	bool      tagged()const{return m_bTagged;}
	void      tag(){m_bTagged = true;}
	void      unTag(){m_bTagged = false;}

private:
	Unit* owner;
	
	//binary flags to indicate whether or not a behavior should be active
	int           m_iFlags;

	float		  m_dMultSeparation;
	//the current target 
	BoyLib::Vector2 m_vTarget;

	//the steering force created by the combined effect of all
	//the selected behaviors
	BoyLib::Vector2 m_vSteeringForce;
	
	//used by group behaviors to tag neighbours
	bool         m_bTagged;
};