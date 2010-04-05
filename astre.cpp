#include "astre.h"
#include <cmath>

#include <stdio.h>
#include <iostream>

using namespace std;

const float Astre::cG=6.67e-11;
const float Astre::dt=1;//10e4;
const float System::distMax=10000;//10e4;

Astre::Astre(){// : sys(system){
	//type=0;
	m=0;
	r=0;
	x=0;
	y=0;
	vx=0;
	vy=0;
	num=0;
	name="";
	fx=0;
	fy=0;
	//cout<<"size in Astre"<<sys->nb<<" "<<sys->astre.size()<<endl;
}

Astre::Astre(float _m, float _r, float _x, float _y, float _vx, float _vy, std::string _name){
	m=_m;
	r=_r;
	x=_x;
	y=_y;
	vx=_vx;
	vy=_vy;
	num=0;
	name=_name;
	fx=0;
	fy=0;
}

Astre::~Astre(){
}

Astre& Astre::operator=(const Astre& a){
//	type=a.type;
	m=a.m;
	r=a.r;
	x=a.r;
	y=a.y;
	vx=a.vx;
	vy=a.vy;
	name=a.name;
	fx=a.fx;
	fy=a.fy;
	
	//sys=a.sys;
	
	return *this;
}

void Astre::Move(){
	//printf("num=%d m=%f x=%f y=%f vx=%f vy=%f fx=%f fy=%f\n",num,m,x,y,vx,vy,fx,fy);
	
	x+= vx * dt;
	y+= vy * dt;
}

void Astre::ComputeForce(System& sys, int& nbCollisions){
//		cout<<"size in AstreCompspeed"<<sys.nb<<" "<<sys.astre.size()<<endl;
	nbCollisions=0;
	//Compute forces : use simetry of forces
	if(m>0)
	for(ARR<Astre>::iterator a=sys.astre.begin(); a != sys.astre.end(); a++){
		if(a->m >0 && a->num > num){
			//cout<<"AAA"<<sys.astre.size()<<" "<<num<<" vs "<<a->num<<endl;
			float ex= a->x - x;
			float ey= a->y - y;
			float sqdist= ex * ex + ey * ey;
			float dist=sqrt(sqdist);
			
			if(dist < r + a->r){
				//Collision
				nbCollisions++;
				//sys.nb--;
				if(a->m > m)
					Collision(*a, *this);
				else
					Collision(*this, *a);
			}
			else
			{
				ex /= dist;
				ey /= dist;
				float ffx = ex * m * a->m * cG / sqdist;
				float ffy = ey * m * a->m * cG / sqdist;
				fx += ffx;
				fy += ffy;
				a->fx -= ffx;
				a->fy -= ffy;
			}
		}
	}
//	cout<<"ax "<<ax<<"ay "<<ay<<endl;
}

void Astre::ComputeSpeed(){
	// Compute speed
	
	vx+= fx/m * dt;
	vy+= fy/m * dt;
}

void Astre::Collision(Astre& big, Astre& small){
	big.vx= (big.vx * big.m + small.vx * small.m)/(big.m + small.m);
	big.vy= (big.vy * big.m + small.vy * small.m)/(big.m + small.m);
	big.x= (big.x * big.m + small.x * small.m)/(big.m + small.m);
	big.y= (big.y * big.m + small.y * small.m)/(big.m + small.m);
	
	big.r=pow((big.m + small.m)/6e12, 0.333)*20;//pow((pow(big.r, 3)+ pow(small.r, 3)),0.3333);
	big.m+=small.m;
	
	small.m=0;
	small.r=0;
}

System::System(){
	cpt=0;
//	cout<<"size "<<astre.size()<<endl;
}
System::~System(){}

void System::AddAstre(Astre& a){
	cpt++;
	
	a.SetNumber(cpt);//astre.size();
	astre.push_back(a);
}

int System::NbAstre(){
	return astre.size();
}

int System::ComputeSpeed(){
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
	
	//if(nbCollision>0)cout<<"computespeed coll 2 "<<nbCollision<<endl;
	return nbCollision;
}

int System::Move(){
	
	for(ARR<Astre>::iterator a=astre.begin(); a != astre.end(); a++)
		a->Move();
	int removed=0;
	gx=0;
	gy=0;
	float m=0;
	
	// Compute mass centre
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
	
	ARR<Astre>::iterator aa = astre.begin();
	static float sqdist= distMax * distMax;
	while(aa != astre.end() )
		if(aa->m <= 0 || (aa->x - gx)*(aa->x - gx) + (aa->y - gy)*(aa->y - gy) > sqdist){
			aa = astre.erase(aa);
			removed++;
		}
		else aa++;
		
	return removed;
}

void System::GetBiggestAstrePosition(float& x, float& y)
{
	x=0;
	y=0;
	float m=0;
	for(ARR<Astre>::iterator a=astre.begin(); a != astre.end(); a++)
	{
		if(a->m > m){
			x =a->x;
			y= a->y;
			m= a->m;
		}
	}
}

void System::GetBorders(float& x1, float& y1, float& x2, float& y2)
{
	x1=y1=1e999;
	x2=y2=-1e999;
	
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

