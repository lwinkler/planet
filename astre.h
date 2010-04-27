/****************************************************************************
**
** Simulator of a system of celestial objects
**
** Author : Laurent Winkler
** 
****************************************************************************/

#ifndef ASTRE_H
#define ASTRE_H

//#include <vector>
#include <list>
#include <string>
#include <cmath>

#define ARR list
#define DTYPE double

class Astre;

/// @class Universe The system containing all the celestial corpses

class Universe{

	public:
		static const DTYPE distMax;
		
		std::list<Astre> astre;
		Universe();
		~Universe();
		int ComputeSpeed();
		int Move();
		void AddAstre(Astre& a);
		int NbAstre();
		void Reset();
		
		void GetGravityCenter(DTYPE& _x, DTYPE& _y) {_x=gx; _y=gy;};
		void GetBiggestAstrePosition(DTYPE& x, DTYPE& y);
		void GetBorders(DTYPE& x1, DTYPE& y1, DTYPE& x2, DTYPE& y2);
		int FindAstreAtPosition(DTYPE x, DTYPE y, Astre& a);
	private:
		int cpt;
		DTYPE gx;	// Center of gravity
		DTYPE gy;
};

/// @class Astre A celestial corpse

class Astre{
	public:
		static const DTYPE cG;	// Gravity constant
		static const DTYPE dt;	// time interval between two iterations
		
	public:
		Astre();
		Astre(DTYPE m, DTYPE r, DTYPE x, DTYPE y, DTYPE vx, DTYPE vy, std::string name);

		~Astre();
		Astre& operator=(const Astre& a);
		
		void ComputeForce(Universe& sys, int& nbCollision);
		void ComputeSpeed();
		void Move();
		
		
		inline void GetPosition(DTYPE& _x, DTYPE& _y) { x=_x ; y=_y;};
		inline DTYPE GetMass() {return m;};
		inline DTYPE GetRadius() {return r;};
		
		inline std::string GetName() { return name; };
		inline void SetName(std::string _name) {name=_name; };
		inline int GetNumber() { return num; };
		inline void SetNumber(int _num) { num = _num; };
		inline void ResetForce() {fx=fy=0;};
		
		inline static DTYPE SetInRange(DTYPE x, DTYPE min, DTYPE max){
			if(x < min)return min;
			if(x > max)return max;
			return x;
		}
		inline static DTYPE MassFromRadius(DTYPE r){return r*r*r /125.;};
		inline static DTYPE RadiusFromMass(DTYPE m){return std::pow(m, 0.333) * 5.;};
	private:
		static void Collision(Astre& big, Astre& Small);

	public:
		std::string name;
		DTYPE m;	// mass
		DTYPE r; 	// radius
		DTYPE x;	// x position
		DTYPE y;	// y position
		DTYPE vx;	// x velocity
		DTYPE vy;	// y velocity
		DTYPE fx;	// x force
		DTYPE fy;	// y force
		//int type;
		//int color;
		int num;	// number or id
};


#endif