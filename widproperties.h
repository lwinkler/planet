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

#ifndef ASTREPROPERTIES_H
#define ASTREPROPERTIES_H

#include <QWidget>
#include "astre.h"

class QLabel;
class QSlider;
class QLabel;
class QCheckBox;

/// @class AstreProperties QT widget to edit the properties of a celestial corpse 

class AstreProperties : public QWidget
{
	Q_OBJECT
	
	public:
		AstreProperties(QWidget *parent = 0);
		
	public slots:
		void changeMass(int mass);
		void changeRadius(int radius);
		void removeAstre();
		void setAstre(int num, const Astre&);
		
	signals:
		void astreChanged(int num, const Astre astre);
		
	private:
		Astre astre;
		void init();
		void paintEvent(QPaintEvent*);
		
		QSlider *sliderMass;
		QSlider *sliderRadius;
		QLabel *dispMass;
		QLabel *dispRadius;
		QLabel *label;
		QCheckBox *cbFixed;
		bool settingMass; // If mass and radius linked
		bool settingRadius;
};

#endif
