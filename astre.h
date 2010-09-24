/***************************************************************************
 *   Copyright (C) 2010                                                    *
 *   laurent.winkler@bluewin.ch                                            *
 *                                                                         *
 *   Simulator of planetary system                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef ASTRE_H
#define ASTRE_H

//#include <vector>
#include <list>
#include <string>
#include <cmath>

#define ARR list
#define DTYPE float

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
                void CreateRandom(int nbAstres, DTYPE posX, DTYPE posY, DTYPE distMax, DTYPE massMax, DTYPE radiusMax, DTYPE speedMax);
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