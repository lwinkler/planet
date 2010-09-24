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

#include <QDateTime>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

#include <QApplication>
#include <QTransform>
#include <math.h>
#include <stdlib.h>

#include "astre.h"
#include "astrefield.h"

#include <iostream>
#include <stdio.h>
#include <time.h>

#define MIN(a,b) ((a)<(b))?(a):(b)
#define MAX(a,b) ((a)>(b))?(a):(b)

using namespace std;

const int AstreField::timerInterval=20;
const Qt::GlobalColor AstreField::colors[15]={
	Qt::red,
	Qt::darkRed,
	Qt::green,
	Qt::darkGreen,
	Qt::blue,
	Qt::darkBlue,
	Qt::cyan,
	Qt::darkCyan,
	Qt::magenta,
	Qt::darkMagenta,
	Qt::yellow,
	Qt::darkYellow,
	Qt::gray,
	Qt::darkGray,
	Qt::lightGray
};

AstreField::AstreField(QWidget *parent)
: QWidget(parent)
{
	//currentAngle = 45;
	dispTimer = new QTimer(this);
	connect(dispTimer, SIGNAL(timeout()), this, SLOT(moveSys()));
	
	setPalette(QPalette(QColor(0, 0, 0)));
	setAutoFillBackground(true);

	dispScale=1;
	dispCenter=QPointF(0,0);
	
	init();
}

AstreField::~AstreField(){
}


void AstreField::pauseSimulation()
{
	if(dispTimer->isActive())dispTimer->stop();
	else dispTimer->start(timerInterval);
}

void AstreField::changeViewCenter(int index)
{
	centerView = index - 3;
	centerViewAstre = sys.astre.begin();
	if(centerView >= 0)
		for(int i=0; i<centerView;i++)
			centerViewAstre++;
		
	//cout<<"change view center"<<index<<" "<<centerView<<endl;
	update();
}

void AstreField::changeViewScale(int index)
{
	scaleView = index;
	dispScale = scaleView / 100.0;
	update();
}

void AstreField::moveSys()
{
	static clock_t startTime = clock();
	//QRegion region = QRect();
	//cout<<"moveSys"<<endl;
	++timerCount;
	
	int nbCollision = sys.ComputeSpeed();
	//setUpdatesEnabled(false);
	int nbRemoved = sys.Move();
	if(nbCollision > 0 || nbRemoved > 0)
		emit nbAstreChanged(sys.NbAstre());
	for(ARR<Astre>::iterator a=sys.astre.begin(); a != sys.astre.end(); a++){
		
		QRect astreRect = QRect(-a->r/2 , a->r/2, -a->r/2 , a->r/2);
		astreRect.moveCenter(QPoint(a->x, a->y));
		/*if (shotR.intersects(targetRect())) {
			autoShootTimer->stop();
			emit hit();
			emit canShoot(true);
		} else if (shotR.x() > width() || shotR.y() > height()
			|| shotR.intersects(barrierRect())) {
			autoShootTimer->stop();
		emit missed();
		emit canShoot(true);
		} else {*/
		//	region = region.unite(astreRect);
		//}
		//update(region);
	}
	steps++;
	
	if(steps % 10 == 0)
	{
		clock_t stopTime = clock();
		perf = ( (double)(stopTime - startTime)) / (10. * CLOCKS_PER_SEC);
		//sumPerf = 0;
		startTime = clock();
		emit systemMoved(0);		
	}

	update();	
}

void AstreField::mousePressEvent(QMouseEvent *event)
{
	QPoint pos = event->pos();	
	QTransform matrix;
	matrix.scale(dispScale, dispScale);
	matrix.translate(-dispCenter.x(), -dispCenter.y());
	matrix.translate(width()/2/dispScale , height()/2/dispScale);
	QTransform invmat;
	invmat  = matrix.inverted();
	QPointF posSys = pos * invmat;

	if (event->button() == Qt::RightButton)
	{
		// Add a planet
		
		newAstre_x=posSys.x();
		newAstre_y=posSys.y();
		
		timerCountNewAstre = timerCount;
	}
	else if (event->button() == Qt::LeftButton)
	{
		// Select a planet for properties
		Astre astre; 
		int num = sys.FindAstreAtPosition(posSys.x(), posSys.y(), astre);
		emit astreSelected(num, astre);
		
	}
}

/*void AstreField::mouseMoveEvent(QMouseEvent *event)
{
	if (!barrelPressed)
		return;
	QPoint pos = event->pos();
	if (pos.x() <= 0)
		pos.setX(1);
	if (pos.y() >= height())
		pos.setY(height() - 1);
	double rad = atan(((double)rect().bottom() - pos.y()) / pos.x());
	setAngle(qRound(rad * 180 / 3.14159265));
}*/

