#include "Globals.h"

void Steering::update(float dt )
{
}

BoyLib::Vector2 Steering::calculateWeightedSum()
{        
  //if (On(wall_avoidance))
  //{
  //  m_vSteeringForce += WallAvoidance(m_pVehicle->World()->Walls()) *
  //                       m_dWeightWallAvoidance;
  //}
  // 
  //if (On(obstacle_avoidance))
  //{
  //  m_vSteeringForce += ObstacleAvoidance(m_pVehicle->World()->Obstacles()) * 
  //          m_dWeightObstacleAvoidance;
  //}

  //if (On(evade))
  //{
  //  assert(m_pTargetAgent1 && "Evade target not assigned");
  //  
  //  m_vSteeringForce += Evade(m_pTargetAgent1) * m_dWeightEvade;
  //}


  ////these next three can be combined for flocking behavior (wander is
  ////also a good behavior to add into this mix)
  //if (!isSpacePartitioningOn())
  //{
  //  if (On(separation))
  //  {
  //    m_vSteeringForce += Separation(m_pVehicle->World()->Agents()) * m_dWeightSeparation;
  //  }

  //  if (On(allignment))
  //  {
  //    m_vSteeringForce += Alignment(m_pVehicle->World()->Agents()) * m_dWeightAlignment;
  //  }

  //  if (On(cohesion))
  //  {
  //    m_vSteeringForce += Cohesion(m_pVehicle->World()->Agents()) * m_dWeightCohesion;
  //  }
  //}
  //else
  //{
  //  if (On(separation))
  //  {
  //    m_vSteeringForce += SeparationPlus(m_pVehicle->World()->Agents()) * m_dWeightSeparation;
  //  }

  //  if (On(allignment))
  //  {
  //    m_vSteeringForce += AlignmentPlus(m_pVehicle->World()->Agents()) * m_dWeightAlignment;
  //  }

  //  if (On(cohesion))
  //  {
  //    m_vSteeringForce += CohesionPlus(m_pVehicle->World()->Agents()) * m_dWeightCohesion;
  //  }
  //}


  //if (On(wander))
  //{
  //  m_vSteeringForce += Wander() * m_dWeightWander;
  //}

  /*if (On(seek))
  {
    m_vSteeringForce += Seek(m_pVehicle->World()->Crosshair()) * m_dWeightSeek;
  }*/

  //if (On(flee))
  //{
  //  m_vSteeringForce += Flee(m_pVehicle->World()->Crosshair()) * m_dWeightFlee;
  //}

  //if (On(arrive))
  //{
  //  m_vSteeringForce += Arrive(m_pVehicle->World()->Crosshair(), m_Deceleration) * m_dWeightArrive;
  //}

  //if (On(pursuit))
  //{
  //  assert(m_pTargetAgent1 && "pursuit target not assigned");

  //  m_vSteeringForce += Pursuit(m_pTargetAgent1) * m_dWeightPursuit;
  //}

  //if (On(offset_pursuit))
  //{
  //  assert (m_pTargetAgent1 && "pursuit target not assigned");
  //  assert (!m_vOffset.isZero() && "No offset assigned");

  //  m_vSteeringForce += OffsetPursuit(m_pTargetAgent1, m_vOffset) * m_dWeightOffsetPursuit;
  //}

  //if (On(interpose))
  //{
  //  assert (m_pTargetAgent1 && m_pTargetAgent2 && "Interpose agents not assigned");

  //  m_vSteeringForce += Interpose(m_pTargetAgent1, m_pTargetAgent2) * m_dWeightInterpose;
  //}

  //if (On(hide))
  //{
  //  assert(m_pTargetAgent1 && "Hide target not assigned");

  //  m_vSteeringForce += Hide(m_pTargetAgent1, m_pVehicle->World()->Obstacles()) * m_dWeightHide;
  //}

  //if (On(follow_path))
  //{
  //  m_vSteeringForce += FollowPath() * m_dWeightFollowPath;
  //}

//  m_vSteeringForce.Truncate(m_pVehicle->MaxForce());
 
 // return m_vSteeringForce;

return BoyLib::Vector2(0,0);
}

