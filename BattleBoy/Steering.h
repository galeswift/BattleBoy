#pragma once

class Unit;

class Steering
{
public:
	Steering(Unit* _owner) : owner(_owner) {};
	void update(float dt);
	BoyLib::Vector2 calculateWeightedSum();	
private:
	Unit* owner;
private:
	enum behavior_type
	{
		none               = 0x00000,
		seek               = 0x00002,
		flee               = 0x00004,
		arrive             = 0x00008,
		wander             = 0x00010,
		cohesion           = 0x00020,
		separation         = 0x00040,
		allignment         = 0x00080,
		obstacle_avoidance = 0x00100,
		wall_avoidance     = 0x00200,
		follow_path        = 0x00400,
		pursuit            = 0x00800,
		evade              = 0x01000,
		interpose          = 0x02000,
		hide               = 0x04000,
		flock              = 0x08000,
		offset_pursuit     = 0x10000,
	};
};