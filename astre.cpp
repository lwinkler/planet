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

#include "astre.h"
#include <cmath>

#include <stdio.h>
#include <cstdlib> 
#include <iostream>

using namespace std;

const DTYPE Astre::cG=6.67e1;//6.67e-11;
const DTYPE Astre::dt=1;//10e4;
const DTYPE Universe::distMax=10000;//10e4;

/* -------------------------------------------------------------------- */
/// Method for the Astre class
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/// Empty constructor
/* -------------------------------------------------------------------- */

Astre::Astre(){
	//type=0;
	m=0;
	r=0;
	x=0;
	y=0;
	vx=0;
	vy=0;
	num=-1;
	name="";
	fx=0;
	fy=0;
}

/* -------------------------------------------------------------------- */
/// Constructor
/* -------------------------------------------------------------------- */

Astre::Astre(DTYPE _m, DTYPE _r, DTYPE _x, DTYPE _y, DTYPE _vx, DTYPE _vy, std::string _name){
	m=_m;
	r=_r;
	x=_x;
	y=_y;
	vx=_vx;
	vy=_vy;
	num=-1;
	name=_name;
	fx=0;
	fy=0;
}

/* -------------------------------------------------------------------- */
/// Destructor
/* -------------------------------------------------------------------- */


Astre::~Astre(){
}

/* -------------------------------------------------------------------- */
/// Copy constructor
/* -------------------------------------------------------------------- */

Astre& Astre::operator=(const Astre& a){
//	type=a.type;
	m=a.m;
	r=a.r;
	x=a.x;
	y=a.y;
	vx=a.vx;
	vy=a.vy;
	//num=-1;//a.num;
	name=a.name;
	fx=a.fx;
	fy=a.fy;
	
	//sys=a.sys;
	
	return *this;
}

/* -------------------------------------------------------------------- */
/// Move the corpse
/* -------------------------------------------------------------------- */

void Astre::Move(){
	//printf("num=%d m=%f x=%f y=%f vx=%f vy=%f fx=%f fy=%f\n",num,m,x,y,vx,vy,fx,fy);
	
	x+= vx * dt;
	y+= vy * dt;
}

/* -------------------------------------------------------------------- */
/// Compute the forces applying to a corpse
/* -------------------------------------------------------------------- */

void Astre::ComputeForce(Universe& sys, int& nbCollisions){
	nbCollisions=0;

	//Compute forces : use simetry of forces
	if(m>0)
	for(ARR<Astre>::iterator a=sys.astre.begin(); a != sys.astre.end(); a++){
		if(a->m >0 && a->num > num){
			DTYPE ex= a->x - x;
			DTYPE ey= a->y - y;
			DTYPE sqdist= ex * ex + ey * ey;
			DTYPE dist= sqrt(sqdist);
			
			if(dist < r + a->r){
				//Collision
				nbCollisions++;
				if(a->m > m)
					Collision(*a, *this);
				else
					Collision(*this, *a);
			}
			else
			{
				ex /= dist;
				ey /= dist;
				DTYPE ffx = ex * m * a->m * cG / sqdist;
				DTYPE ffy = ey * m * a->m * cG / sqdist;
				fx += ffx;
				fy += ffy;
				a->fx -= ffx;
				a->fy -= ffy;
			}
		}
	}
//	cout<<"ax "<<ax<<"ay "<<ay<<endl;
}

/* -------------------------------------------------------------------- */
///Compute the speed given the force
/* -------------------------------------------------------------------- */

void Astre::ComputeSpeed(){
	// Compute speed
	
	vx+= fx/m * dt;
	vy+= fy/m * dt;
}

/* -------------------------------------------------------------------- */
/// Collision between two corpses
/* -------------------------------------------------------------------- */

void Astre::Collision(Astre& big, Astre& small){
	big.vx= (big.vx * big.m + small.vx * small.m)/(big.m + small.m);
	big.vy= (big.vy * big.m + small.vy * small.m)/(big.m + small.m);
	big.x= (big.x * big.m + small.x * small.m)/(big.m + small.m);
	big.y= (big.y * big.m + small.y * small.m)/(big.m + small.m);
	
	big.r=pow((pow(big.r, 3)+ pow(small.r, 3)),0.3333);//pow((big.m + small.m)/6e12, 0.333)*20;//
	big.m+=small.m;
	
	small.m=0;
	small.r=0;
}

/* -------------------------------------------------------------------- */
/// Method for the Universe class
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/// Constructor
/* -------------------------------------------------------------------- */

Universe::Universe(){
       	srand((unsigned)time(NULL));

	cpt=0;
//	cout<<"size "<<astre.size()<<endl;
}

/* -------------------------------------------------------------------- */
/// Destructor
/* -------------------------------------------------------------------- */

Universe::~Universe(){}

