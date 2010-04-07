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
	
	float vx= (posSys.x() - newAstre_x) / (timerCountNewAstre * Astre::dt);
	float vy= (posSys.y() - newAstre_y) / (timerCountNewAstre * Astre::dt);
	
	if(timerCountNewAstre==0)vx=vy=0; // If simulation stopped
	
	float m=6.;
	float r=Astre::RadiusFromMass(m);
	Astre newAstre(m, r, posSys.x(), posSys.y(), vx, vy, "planet " + cptAstre);
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
	
	float x = 0;
	float y = 0;
	float x1,y1,x2,y2;
	
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
		float m=600;
		float r=Astre::RadiusFromMass(m);
		float x=0;
		float y=0;
		float vx=0;
		float vy=0;
		string name = "planet " + cptAstre;
		Astre astre(m, r, x, y, vx, vy, name);
		sys.AddAstre(astre);
		cptAstre++;
	}
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