////------------------------------- Seek -----------------------------------
////
////  Given a target, this behavior returns a steering force which will
////  direct the agent towards the target
////------------------------------------------------------------------------
//Vector2D SteeringBehavior::Seek(Vector2D TargetPos)
//{
//  Vector2D DesiredVelocity = Vec2DNormalize(TargetPos - m_pVehicle->Pos())
//                            * m_pVehicle->MaxSpeed();
//
//  return (DesiredVelocity - m_pVehicle->Velocity());
//}

//
////--------------------------- Arrive -------------------------------------
////
////  This behavior is similar to seek but it attempts to arrive at the
////  target with a zero velocity
////------------------------------------------------------------------------
//Vector2D SteeringBehavior::Arrive(Vector2D     TargetPos,
//                                  Deceleration deceleration)
//{
//  Vector2D ToTarget = TargetPos - m_pVehicle->Pos();
//
//  //calculate the distance to the target
//  double dist = ToTarget.Length();
//
//  if (dist > 0)
//  {
//    //because Deceleration is enumerated as an int, this value is required
//    //to provide fine tweaking of the deceleration..
//    const double DecelerationTweaker = 0.3;
//
//    //calculate the speed required to reach the target given the desired
//    //deceleration
//    double speed =  dist / ((double)deceleration * DecelerationTweaker);     
//
//    //make sure the velocity does not exceed the max
//    speed = min(speed, m_pVehicle->MaxSpeed());
//
//    //from here proceed just like Seek except we don't need to normalize 
//    //the ToTarget vector because we have already gone to the trouble
//    //of calculating its length: dist. 
//    Vector2D DesiredVelocity =  ToTarget * speed / dist;
//
//    return (DesiredVelocity - m_pVehicle->Velocity());
//  }
//
//  return Vector2D(0,0);
//}
//
//
////------------------------------ Pursuit ---------------------------------
////
////  this behavior creates a force that steers the agent towards the 
////  evader
////------------------------------------------------------------------------
//Vector2D SteeringBehavior::Pursuit(const Vehicle* evader)
//{
//  //if the evader is ahead and facing the agent then we can just seek
//  //for the evader's current position.
//  Vector2D ToEvader = evader->Pos() - m_pVehicle->Pos();
//
//  double RelativeHeading = m_pVehicle->Heading().Dot(evader->Heading());
//
//  if ( (ToEvader.Dot(m_pVehicle->Heading()) > 0) &&  
//       (RelativeHeading < -0.95))  //acos(0.95)=18 degs
//  {
//    return Seek(evader->Pos());
//  }
//
//  //Not considered ahead so we predict where the evader will be.
// 
//  //the lookahead time is propotional to the distance between the evader
//  //and the pursuer; and is inversely proportional to the sum of the
//  //agent's velocities
//  double LookAheadTime = ToEvader.Length() / 
//                        (m_pVehicle->MaxSpeed() + evader->Speed());
//  
//  //now seek to the predicted future position of the evader
//  return Seek(evader->Pos() + evader->Velocity() * LookAheadTime);
//}
//
//
////----------------------------- Evade ------------------------------------
////
////  similar to pursuit except the agent Flees from the estimated future
////  position of the pursuer
////------------------------------------------------------------------------
//Vector2D SteeringBehavior::Evade(const Vehicle* pursuer)
//{
//  /* Not necessary to include the check for facing direction this time */
//
//  Vector2D ToPursuer = pursuer->Pos() - m_pVehicle->Pos();
//
//  //uncomment the following two lines to have Evade only consider pursuers 
//  //within a 'threat range'
//  const double ThreatRange = 100.0;
//  if (ToPursuer.LengthSq() > ThreatRange * ThreatRange) return Vector2D();
// 
//  //the lookahead time is propotional to the distance between the pursuer
//  //and the pursuer; and is inversely proportional to the sum of the
//  //agents' velocities
//  double LookAheadTime = ToPursuer.Length() / 
//                         (m_pVehicle->MaxSpeed() + pursuer->Speed());
//  
//  //now flee away from predicted future position of the pursuer
//  return Flee(pursuer->Pos() + pursuer->Velocity() * LookAheadTime);
//}
//
//
////--------------------------- Wander -------------------------------------
////
////  This behavior makes the agent wander about randomly
////------------------------------------------------------------------------
//Vector2D SteeringBehavior::Wander()
//{ 
//  //this behavior is dependent on the update rate, so this line must
//  //be included when using time independent framerate.
//  double JitterThisTimeSlice = m_dWanderJitter * m_pVehicle->TimeElapsed();
//
//  //first, add a small random vector to the target's position
//  m_vWanderTarget += Vector2D(RandomClamped() * JitterThisTimeSlice,
//                              RandomClamped() * JitterThisTimeSlice);
//
//  //reproject this new vector back on to a unit circle
//  m_vWanderTarget.Normalize();
//
//  //increase the length of the vector to the same as the radius
//  //of the wander circle
//  m_vWanderTarget *= m_dWanderRadius;
//
//  //move the target into a position WanderDist in front of the agent
//  Vector2D target = m_vWanderTarget + Vector2D(m_dWanderDistance, 0);
//
//  //project the target into world space
//  Vector2D Target = PointToWorldSpace(target,
//                                       m_pVehicle->Heading(),
//                                       m_pVehicle->Side(), 
//                                       m_pVehicle->Pos());
//
//  //and steer towards it
//  return Target - m_pVehicle->Pos(); 
//}
//
//
////---------------------- ObstacleAvoidance -------------------------------
////
////  Given a vector of CObstacles, this method returns a steering force
////  that will prevent the agent colliding with the closest obstacle
////------------------------------------------------------------------------
//Vector2D SteeringBehavior::ObstacleAvoidance(const std::vector<BaseGameEntity*>& obstacles)
//{
//  //the detection box length is proportional to the agent's velocity
//  m_dDBoxLength = Prm.MinDetectionBoxLength + 
//                  (m_pVehicle->Speed()/m_pVehicle->MaxSpeed()) *
//                  Prm.MinDetectionBoxLength;
//
//  //tag all obstacles within range of the box for processing
//  m_pVehicle->World()->TagObstaclesWithinViewRange(m_pVehicle, m_dDBoxLength);
//
//  //this will keep track of the closest intersecting obstacle (CIB)
//  BaseGameEntity* ClosestIntersectingObstacle = NULL;
// 
//  //this will be used to track the distance to the CIB
//  double DistToClosestIP = MaxDouble;
//
//  //this will record the transformed local coordinates of the CIB
//  Vector2D LocalPosOfClosestObstacle;
//
//  std::vector<BaseGameEntity*>::const_iterator curOb = obstacles.begin();
//
//  while(curOb != obstacles.end())
//  {
//    //if the obstacle has been tagged within range proceed
//    if ((*curOb)->IsTagged())
//    {
//      //calculate this obstacle's position in local space
//      Vector2D LocalPos = PointToLocalSpace((*curOb)->Pos(),
//                                             m_pVehicle->Heading(),
//                                             m_pVehicle->Side(),
//                                             m_pVehicle->Pos());
//
//      //if the local position has a negative x value then it must lay
//      //behind the agent. (in which case it can be ignored)
//      if (LocalPos.x >= 0)
//      {
//        //if the distance from the x axis to the object's position is less
//        //than its radius + half the width of the detection box then there
//        //is a potential intersection.
//        double ExpandedRadius = (*curOb)->BRadius() + m_pVehicle->BRadius();
//
//        if (fabs(LocalPos.y) < ExpandedRadius)
//        {
//          //now to do a line/circle intersection test. The center of the 
//          //circle is represented by (cX, cY). The intersection points are 
//          //given by the formula x = cX +/-sqrt(r^2-cY^2) for y=0. 
//          //We only need to look at the smallest positive value of x because
//          //that will be the closest point of intersection.
//          double cX = LocalPos.x;
//          double cY = LocalPos.y;
//          
//          //we only need to calculate the sqrt part of the above equation once
//          double SqrtPart = sqrt(ExpandedRadius*ExpandedRadius - cY*cY);
//
//          double ip = cX - SqrtPart;
//
//          if (ip <= 0.0)
//          {
//            ip = cX + SqrtPart;
//          }
//
//          //test to see if this is the closest so far. If it is keep a
//          //record of the obstacle and its local coordinates
//          if (ip < DistToClosestIP)
//          {
//            DistToClosestIP = ip;
//
//            ClosestIntersectingObstacle = *curOb;
//
//            LocalPosOfClosestObstacle = LocalPos;
//          }         
//        }
//      }
//    }
//
//    ++curOb;
//  }
//
//  //if we have found an intersecting obstacle, calculate a steering 
//  //force away from it
//  Vector2D SteeringForce;
//
//  if (ClosestIntersectingObstacle)
//  {
//    //the closer the agent is to an object, the stronger the 
//    //steering force should be
//    double multiplier = 1.0 + (m_dDBoxLength - LocalPosOfClosestObstacle.x) /
//                        m_dDBoxLength;
//
//    //calculate the lateral force
//    SteeringForce.y = (ClosestIntersectingObstacle->BRadius()-
//                       LocalPosOfClosestObstacle.y)  * multiplier;   
//
//    //apply a braking force proportional to the obstacles distance from
//    //the vehicle. 
//    const double BrakingWeight = 0.2;
//
//    SteeringForce.x = (ClosestIntersectingObstacle->BRadius() - 
//                       LocalPosOfClosestObstacle.x) * 
//                       BrakingWeight;
//  }
//
//  //finally, convert the steering vector from local to world space
//  return VectorToWorldSpace(SteeringForce,
//                            m_pVehicle->Heading(),
//                            m_pVehicle->Side());
//}
//
////--------------------------- WallAvoidance --------------------------------
////
////  This returns a steering force that will keep the agent away from any
////  walls it may encounter
////------------------------------------------------------------------------
//Vector2D SteeringBehavior::WallAvoidance(const std::vector<Wall2D>& walls)
//{
//  //the feelers are contained in a std::vector, m_Feelers
//  CreateFeelers();
//  
//  double DistToThisIP    = 0.0;
//  double DistToClosestIP = MaxDouble;
//
//  //this will hold an index into the vector of walls
//  int ClosestWall = -1;
//
//  Vector2D SteeringForce,
//            point,         //used for storing temporary info
//            ClosestPoint;  //holds the closest intersection point
//
//  //examine each feeler in turn
//  for (unsigned int flr=0; flr<m_Feelers.size(); ++flr)
//  {
//    //run through each wall checking for any intersection points
//    for (unsigned int w=0; w<walls.size(); ++w)
//    {
//      if (LineIntersection2D(m_pVehicle->Pos(),
//                             m_Feelers[flr],
//                             walls[w].From(),
//                             walls[w].To(),
//                             DistToThisIP,
//                             point))
//      {
//        //is this the closest found so far? If so keep a record
//        if (DistToThisIP < DistToClosestIP)
//        {
//          DistToClosestIP = DistToThisIP;
//
//          ClosestWall = w;
//
//          ClosestPoint = point;
//        }
//      }
//    }//next wall
//
//  
//    //if an intersection point has been detected, calculate a force  
//    //that will direct the agent away
//    if (ClosestWall >=0)
//    {
//      //calculate by what distance the projected position of the agent
//      //will overshoot the wall
//      Vector2D OverShoot = m_Feelers[flr] - ClosestPoint;
//
//      //create a force in the direction of the wall normal, with a 
//      //magnitude of the overshoot
//      SteeringForce = walls[ClosestWall].Normal() * OverShoot.Length();
//    }
//
//  }//next feeler
//
//  return SteeringForce;
//}
//
////------------------------------- CreateFeelers --------------------------
////
////  Creates the antenna utilized by WallAvoidance
////------------------------------------------------------------------------
//void SteeringBehavior::CreateFeelers()
//{
//  //feeler pointing straight in front
//  m_Feelers[0] = m_pVehicle->Pos() + m_dWallDetectionFeelerLength * m_pVehicle->Heading();
//
//  //feeler to left
//  Vector2D temp = m_pVehicle->Heading();
//  Vec2DRotateAroundOrigin(temp, HalfPi * 3.5f);
//  m_Feelers[1] = m_pVehicle->Pos() + m_dWallDetectionFeelerLength/2.0f * temp;
//
//  //feeler to right
//  temp = m_pVehicle->Heading();
//  Vec2DRotateAroundOrigin(temp, HalfPi * 0.5f);
//  m_Feelers[2] = m_pVehicle->Pos() + m_dWallDetectionFeelerLength/2.0f * temp;
//}
//
//
////---------------------------- Separation --------------------------------
////
//// this calculates a force repelling from the other neighbors
////------------------------------------------------------------------------
//Vector2D SteeringBehavior::Separation(const vector<Vehicle*> &neighbors)
//{  
//  Vector2D SteeringForce;
//
//  for (unsigned int a=0; a<neighbors.size(); ++a)
//  {
//    //make sure this agent isn't included in the calculations and that
//    //the agent being examined is close enough. ***also make sure it doesn't
//    //include the evade target ***
//    if((neighbors[a] != m_pVehicle) && neighbors[a]->IsTagged() &&
//      (neighbors[a] != m_pTargetAgent1))
//    {
//      Vector2D ToAgent = m_pVehicle->Pos() - neighbors[a]->Pos();
//
//      //scale the force inversely proportional to the agents distance  
//      //from its neighbor.
//      SteeringForce += Vec2DNormalize(ToAgent)/ToAgent.Length();
//    }
//  }
//
//  return SteeringForce;
//}
//
//
////---------------------------- Alignment ---------------------------------
////
////  returns a force that attempts to align this agents heading with that
////  of its neighbors
////------------------------------------------------------------------------
//Vector2D SteeringBehavior::Alignment(const vector<Vehicle*>& neighbors)
//{
//  //used to record the average heading of the neighbors
//  Vector2D AverageHeading;
//
//  //used to count the number of vehicles in the neighborhood
//  int    NeighborCount = 0;
//
//  //iterate through all the tagged vehicles and sum their heading vectors  
//  for (unsigned int a=0; a<neighbors.size(); ++a)
//  {
//    //make sure *this* agent isn't included in the calculations and that
//    //the agent being examined  is close enough ***also make sure it doesn't
//    //include any evade target ***
//    if((neighbors[a] != m_pVehicle) && neighbors[a]->IsTagged() &&
//      (neighbors[a] != m_pTargetAgent1))
//    {
//      AverageHeading += neighbors[a]->Heading();
//
//      ++NeighborCount;
//    }
//  }
//
//  //if the neighborhood contained one or more vehicles, average their
//  //heading vectors.
//  if (NeighborCount > 0)
//  {
//    AverageHeading /= (double)NeighborCount;
//
//    AverageHeading -= m_pVehicle->Heading();
//  }
//  
//  return AverageHeading;
//}
//
////-------------------------------- Cohesion ------------------------------
////
////  returns a steering force that attempts to move the agent towards the
////  center of mass of the agents in its immediate area
////------------------------------------------------------------------------
//Vector2D SteeringBehavior::Cohesion(const vector<Vehicle*> &neighbors)
//{
//  //first find the center of mass of all the agents
//  Vector2D CenterOfMass, SteeringForce;
//
//  int NeighborCount = 0;
//
//  //iterate through the neighbors and sum up all the position vectors
//  for (unsigned int a=0; a<neighbors.size(); ++a)
//  {
//    //make sure *this* agent isn't included in the calculations and that
//    //the agent being examined is close enough ***also make sure it doesn't
//    //include the evade target ***
//    if((neighbors[a] != m_pVehicle) && neighbors[a]->IsTagged() &&
//      (neighbors[a] != m_pTargetAgent1))
//    {
//      CenterOfMass += neighbors[a]->Pos();
//
//      ++NeighborCount;
//    }
//  }
//
//  if (NeighborCount > 0)
//  {
//    //the center of mass is the average of the sum of positions
//    CenterOfMass /= (double)NeighborCount;
//
//    //now seek towards that position
//    SteeringForce = Seek(CenterOfMass);
//  }
//
//  //the magnitude of cohesion is usually much larger than separation or
//  //allignment so it usually helps to normalize it.
//  return Vec2DNormalize(SteeringForce);
//}
//
//
///* NOTE: the next three behaviors are the same as the above three, except
//          that they use a cell-space partition to find the neighbors
//*/
//
//
////---------------------------- Separation --------------------------------
////
//// this calculates a force repelling from the other neighbors
////
////  USES SPACIAL PARTITIONING
////------------------------------------------------------------------------
//Vector2D SteeringBehavior::SeparationPlus(const vector<Vehicle*> &neighbors)
//{  
//  Vector2D SteeringForce;
//
//  //iterate through the neighbors and sum up all the position vectors
//  for (BaseGameEntity* pV = m_pVehicle->World()->CellSpace()->begin();
//                         !m_pVehicle->World()->CellSpace()->end();     
//                       pV = m_pVehicle->World()->CellSpace()->next())
//  {    
//    //make sure this agent isn't included in the calculations and that
//    //the agent being examined is close enough
//    if(pV != m_pVehicle)
//    {
//      Vector2D ToAgent = m_pVehicle->Pos() - pV->Pos();
//
//      //scale the force inversely proportional to the agents distance  
//      //from its neighbor.
//      SteeringForce += Vec2DNormalize(ToAgent)/ToAgent.Length();
//    }
//
//  }
//
//  return SteeringForce;
//}
////---------------------------- Alignment ---------------------------------
////
////  returns a force that attempts to align this agents heading with that
////  of its neighbors
////
////  USES SPACIAL PARTITIONING
////------------------------------------------------------------------------
//Vector2D SteeringBehavior::AlignmentPlus(const vector<Vehicle*> &neighbors)
//{
//  //This will record the average heading of the neighbors
//  Vector2D AverageHeading;
//
//  //This count the number of vehicles in the neighborhood
//  double    NeighborCount = 0.0;
//
//  //iterate through the neighbors and sum up all the position vectors
//  for (MovingEntity* pV = m_pVehicle->World()->CellSpace()->begin();
//                         !m_pVehicle->World()->CellSpace()->end();     
//                     pV = m_pVehicle->World()->CellSpace()->next())
//  {
//    //make sure *this* agent isn't included in the calculations and that
//    //the agent being examined  is close enough
//    if(pV != m_pVehicle)
//    {
//      AverageHeading += pV->Heading();
//
//      ++NeighborCount;
//    }
//
//  }
//
//  //if the neighborhood contained one or more vehicles, average their
//  //heading vectors.
//  if (NeighborCount > 0.0)
//  {
//    AverageHeading /= NeighborCount;
//
//    AverageHeading -= m_pVehicle->Heading();
//  }
//  
//  return AverageHeading;
//}
//
//
////-------------------------------- Cohesion ------------------------------
////
////  returns a steering force that attempts to move the agent towards the
////  center of mass of the agents in its immediate area
////
////  USES SPACIAL PARTITIONING
////------------------------------------------------------------------------
//Vector2D SteeringBehavior::CohesionPlus(const vector<Vehicle*> &neighbors)
//{
//  //first find the center of mass of all the agents
//  Vector2D CenterOfMass, SteeringForce;
//
//  int NeighborCount = 0;
//
//  //iterate through the neighbors and sum up all the position vectors
//  for (BaseGameEntity* pV = m_pVehicle->World()->CellSpace()->begin();
//                         !m_pVehicle->World()->CellSpace()->end();     
//                       pV = m_pVehicle->World()->CellSpace()->next())
//  {
//    //make sure *this* agent isn't included in the calculations and that
//    //the agent being examined is close enough
//    if(pV != m_pVehicle)
//    {
//      CenterOfMass += pV->Pos();
//
//      ++NeighborCount;
//    }
//  }
//
//  if (NeighborCount > 0)
//  {
//    //the center of mass is the average of the sum of positions
//    CenterOfMass /= (double)NeighborCount;
//
//    //now seek towards that position
//    SteeringForce = Seek(CenterOfMass);
//  }
//
//  //the magnitude of cohesion is usually much larger than separation or
//  //allignment so it usually helps to normalize it.
//  return Vec2DNormalize(SteeringForce);
//}
//
//
////--------------------------- Interpose ----------------------------------
////
////  Given two agents, this method returns a force that attempts to 
////  position the vehicle between them
////------------------------------------------------------------------------
//Vector2D SteeringBehavior::Interpose(const Vehicle* AgentA,
//                                     const Vehicle* AgentB)
//{
//  //first we need to figure out where the two agents are going to be at 
//  //time T in the future. This is approximated by determining the time
//  //taken to reach the mid way point at the current time at at max speed.
//  Vector2D MidPoint = (AgentA->Pos() + AgentB->Pos()) / 2.0;
//
//  double TimeToReachMidPoint = Vec2DDistance(m_pVehicle->Pos(), MidPoint) /
//                               m_pVehicle->MaxSpeed();
//
//  //now we have T, we assume that agent A and agent B will continue on a
//  //straight trajectory and extrapolate to get their future positions
//  Vector2D APos = AgentA->Pos() + AgentA->Velocity() * TimeToReachMidPoint;
//  Vector2D BPos = AgentB->Pos() + AgentB->Velocity() * TimeToReachMidPoint;
//
//  //calculate the mid point of these predicted positions
//  MidPoint = (APos + BPos) / 2.0;
//
//  //then steer to Arrive at it
//  return Arrive(MidPoint, fast);
//}
//
////--------------------------- Hide ---------------------------------------
////
////------------------------------------------------------------------------
//Vector2D SteeringBehavior::Hide(const Vehicle*           hunter,
//                                 const vector<BaseGameEntity*>& obstacles)
//{
//  double    DistToClosest = MaxDouble;
//  Vector2D BestHidingSpot;
//
//  std::vector<BaseGameEntity*>::const_iterator curOb = obstacles.begin();
//  std::vector<BaseGameEntity*>::const_iterator closest;
//
//  while(curOb != obstacles.end())
//  {
//    //calculate the position of the hiding spot for this obstacle
//    Vector2D HidingSpot = GetHidingPosition((*curOb)->Pos(),
//                                             (*curOb)->BRadius(),
//                                              hunter->Pos());
//            
//    //work in distance-squared space to find the closest hiding
//    //spot to the agent
//    double dist = Vec2DDistanceSq(HidingSpot, m_pVehicle->Pos());
//
//    if (dist < DistToClosest)
//    {
//      DistToClosest = dist;
//
//      BestHidingSpot = HidingSpot;
//
//      closest = curOb;
//    }  
//            
//    ++curOb;
//
//  }//end while
//  
//  //if no suitable obstacles found then Evade the hunter
//  if (DistToClosest == MaxFloat)
//  {
//    return Evade(hunter);
//  }
//      
//  //else use Arrive on the hiding spot
//  return Arrive(BestHidingSpot, fast);
//}
//
////------------------------- GetHidingPosition ----------------------------
////
////  Given the position of a hunter, and the position and radius of
////  an obstacle, this method calculates a position DistanceFromBoundary 
////  away from its bounding radius and directly opposite the hunter
////------------------------------------------------------------------------
//Vector2D SteeringBehavior::GetHidingPosition(const Vector2D& posOb,
//                                              const double     radiusOb,
//                                              const Vector2D& posHunter)
//{
//  //calculate how far away the agent is to be from the chosen obstacle's
//  //bounding radius
//  const double DistanceFromBoundary = 30.0;
//  double       DistAway    = radiusOb + DistanceFromBoundary;
//
//  //calculate the heading toward the object from the hunter
//  Vector2D ToOb = Vec2DNormalize(posOb - posHunter);
//  
//  //scale it to size and add to the obstacles position to get
//  //the hiding spot.
//  return (ToOb * DistAway) + posOb;
//}
//
//
////------------------------------- FollowPath -----------------------------
////
////  Given a series of Vector2Ds, this method produces a force that will
////  move the agent along the waypoints in order. The agent uses the
//// 'Seek' behavior to move to the next waypoint - unless it is the last
////  waypoint, in which case it 'Arrives'
////------------------------------------------------------------------------
//Vector2D SteeringBehavior::FollowPath()
//{ 
//  //move to next target if close enough to current target (working in
//  //distance squared space)
//  if(Vec2DDistanceSq(m_pPath->CurrentWaypoint(), m_pVehicle->Pos()) <
//     m_dWaypointSeekDistSq)
//  {
//    m_pPath->SetNextWaypoint();
//  }
//
//  if (!m_pPath->Finished())
//  {
//    return Seek(m_pPath->CurrentWaypoint());
//  }
//
//  else
//  {
//    return Arrive(m_pPath->CurrentWaypoint(), normal);
//  }
//}
//
////------------------------- Offset Pursuit -------------------------------
////
////  Produces a steering force that keeps a vehicle at a specified offset
////  from a leader vehicle
////------------------------------------------------------------------------
//Vector2D SteeringBehavior::OffsetPursuit(const Vehicle*  leader,
//                                          const Vector2D offset)
//{
//  //calculate the offset's position in world space
//  Vector2D WorldOffsetPos = PointToWorldSpace(offset,
//                                                  leader->Heading(),
//                                                  leader->Side(),
//                                                  leader->Pos());
//
//  Vector2D ToOffset = WorldOffsetPos - m_pVehicle->Pos();
//
//  //the lookahead time is propotional to the distance between the leader
//  //and the pursuer; and is inversely proportional to the sum of both
//  //agent's velocities
//  double LookAheadTime = ToOffset.Length() / 
//                        (m_pVehicle->MaxSpeed() + leader->Speed());
//  
//  //now Arrive at the predicted future position of the offset
//  return Arrive(WorldOffsetPos + leader->Velocity() * LookAheadTime, fast);
//}
//
