
#ifndef ASTRE_H
#define ASTRE_H

#include <vector>
#include <list>
#include <string>

#define ARR list

class Astre;

class System{

	public:
		std::list<Astre> astre;
		System();
		~System();
		int ComputeSpeed();
		int Move();
		void AddAstre(Astre& a);
		int NbAstre();
		
		void GetGravityCenter(float& _x, float& _y) {_x=gx; _y=gy;};
		void GetBiggestAstrePosition(float& x, float& y);
		void GetBorders(float& x1, float& y1, float& x2, float& y2);
	
		static const float distMax;
	private:
		int cpt;
		float gx;
		float gy;
};

class Astre{
	private:
		//System* sys;
		static void Collision(Astre& big, Astre& Small);

	public:
		Astre();
		Astre(float m, float r, float x, float y, float vx, float vy, std::string name);

		~Astre();
		Astre& operator=(const Astre& a);
		
		void ComputeForce(System& sys, int& nbCollision);
		void ComputeSpeed();
		void Move();
		
		
		inline void GetPosition(float& _x, float& _y){ x=_x ; y=_y;};
		inline float GetMass() {return m;};
		inline float GetRadius() {return r;};
		
		inline std::string GetName() { return name; };
		inline void SetName(std::string _name) {name=_name; };
		inline int GetNumber() { return num; };
		inline void SetNumber(int _num) { num = _num; };
		inline void ResetForce() {fx=fy=0;};
		
		inline static float SetInRange(float x, float min, float max){
			if(x < min)return min;
			if(x > max)return max;
			return x;
		}
		
		static const float cG;
		static const float dt;
		
		std::string name;
		float m;
		float r;
		float x;
		float y;
		float vx;
		float vy;
		float fx;
		float fy;
		//int type;
		//int color;
		int num;

};


#endif