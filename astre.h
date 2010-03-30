
#ifndef ASTRE_H
#define ASTRE_H

#include <vector>


class Astre;
class System{

	public:
		std::vector<Astre> astre;
		System();
		~System();
		void ComputeSpeed();
		void Move();
		void AddAstre(Astre& a);
		
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
		
		void ComputeSpeed(System sys);
		void Move();
		
		static const float cG;
		static const float dt;
		
		int type;
		int num;
		float m;
		float r;
		float x;
		float y;
		float vx;
		float vy;
	
};


#endif