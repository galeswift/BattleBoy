#include "Globals.h"

void Steering::update(float dt )
{                                                      
	//reset the force
	m_vSteeringForce.zero();

	//this will hold the value of each individual steering force
	m_vSteeringForce = sumForces();

	//make sure the force doesn't exceed the vehicles maximum allowable
	m_vSteeringForce.truncate(owner->getMaxAccel());

	// Update our acceleration
	owner->setVel(m_vSteeringForce);
}


//--------------------- accumulateForce ----------------------------------
//
//  This function calculates how much of its max steering force the 
//  vehicle has left to apply and then applies that amount of the
//  force to add.
//------------------------------------------------------------------------
bool Steering::accumulateForce(BoyLib::Vector2 &sf, BoyLib::Vector2 forceToAdd)
{
	//first calculate how much steering force we have left to use
	float magnitudeSoFar = sf.magnitude();

	float magnitudeRemaining = owner->getMaxAccel() - magnitudeSoFar;

	//return false if there is no more force left to use
	if (magnitudeRemaining <= 0.0) return false;

	//calculate the magnitude of the force we want to add
	float magnitudeToAdd = forceToAdd.magnitude();

	//now calculate how much of the force we can really add  
	if (magnitudeToAdd > magnitudeRemaining)
	{
	magnitudeToAdd = magnitudeRemaining;
	}

	//add it to the steering force
	sf += (vec2DNormalize(forceToAdd) * magnitudeToAdd); 

	return true;
}

//-------------------------- SumForces -----------------------------------
//
//  this method calls each active steering behavior and acumulates their
//  forces until the max steering force magnitude is reached at which
//  time the function returns the steering force accumulated to that 
//  point
//------------------------------------------------------------------------
BoyLib::Vector2 Steering::sumForces()
{
	BoyLib::Vector2 force;

	//the soccer players must always tag their neighbors
	findNeighbours();

	if (on(SEPARATION))
	{
	force += separation() * m_dMultSeparation;

	if (!accumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}    

	if (on(SEEK))
	{
	force += seek(m_vTarget);

	if (!accumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (on(ARRIVE))
	{
	force += arrive(m_vTarget, fast);

	if (!accumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	//if (On(pursuit))
	//{
	//force += Pursuit(m_pBall);

	//if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	//}

	//if (On(interpose))
	//{
	//force += Interpose(m_pBall, m_vTarget, m_dInterposeDist);

	//if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	//}

	return m_vSteeringForce;
}


//------------------------------- Seek -----------------------------------
//
//  Given a target, this behavior returns a steering force which will
//  allign the agent with the target and move the agent in the desired
//  direction
//------------------------------------------------------------------------
BoyLib::Vector2 Steering::seek(const BoyLib::Vector2& target)
{
 
  BoyLib::Vector2 desiredVelocity = vec2DNormalize(target - owner->getPos())
                            * owner->getMaxSpeed();

  return (desiredVelocity - owner->getVel());
}


//--------------------------- Arrive -------------------------------------
//
//  This behavior is similar to seek but it attempts to arrive at the
//  target with a zero velocity
//------------------------------------------------------------------------
BoyLib::Vector2 Steering::arrive(BoyLib::Vector2    target,
                                   Deceleration deceleration)
{
  BoyLib::Vector2 toTarget = target - owner->getPos();

  //calculate the distance to the target
  float dist = toTarget.magnitude();

  if (dist > 0)
  {
    //because Deceleration is enumerated as an int, this value is required
    //to provide fine tweaking of the deceleration..
	const float decelerationTweaker = 3.0f;
	const float slowingdistance = owner->getMaxSpeed();
    //calculate the speed required to reach the target given the desired
    //deceleration
	float speed =  dist / ((float)deceleration * decelerationTweaker);                    

	if( owner->getMaxSpeed() < speed )
	{
		speed = owner->getMaxSpeed();
	}
	
	//from here proceed just like Seek except we don't need to normalize 
    //the ToTarget vector because we have already gone to the trouble
    //of calculating its length: dist. 
    BoyLib::Vector2 desiredVelocity =  toTarget * speed / dist;

	return (desiredVelocity - owner->getVel());
  }

  return BoyLib::Vector2(0,0);
}


//-------------------------- FindNeighbours ------------------------------
//
//  tags any vehicles within a predefined radius
//------------------------------------------------------------------------
void Steering::findNeighbours()
{
	if( !owner || !owner->getGame()) 
	{
		return;
	}

  std::vector<Actor*>& allActors = owner->getGame()->getActors();
  std::vector<Actor*>::iterator curActor;
  for (curActor = allActors.begin(); curActor!=allActors.end(); ++curActor)
  {
	Unit* curUnit = dynamic_cast<Unit*>(*curActor);

	if( curUnit != NULL )
	{
		//first clear any current tag
		(*curUnit).getSteering()->unTag();

		//work in distance squared to avoid sqrts
		BoyLib::Vector2 to = (*curUnit).getPos() - owner->getPos();

		if (to.magnitudeSquared() < (30.0f * 30.0f))
		{
		  (*curUnit).getSteering()->tag();
		}
	}
  }//next
}

//---------------------------- Separation --------------------------------
//
// this calculates a force repelling from the other neighbors
//------------------------------------------------------------------------
BoyLib::Vector2 Steering::separation()
{  
   //iterate through all the neighbors and calculate the vector from the
  BoyLib::Vector2 steeringForce;
  
  std::vector<Actor*>& allActors = owner->getGame()->getActors();
  std::vector<Actor*>::iterator curActor;
  for (curActor = allActors.begin(); curActor!=allActors.end(); ++curActor)
  {
	Unit* curUnit = dynamic_cast<Unit*>(*curActor);

	if( curUnit != NULL )
	{
		//make sure this agent isn't included in the calculations and that
		//the agent is close enough
		if((curUnit != owner) && (*curUnit).getSteering()->tagged())
		{
		  BoyLib::Vector2 toAgent = owner->getPos() - (*curUnit).getPos();

		  //scale the force inversely proportional to the agents distance  
		  //from its neighbor.
		  steeringForce += vec2DNormalize(toAgent)/toAgent.magnitude();
		}
	}
  }

  return steeringForce;
}