void AstreField::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() != Qt::RightButton)
		return;
	QPoint pos = event->pos();
	
	QTransform matrix;
	matrix.scale(dispScale, dispScale);
	matrix.translate(-dispCenter.x(), -dispCenter.y());
	matrix.translate(width()/2/dispScale , height()/2/dispScale);
	QTransform invmat;
	invmat  = matrix.inverted();
	QPointF posSys = pos * invmat;
	timerCountNewAstre = timerCount - timerCountNewAstre;
	
	DTYPE vx= (posSys.x() - newAstre_x) / (timerCountNewAstre * Astre::dt);
	DTYPE vy= (posSys.y() - newAstre_y) / (timerCountNewAstre * Astre::dt);
	
	if(timerCountNewAstre==0)vx=vy=0; // If simulation stopped
	
	DTYPE m=6.;
	DTYPE r=Astre::RadiusFromMass(m);
	char name[32];
	sprintf(name, "planet %d", cptAstre);
	Astre newAstre(m, r, posSys.x(), posSys.y(), vx, vy, name);
	sys.AddAstre(newAstre);
	cptAstre++;
	newAstre.m=0;
	
	emit nbAstreChanged(sys.NbAstre());
	update();

}

void AstreField::paintEvent(QPaintEvent * /* event */)
{
	QPainter painter(this);
	painter.setBackground(Qt::black);
	painter.setBrush(Qt::black);
	//painter.drawRect(QRect(0, 0, width(), height()));
	
	DTYPE x = 0;
	DTYPE y = 0;
	
	if(centerView== -3)
	{
		sys.GetGravityCenter(x,y);
		dispCenter = QPointF(x,y);
	}
	else if(centerView== -2)
	{
		sys.GetBiggestAstrePosition(x,y);
		dispCenter = QPointF(x,y);
	}
	else if(centerView== -1)
	{
		//DTYPE x1,y1,x2,y2;
		/*sys.GetBorders(x1,y1,x2,y2);
		dispCenter = QPointF((x1+x2)/2,(y1+y2)/2);
		dispScale = Astre::SetInRange(MIN(width()/(x2-x1), height()/(y2-y1)), 0.01, 1);*/
		sys.GetGravityCenter(x,y);
		dispCenter = QPointF(x,y);
		dispScale = Astre::SetInRange(MIN(width()/Universe::distMax/2, height()/Universe::distMax/2), 0.01, 1);
	}else {
		if(centerViewAstre != (ARR<Astre>::iterator) NULL && centerViewAstre->m > 0){
			x= centerViewAstre->x;
			y= centerViewAstre->y;
		}else{
			sys.GetBiggestAstrePosition(x,y);
		}
		dispCenter = QPointF(x,y);
	}
	if(dispScale == 0)dispScale = 0.3;
	
	QTransform matrix;
	matrix.scale(dispScale, dispScale);
	matrix.translate(-dispCenter.x(), -dispCenter.y());
	matrix.translate(width()/2/dispScale , height()/2/dispScale);
	
	painter.setWorldTransform(matrix);

	for(ARR<Astre>::iterator a=sys.astre.begin(); a != sys.astre.end(); a++)
		paintAstre(painter, *a);
	
//	if(newAstre.m > 0 ) paintAstre(painter, newAstre);
}

void AstreField::paintAstre(QPainter &painter, const Astre& astre)
{
	if(astre.m>0){
		painter.setPen(colors[astre.num % 15]);
		painter.setBrush(colors[astre.num % 15]);
		//painter.setBackground(Qt::black);
		
		
		painter.drawEllipse(QPointF(astre.x, astre.y), astre.r, astre.r);
	}
}


void AstreField::init()
{
	cptAstre=0;
	timerCount = 0;
	dispTimer->stop();
	sys.Reset();
	{
		DTYPE m=600;
		DTYPE r=Astre::RadiusFromMass(m);
		DTYPE x=0;
		DTYPE y=0;
		DTYPE vx=0;
		DTYPE vy=0;
		char name[32];
		sprintf(name, "planet %d", cptAstre);
		Astre astre(m, r, x, y, vx, vy, name);
		sys.AddAstre(astre);
		cptAstre++;
	}
	steps=0;
	perf=0;
	emit nbAstreChanged(sys.NbAstre());
	update();
}

void AstreField::astreChanged(int num, const Astre& astre1){
	
	for(ARR<Astre>::iterator a=sys.astre.begin(); a != sys.astre.end(); a++){
		if(a->num == num) {
			a->m = astre1.m;
			a->r = astre1.r;
		}
	}
	update();
}

void AstreField::createRandom(){
	DTYPE m=100;
	DTYPE r=Astre::RadiusFromMass(m);
	DTYPE x=0;
	DTYPE y=0;
	sys.GetGravityCenter(x,y);
	DTYPE v=20;
	sys.CreateRandom(10, x, y, Universe::distMax, m, r, v);
	emit nbAstreChanged(sys.NbAstre());
	update();
}



