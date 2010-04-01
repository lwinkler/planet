/****************************************************************************
**
** Copyright (C) 2005-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the documentation of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file.  Alternatively you may (at
** your option) use any later version of the GNU General Public
** License if such license has been publicly approved by Trolltech ASA
** (or its successors, if any) and the KDE Free Qt Foundation. In
** addition, as a special exception, Trolltech gives you certain
** additional rights. These rights are described in the Trolltech GPL
** Exception version 1.2, which can be found at
** http://www.trolltech.com/products/qt/gplexception/ and in the file
** GPL_EXCEPTION.txt in this package.
**
** Please review the following information to ensure GNU General
** Public Licensing requirements will be met:
** http://trolltech.com/products/qt/licenses/licensing/opensource/. If
** you are unsure which license is appropriate for your use, please
** review the following information:
** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
** or contact the sales department at sales@trolltech.com.
**
** In addition, as a special exception, Trolltech, as the sole
** copyright holder for Qt Designer, grants users of the Qt/Eclipse
** Integration plug-in the right for the Qt/Eclipse Integration to
** link to functionality provided by Qt Designer and its related
** libraries.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not expressly
** granted herein.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

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

#include "iostream"

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
	
	setPalette(QPalette(QColor(250, 250, 200)));
	setAutoFillBackground(true);
	setPalette(QPalette(QColor(250, 250, 200)));
	setAutoFillBackground(true);
	//newTarget();
	//sys=new System;

	dispScale=0.5;
	dispCenter=QPointF(0,0);
	
	init();

	
//	setUpdatesEnabled(true);

	//update();	
	
	/*QPainter painter(this);
	painter.setBackground(Qt::black);
	

	painter.setPen(Qt::black);
	painter.setBrush(Qt::red);
	painter.setBackground(Qt::black);
	painter.drawEllipse(QPointF(33,33), 4, 40);*/
	//cout<<"end of field init"<<endl;
	//activateWindow();
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
	
	/*if(centerView >= 0){
		int tmp=centerView;
		vector<Astre>::iterator a;
		int i=0;
		for(a=sys.astre.begin(); a != sys.astre.end() && tmp>0; a++, i++)
			if(a->m > 0) tmp--;
		centerView = i;
	}*/
	centerViewAstre = sys.astre.begin();
	if(centerView >= 0)
		for(int i=0; i<centerView;i++)
			centerViewAstre++;
		
	cout<<"change view center"<<index<<" "<<centerView<<endl;
	update();
}

void AstreField::changeViewScale(int index)
{
	scaleView = index;
}

void AstreField::moveSys()
{
	//QRegion region = QRect();
	//cout<<"moveSys"<<endl;
	++timerCount;
	
	int nbCollision = sys.ComputeSpeed();
	//setUpdatesEnabled(false);
	sys.Move();
	if(nbCollision > 0)
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
	//setUpdatesEnabled(true);
	update();
}

void AstreField::mousePressEvent(QMouseEvent *event)
{
	if (event->button() != Qt::LeftButton)
		return;
	QPoint pos = event->pos();
	
	QTransform matrix;
	matrix.scale(dispScale, dispScale);
	matrix.translate(-dispCenter.x(), -dispCenter.y());
	matrix.translate(width()/2/dispScale , height()/2/dispScale);
	QTransform invmat;
	invmat  = matrix.inverted();
	QPointF posSys = pos * invmat;
	
	newAstre.x=posSys.x();
	newAstre.y=posSys.y();
	
	timerCountNewAstre = timerCount;
}

void AstreField::mouseMoveEvent(QMouseEvent *event)
{
	/*if (!barrelPressed)
		return;
	QPoint pos = event->pos();
	if (pos.x() <= 0)
		pos.setX(1);
	if (pos.y() >= height())
		pos.setY(height() - 1);
	double rad = atan(((double)rect().bottom() - pos.y()) / pos.x());
	setAngle(qRound(rad * 180 / 3.14159265));*/
}

