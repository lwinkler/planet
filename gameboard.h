/****************************************************************************
**
** Simulator of a system of celestial objects
**
** Author : Laurent Winkler
** 
****************************************************************************/

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>

class QLCDNumber;
class QComboBox;
class Universe;
class AstreField;


/// @class Gameboard The main widget

class GameBoard : public QWidget
{
	Q_OBJECT
	
	public:
		GameBoard(QWidget *parent = 0);
		
	protected slots:
		void fillViewCenter(int nb);
		void about();
		
		
private:
		QComboBox* viewCenter;
		QLCDNumber* number;
		QLCDNumber* steps;
		QLCDNumber* perf;
		Universe * sys;
		AstreField * astreField;
};

#endif
