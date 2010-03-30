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

#include <math.h>
#include <stdlib.h>

#include "astre.h"
#include "astrefield.h"

#include "iostream"

using namespace std;

const int AstreField::timerInterval=20;

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
//	for(int i=0; i< sys.astre.size(); i++)
//		delete sys.astre[i];
//	delete sys;
}

/*void AstreField::setAngle(int angle)
{
	if (angle < 5)
	angle = 5;
	if (angle > 70)
	angle = 70;
	if (currentAngle == angle)
	return;
	currentAngle = angle;
	update(cannonRect());
	emit angleChanged(currentAngle);
	}

void AstreField::setForce(int force)
{
	if (force < 0)
		force = 0;
		if (currentForce == force)
			return;
	currentForce = force;
	emit forceChanged(currentForce);
}

void AstreField::shoot()
{
	if (isShooting())
		return;
	timerCount = 0;
	shootAngle = currentAngle;
	shootForce = currentForce;
	autoShootTimer->start(5);
	emit canShoot(false);
}

void AstreField::newTarget()
{
	static bool firstTime = true;
	
	if (firstTime) {
		firstTime = false;
		QTime midnight(0, 0, 0);
		qsrand(midnight.secsTo(QTime::currentTime()));
	}
	target = QPoint(200 + qrand() % 190, 10 + qrand() % 255);
	update();
}

void AstreField::setGameOver()
{
	if (gameEnded)
		return;
	if (isShooting())
		autoShootTimer->stop();
	gameEnded = true;
	update();
}

void AstreField::restartGame()
{
	if (isShooting())
		autoShootTimer->stop();
	gameEnded = false;
	update();
	emit canShoot(true);
}
*/
void AstreField::pauseSimulation()
{
	if(dispTimer->isActive())dispTimer->stop();
	else dispTimer->start(timerInterval);
	//paintEvent(NULL);
}

void AstreField::moveSys()
{
	//QRegion region = QRect();
	//cout<<"moveSys"<<endl;
	for(unsigned int i =0; i< sys.astre.size(); i++){
		++timerCount;
		
		sys.ComputeSpeed();
		sys.Move();
		
		QRect astreRect = QRect(-sys.astre[i].r/2 , sys.astre[i].r/2, -sys.astre[i].r/2 , sys.astre[i].r/2);
		astreRect.moveCenter(QPoint(sys.astre[i].x, sys.astre[i].y));
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
	update();
}

void AstreField::mousePressEvent(QMouseEvent *event)
{
/*	if (event->button() != Qt::LeftButton)
		return;
	if (barrelHit(event->pos()))
		barrelPressed = true;*/
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
//	if (event->button() == Qt::LeftButton)
//		barrelPressed = false;
}

void AstreField::paintEvent(QPaintEvent * /* event */)
{
	
	//exit(-1);
	//cout<<"paintEvent"<<endl;
	QPainter painter(this);
	painter.setBackground(Qt::black);
	

	for (unsigned int i=0; i < sys.astre.size(); i++)
		paintAstre(painter, &sys.astre[i]);
	
}

void AstreField::paintAstre(QPainter &painter, Astre* astre)
{
	if(astre->m>0){
		painter.setPen(Qt::NoPen);
		painter.setBrush(Qt::black);
		painter.setBackground(Qt::black);
		painter.drawEllipse(QPointF(astre->x, astre->y), astre->r, astre->r);
	}
}
void AstreField::init()
{
	timerCount = 0;
	dispTimer->stop();
	sys.astre.resize(0);
	{
		Astre astre;
		astre.m=6e12;
		astre.r=pow(astre.m/6e12, 0.333)*20;
		astre.x=150;
		astre.y=150;
		astre.vx=0;
		astre.vy=0;
		sys.AddAstre(astre);
	}
	{
		Astre astre;
		astre.m=1e10;
		astre.r=pow(astre.m/6e12, 0.333)*20;
		astre.x=150;
		astre.y=50;
		astre.vx=2;
		astre.vy=0;
		sys.AddAstre(astre);
	}
	{
		Astre astre;
		astre.m=2e10;
		astre.r=pow(astre.m/6e12, 0.333)*20;
		astre.x=50;
		astre.y=150;
		astre.vx=1;
		astre.vy=-2;
		sys.AddAstre(astre);
	}
	{
		Astre astre;
		astre.m=.5e10;
		astre.r=pow(astre.m/6e12, 0.333)*20;
		astre.x=250;
		astre.y=150;
		astre.vx=-1;
		astre.vy=2;
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