/* -------------------------------------------------------------------- */
/// Add an astre
/* -------------------------------------------------------------------- */
void Universe::AddAstre(Astre& a){
	cpt++;
	
	a.SetNumber(cpt);//astre.size();
	astre.push_back(a);
}

/* -------------------------------------------------------------------- */
/// Get astre number
/* -------------------------------------------------------------------- */

int Universe::NbAstre(){
	return astre.size();
}

/* -------------------------------------------------------------------- */
/// Reset universe
/* -------------------------------------------------------------------- */

void Universe::Reset(){
	astre.resize(0);
	cpt=0;
}

/* -------------------------------------------------------------------- */
/// Compute the speed for the whole universe
/* -------------------------------------------------------------------- */

int Universe::ComputeSpeed(){
	int nbCollision=0;
	for(ARR<Astre>::iterator a=astre.begin(); a != astre.end(); a++){
		a->ResetForce();
	}
	
	for(ARR<Astre>::iterator a=astre.begin(); a != astre.end(); a++){
		int nbColl1;
		a->ComputeForce(*this, nbColl1);
		nbCollision+=nbColl1;
	}
	
	for(ARR<Astre>::iterator a=astre.begin(); a != astre.end(); a++){
		a->ComputeSpeed();
	}
	
	return nbCollision;
}

/* -------------------------------------------------------------------- */
/// Move all celestial corpses, 1 iteration
/* -------------------------------------------------------------------- */

int Universe::Move(){
	
	for(ARR<Astre>::iterator a=astre.begin(); a != astre.end(); a++)
		a->Move();
	int removed=0;
	gx=0;
	gy=0;
	DTYPE m=0;
	
	// Compute and update mass centre
	for(ARR<Astre>::iterator a=astre.begin(); a != astre.end(); a++)
	{
		if(a->m >0){
			gx+= a->m * a->x;
			gy+= a->m * a->y;
			m+= a->m;
		}
	}
	gx/=m;
	gy/=m;
	
	// Clean astres
	ARR<Astre>::iterator aa = astre.begin();
	static DTYPE sqdist= distMax * distMax;
	while(aa != astre.end() )
		if(aa->m <= 0 || (aa->x - gx)*(aa->x - gx) + (aa->y - gy)*(aa->y - gy) > sqdist){
			aa = astre.erase(aa);
			removed++;
		}
		else aa++;
		
	return removed;
}

/* -------------------------------------------------------------------- */
/// Get the position of the biggest astre
/* -------------------------------------------------------------------- */
void Universe::GetBiggestAstrePosition(DTYPE& x, DTYPE& y)
{
	x=0;
	y=0;
	DTYPE m=0;
	for(ARR<Astre>::iterator a=astre.begin(); a != astre.end(); a++)
	{
		if(a->m > m){
			x = a->x;
			y = a->y;
			m = a->m;
		}
	}
}
/* -------------------------------------------------------------------- */
/// Get the borders of universe (most distant planets)
/* -------------------------------------------------------------------- */

void Universe::GetBorders(DTYPE& x1, DTYPE& y1, DTYPE& x2, DTYPE& y2)
{
	x1=y1=1e99;
	x2=y2=-1e99;
	
	for(ARR<Astre>::iterator a=astre.begin(); a != astre.end(); a++)
	{
		if(a->m > 0){
			if(a->x< x1) x1 =a->x;
			if(a->x > x2) x2 =a->x;
			if(a->y < y1) y1 =a->y;
			if(a->y > y2) y2 =a->y;
		}
	}
}

/* -------------------------------------------------------------------- */
/// find a celestial object from its position
/* -------------------------------------------------------------------- */

int Universe::FindAstreAtPosition(DTYPE x, DTYPE y, Astre& found){
	
	for(ARR<Astre>::iterator a=astre.begin(); a != astre.end(); a++)
	{
		if(a->m > 0){
			if((a->x - x)*(a->x - x) + (a->y - y) * (a->y - y) < a->r * a->r){
				found = *a;
				return a->num;
			}
		}
	}
	return -1;
	
}

/* -------------------------------------------------------------------- */
/// create a set of random planets
/* -------------------------------------------------------------------- */
#define RAND() (((double)rand()) / RAND_MAX)

void Universe::CreateRandom(int nbAstres, DTYPE posX, DTYPE posY, DTYPE distMax, DTYPE massMax, DTYPE radiusMax, DTYPE speedMax){
	static int cptAstre = 0;

	for(int i =0; i < nbAstres; i++)
	{
		char name[32];
		sprintf(name, "random planet %d", cptAstre);
		Astre a(RAND() * massMax , RAND() * radiusMax, posX + (RAND()-0.5) * distMax ,
                    posY + (RAND()-0.5) * distMax, (RAND()-0.5) * speedMax, (RAND()-0.5) * speedMax, name);
		AddAstre(a);
		cptAstre++;
	}
}
