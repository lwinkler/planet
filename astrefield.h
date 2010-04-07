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

#ifndef FIELD_H
#define FIELD_H

#include <QWidget>
#include "astre.h"

class QTimer;
class System;
class Astre;

class AstreField : public QWidget
{
	Q_OBJECT
	
	public:
		AstreField(QWidget *parent = 0);
		~AstreField();
		
		/*int angle() const { return currentAngle; }
		int force() const { return currentForce; }
		bool gameOver() const { return gameEnded; }
		bool isShooting() const;
		QSize sizeHint() const;*/
		
	public slots:
		/*void setAngle(int angle);
		void setForce(int force);
		void shoot();
		void newTarget();
		void setGameOver();
		void restartGame();*/
		
	private slots:
		void init();
		void moveSys();
		void pauseSimulation();
		void changeViewCenter(int index);
		void changeViewScale(int index);
		void astreChanged(int, const Astre& a);
				
	signals:
		//void hit();
		//void missed();
		void nbAstreChanged(int nb);
		void astreSelected(int num, const Astre& a);
		//void forceChanged(int newForce);
		//void canShoot(bool can);
		
	protected:
		void paintEvent(QPaintEvent *event);
		void paintAstre(QPainter& painter, const Astre& astre);
		void mousePressEvent(QMouseEvent *event);
		//void mouseMoveEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		
	private:
		//void paintShot(QPainter &painter);
		//void paintTarget(QPainter &painter);
		//void paintBarrier(QPainter &painter);
		//void paintCannon(QPainter &painter);
		//QRect cannonRect() const;
		//QRect shotRect() const;
		//QRect targetRect() const;
		//QRect barrierRect() const;
		//bool barrelHit(const QPoint &pos) const;
		
		//int currentAngle;
		//int currentForce;
		
		int timerCount;
		int centerView;
		std::ARR<Astre>::iterator centerViewAstre;
		int scaleView;
		int cptAstre;
		QTimer *dispTimer;
		//float shootAngle;
		//float shootForce;
		
		//QPoint target;
		
		//bool gameEnded;
		//bool barrelPressed;
		
		//Astre newAstre;
		float newAstre_x;
		float newAstre_y;
		int timerCountNewAstre;
		QPointF dispCenter;
		float dispScale;

	public:
		System sys;
		
		static const int timerInterval;
		static const Qt::GlobalColor colors[15];
};

#endif
