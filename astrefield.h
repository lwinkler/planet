/****************************************************************************
**
** Simulator of a system of celestial objects
**
** Author : Laurent Winkler
** 
****************************************************************************/

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
				
	signals:
		void nbAstreChanged(int nb);
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
		QTimer *dispTimer;

		DTYPE newAstre_x;
		DTYPE newAstre_y;
		int timerCountNewAstre;
		QPointF dispCenter;
		DTYPE dispScale;

};

#endif
