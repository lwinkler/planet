#include "astre.h"
#include <cmath>

#include <stdio.h>
#include <iostream>

using namespace std;

const float Astre::cG=6.67e-11;
const float Astre::dt=1;//10e4;

Astre::Astre(){// : sys(system){
	type=0;
	m=0;
	r=0;
	x=0;
	y=0;
	vx=0;
	vy=0;
	//cout<<"size in Astre"<<sys->nb<<" "<<sys->astre.size()<<endl;
}

Astre::~Astre(){
}

Astre& Astre::operator=(const Astre& a){
	type=a.type;
	m=a.m;
	r=a.r;
	x=a.r;
	y=a.y;
	vx=a.vx;
	vy=a.vy;
	
	//sys=a.sys;
	
	return *this;
}

void Astre::Move(){
	//printf("num=%d m=%f x=%f y=%f vx=%f vy=%f fx=%f fy=%f\n",num,m,x,y,vx,vy,fx,fy);
	
	x+= vx * dt;
	y+= vy * dt;
}

void Astre::ComputeForce(System& sys){
//		cout<<"size in AstreCompspeed"<<sys.nb<<" "<<sys.astre.size()<<endl;

	//Compute forces : use simetry of forces
	if(m>0)
	for(vector<Astre>::iterator a=sys.astre.begin(); a != sys.astre.end(); a++){
		if(a->m >0 && a->num > num){
			//cout<<"AAA"<<sys.astre.size()<<" "<<num<<" vs "<<a->num<<endl;
			float ex= a->x - x;
			float ey= a->y - y;
			float sqdist= ex * ex + ey * ey;
			float dist=sqrt(sqdist);
			
			if(dist < r + a->r){
				//Collision
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
	
	//big.r+=pow((pow(big.r, 3)+ pow(small.r, 3)),0.3333);
	big.m+=small.m;
	
	small.m=0;
	small.r=0;
}

System::System(){
	nb=0;
//	cout<<"size "<<astre.size()<<endl;
}
System::~System(){}

void System::AddAstre(Astre& a){
	nb++;
	a.num=astre.size();
	astre.push_back(a);
}

void System::ComputeSpeed(){
	for(vector<Astre>::iterator a=astre.begin(); a != astre.end(); a++){
		a->fx=0;
		a->fy=0;
	}
	
	for(vector<Astre>::iterator a=astre.begin(); a != astre.end(); a++){
		a->ComputeForce(*this);
	}
	
	for(vector<Astre>::iterator a=astre.begin(); a != astre.end(); a++){
		a->ComputeSpeed();
	}
}

void System::Move(){
	for(vector<Astre>::iterator a=astre.begin(); a != astre.end(); a++)
		a->Move();
}