void AstreField::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() != Qt::LeftButton)
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
	
	float vx= (posSys.x() - newAstre.x) / (timerCountNewAstre * Astre::dt);
	float vy= (posSys.y() - newAstre.y) / (timerCountNewAstre * Astre::dt);
	
	if(timerCountNewAstre==0)vx=vy=0; // If simulation stopped
	
	newAstre.m=6e10;
	newAstre.r=pow(newAstre.m/6e12, 0.333)*20;
	newAstre.x=posSys.x();
	newAstre.y=posSys.y();
	newAstre.vx=vx;
	newAstre.vy=vy;
	sys.AddAstre(newAstre);
	newAstre.m=0;
	
	emit nbAstreChanged(sys.NbAstre());
	update();

//	if (event->button() == Qt::LeftButton)
//		barrelPressed = false;
}

void AstreField::paintEvent(QPaintEvent * /* event */)
{
	
	//exit(-1);
	//cout<<"paintEvent"<<endl;
	QPainter painter(this);
	painter.setBackground(Qt::black);
	
	float x = 0;
	float y = 0;
	float x1,y1,x2,y2;
	
	//cout<<"aaaachange view center"<<centerView<<endl;	
	dispScale = 0;
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
		sys.GetBorders(x1,y1,x2,y2);
		dispCenter = QPointF((x1+x2)/2,(y1+y2)/2);
		dispScale = Astre::SetInRange(MIN(width()/(x2-x1), height()/(y2-y1)), 0.01, 1);
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
	
	if(newAstre.m > 0 ) paintAstre(painter, newAstre);
}

void AstreField::paintAstre(QPainter &painter, const Astre& astre)
{
	if(astre.m>0){
		painter.setPen(Qt::black);
		painter.setBrush(colors[astre.num % 15]);
		//painter.setBackground(Qt::black);
		
		
		painter.drawEllipse(QPointF(astre.x, astre.y), astre.r, astre.r);
	}
}


void AstreField::init()
{
	timerCount = 0;
	dispTimer->stop();
	sys.astre.resize(0);
	{
		Astre astre;
		astre.m=6e13;
		astre.r=pow(astre.m/6e12, 0.333)*20;
		astre.x=150;
		astre.y=150;
		astre.vx=0;
		astre.vy=0;
		sys.AddAstre(astre);
	}
	
	update();
}
/*
void AstreField::paintTarget(QPainter &painter)
{
	painter.setPen(Qt::black);
	painter.setBrush(Qt::red);
	painter.drawRect(targetRect());
	}
	
	void AstreField::paintBarrier(QPainter &painter)
	{
		painter.setPen(Qt::black);
		painter.setBrush(Qt::yellow);
		painter.drawRect(barrierRect());
	}
	
	const QRect barrelRect(30, -5, 20, 10);
	
	void AstreField::paintCannon(QPainter &painter)
	{
		painter.setPen(Qt::NoPen);
		painter.setBrush(Qt::blue);
		
		painter.save();
		painter.translate(0, height());
		painter.drawPie(QRect(-35, -35, 70, 70), 0, 90 * 16);
		painter.rotate(-currentAngle);
		painter.drawRect(barrelRect);
		painter.restore();
	}
	
	QRect AstreField::cannonRect() const
	{
		QRect result(0, 0, 50, 50);
		result.moveBottomLeft(rect().bottomLeft());
		return result;
	}
	
	QRect AstreField::shotRect() const
	{
		const double gravity = 4;
		
		double time = timerCount / 20.0;
		double velocity = shootForce;
		double radians = shootAngle * 3.14159265 / 180;
		
		double velx = velocity * cos(radians);
		double vely = velocity * sin(radians);
		double x0 = (barrelRect.right() + 5) * cos(radians);
		double y0 = (barrelRect.right() + 5) * sin(radians);
		double x = x0 + velx * time;
		double y = y0 + vely * time - 0.5 * gravity * time * time;
		
		QRect result(0, 0, 6, 6);
		result.moveCenter(QPoint(qRound(x), height() - 1 - qRound(y)));
		return result;
	}
	
	QRect AstreField::targetRect() const
	{
		QRect result(0, 0, 20, 10);
		result.moveCenter(QPoint(target.x(), height() - 1 - target.y()));
		return result;
	}
	
	QRect AstreField::barrierRect() const
	{
		return QRect(145, height() - 100, 15, 99);
	}*/

