#pragma once

#include "CrtDbgInc.h"

#include <math.h>
#include <limits>
#include "BoyMath.h"

namespace BoyLib
{
	class Vector2
	{
	public:
		
		Vector2(float x=0, float y=0) { this->x = x; this->y = y; }
		Vector2(const Vector2 &vec) { x=vec.x; y=vec.y; }
		virtual ~Vector2() {}

		Vector2 operator+(const Vector2 &v) const { return Vector2(x+v.x, y+v.y); }
		Vector2 operator-(const Vector2 &v) const { return Vector2(x-v.x, y-v.y); }
		Vector2 operator-() const { return Vector2(-x, -y); }
		Vector2 operator*(float t) const { return Vector2(t*x, t*y); }
		Vector2 operator/(float t) const { return Vector2(x/t, y/t); }
		void operator+=(const Vector2 &v) { x+=v.x; y+=v.y; }
		void operator-=(const Vector2 &v) { x-=v.x; y-=v.y; }
		void operator*=(float t) { x*=t; y*=t; }
		void operator/=(float t) { x/=t; y/=t; }

		void operator=(const Vector2 &v) { x=v.x; y=v.y; }

		bool operator==(const Vector2 &v) { return x==v.x && y==v.y; }
		bool operator!=(const Vector2 &v) { return x!=v.x || y!=v.y; }

		float dot(const Vector2 &v) const { return x*v.x + y*v.y; }
		float magnitude() const { return boy_sqrtf(x*x + y*y); }
		float magnitudeSquared() const { return x*x+y*y; }
		
		//adjusts x and y so that the length of the vector does not exceed max
		inline void truncate(float max)
		{
		  if (this->magnitude() > max)
		  {
			*this = normalize() * max;
		  } 
		}

		Vector2 normalize() const { float aMag = magnitude(); return aMag!=0 ? (*this)/aMag : *this; }

		Vector2 perp() const { return Vector2(-y, x); }

		//sets x and y to zero
		void zero(){x=0.0; y=0.0;}
	public:

		float x;
		float y;
	};
};


//------------------------------------------------------------------------non member functions

inline BoyLib::Vector2 vec2DNormalize(const BoyLib::Vector2 &v)
{
  BoyLib::Vector2 vec = v;

  float vector_length = vec.magnitude();

  if (vector_length > std::numeric_limits<float>::epsilon())
  {
    vec.x /= vector_length;
    vec.y /= vector_length;
  }

  return vec;
}