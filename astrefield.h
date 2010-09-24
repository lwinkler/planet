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

#ifndef FIELD_H
#define FIELD_H

#include <QWidget>
#include "astre.h"

class QTimer;
class Universe;
class Astre;

/// @class AstreField QT widget on which the universe is painted

class AstreField : public QWidget
{
	Q_OBJECT
	
	public:
		AstreField(QWidget *parent = 0);
		~AstreField();

		int GetNbSteps() { return steps; };
		double GetComputationTime() { return perf; };

		Universe sys;
		
		static const int timerInterval;
		static const Qt::GlobalColor colors[15];
		
	private slots:
		void init();
		void moveSys();
		void pauseSimulation();
		void changeViewCenter(int index);
		void changeViewScale(int index);
		void astreChanged(int, const Astre& a);
		void createRandom();
				
	signals:
		void nbAstreChanged(int nb);
		void systemMoved(int nb);
		void astreSelected(int num, const Astre& a);
		
	protected:
		void paintEvent(QPaintEvent *event);
		void paintAstre(QPainter& painter, const Astre& astre);
		void mousePressEvent(QMouseEvent *event);
		//void mouseMoveEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		
	private:
		int timerCount;
		int centerView;
		std::ARR<Astre>::iterator centerViewAstre;
		int scaleView;
		int cptAstre;
		double perf;
		int steps;
		QTimer *dispTimer;

		DTYPE newAstre_x;
		DTYPE newAstre_y;
		int timerCountNewAstre;
		QPointF dispCenter;
		DTYPE dispScale;

};

#endif
