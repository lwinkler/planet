
#ifndef ASTRE_H
#define ASTRE_H

#include <vector>


class Astre;

class System{

	public:
		std::vector<Astre> astre;
		System();
		~System();
		int ComputeSpeed();
		void Move();
		void AddAstre(Astre& a);
		int NbAstre();
		
		void GetGravityCenter(float& x, float& y);
		void GetBiggestAstrePosition(float& x, float& y);
		void GetBorders(float& x1, float& y1, float& x2, float& y2);
		int nb;
};

class Astre{
	private:
		//System* sys;
		static void Collision(Astre& big, Astre& Small);

	public:
		Astre();
		~Astre();
		Astre& operator=(const Astre& a);
		
		void ComputeForce(System& sys, int& nbCollision);
		void ComputeSpeed();
		void Move();
		
		inline static float SetInRange(float x, float min, float max){
			if(x < min)return min;
			if(x > max)return max;
			return x;
		}
		
		static const float cG;
		static const float dt;
		
		int type;
		int color;
		int num;
		float m;
		float r;
		float x;
		float y;
		float vx;
		float vy;
		float fx;
		float fy;

};


#endif