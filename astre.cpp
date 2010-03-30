#include "astre.h"
#include <cmath>

#include <stdio.h>
#include <iostream>

using namespace std;

const float Astre::cG=6.67e-11;
const float Astre::dt=1;//10e4;

Astre::Astre(){// : sys(system){
	//sys=system;
	//num=sys->nb;
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
	printf("num=%d m=%f x=%f y=%f vx=%f vy=%f\n",num,m,x,y,vx,vy);
	
	x+= vx * dt;
	y+= vy * dt;
}

void Astre::ComputeSpeed(System sys){
//		cout<<"size in AstreCompspeed"<<sys.nb<<" "<<sys.astre.size()<<endl;

	float ax=0; // acceleration
	float ay=0;
//	for(unsigned int i=0; i < sys.astre.size(); i++){
	if(m>0)
	for(vector<Astre>::iterator a=sys.astre.begin(); a != sys.astre.end(); a++){
//		cout<<"AAA"<<sys.astre.size()<<" "<<sys.astre[i].num<<" "<<num<<endl;
		if(a->num!=num && a->m >0){
				float ex= a->x - x;
				float ey= a->y - y;
				float sqdist= ex * ex + ey * ey;
				float dist=sqrt(sqdist);
				if(dist/2 < r + a->r && num > a->num){
					//Collision
					if(a->m > m)
						Collision(*a, *this);
					else
						Collision(*this, *a);
				}
				else
				{
					ex/= sqrt(sqdist);
					ey/= sqrt(sqdist);
					ax += ex * a->m / sqdist;
					ay += ey * a->m / sqdist;
				}
		}
	}
	vx+=cG * ax * dt;
	vy+=cG * ay * dt;
//	cout<<"ax "<<ax<<"ay "<<ay<<endl;
}

void Astre::Collision(Astre& big, Astre& small){
	big.vx=0;// (big.vx * big.m + small.vx * small.m)/(big.m + small.m);
	big.vy=0;// (big.vy * big.m + small.vy * small.m)/(big.m + small.m);
	big.x= (big.x * big.m + small.x * small.m)/(big.m + small.m);
	big.y= (big.y * big.m + small.y * small.m)/(big.m + small.m);
	
	//big.r+=pow((pow(big.r, 3)+ pow(small.r, 3)),1/3);
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
	a.num=nb;
	astre.push_back(a);
}

void System::ComputeSpeed(){
	for(vector<Astre>::iterator a=astre.begin(); a != astre.end(); a++){
		a->ComputeSpeed(*this);
	}
}

void System::Move(){
	for(vector<Astre>::iterator a=astre.begin(); a != astre.end(); a++)
		a->Move();
}
