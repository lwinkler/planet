
#ifndef ASTRE_H
#define ASTRE_H

#include <vector>
#include <list>
#include <string>
#include <cmath>

#define ARR list
#define DTYPE double

class Astre;

class Universe{

	public:
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
		static const DTYPE distMax;
	private:
		int cpt;
		DTYPE gx;
		DTYPE gy;
};

class Astre{
	private:
		//Universe* sys;
		static void Collision(Astre& big, Astre& Small);

	public:
		Astre();
		Astre(DTYPE m, DTYPE r, DTYPE x, DTYPE y, DTYPE vx, DTYPE vy, std::string name);

		~Astre();
		Astre& operator=(const Astre& a);
		
		void ComputeForce(Universe& sys, int& nbCollision);
		void ComputeSpeed();
		void Move();
		
		
		inline void GetPosition(DTYPE& _x, DTYPE& _y){ x=_x ; y=_y;};
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
		
		static const DTYPE cG;
		static const DTYPE dt;
		
		std::string name;
		DTYPE m;
		DTYPE r;
		DTYPE x;
		DTYPE y;
		DTYPE vx;
		DTYPE vy;
		DTYPE fx;
		DTYPE fy;
		//int type;
		//int color;
		int num;

};


#endif