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

#include <QLCDNumber>
#include <QLabel>
#include <QSlider>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QPushButton>

#include "widproperties.h"
#include <cmath>
#include <iostream>
#include <stdio.h>

using namespace std;

AstreProperties::AstreProperties(QWidget *parent)
: QWidget(parent)
{
	init();
}

void AstreProperties::init()
{
	astre.num = -1;
	settingMass=settingRadius=false;
	hide();
	label = new QLabel;
	label->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
	label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	
	QLabel* labMass = new QLabel("Mass");
	dispMass = new QLabel("0");
	//lcdMass->setSegmentStyle(QLCDNumber::Filled);
	
	QLabel* labRadius = new QLabel("Radius");
	dispRadius = new QLabel("0");
	//lcdRadius->setSegmentStyle(QLCDNumber::Filled);

	sliderMass = new QSlider(Qt::Horizontal);
	sliderMass->setRange(1, Astre::MassFromRadius(100));
	
	QPushButton *btnRemove = new QPushButton(tr("&Remove planet"));
	//btnRemove->setFont(QFont("Times", 18, QFont::Bold));
	
	connect(btnRemove, SIGNAL(clicked()), this, SLOT(removeAstre()));
	
	sliderRadius = new QSlider(Qt::Horizontal);
	sliderRadius->setRange(1, 100);
	//sliderRadius->setValue(10);
	
	connect(sliderMass, SIGNAL(valueChanged(int)),this, SLOT(changeMass(int)));
	connect(sliderRadius, SIGNAL(valueChanged(int)),this, SLOT(changeRadius(int)));

	cbFixed  = new QCheckBox(tr("Link mass with radius"));
	cbFixed->setChecked(true);
	
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(label);
	layout->addWidget(labMass,1,0);
	layout->addWidget(dispMass,1,1);
	layout->addWidget(sliderMass,1,2);
	layout->addWidget(labRadius,2,0);
	layout->addWidget(dispRadius,2,1);
	layout->addWidget(sliderRadius,2,2);
	layout->addWidget(cbFixed, 3, 1);
	layout->addWidget(btnRemove, 4, 1);
	setLayout(layout);
			
	setFocusProxy(sliderMass);
	setUpdatesEnabled(true);
}

void AstreProperties::changeMass(int slider){
	//cout<<"changeMass"<<endl;
	if(!settingRadius)settingMass=true;
	if(astre.num >= 0) {
		DTYPE mass = ((DTYPE)slider);
		astre.m= mass;
		
		if(cbFixed->isChecked() && !settingRadius) {
			astre.r=Astre::RadiusFromMass(astre.m);
			sliderRadius->setValue(astre.r);
		}
		emit astreChanged(astre.num, astre);
		update();
	}
	settingMass=false;
}

void AstreProperties::changeRadius(int slider){
	//cout<<"changeRAdius"<<endl;
	if(!settingMass)settingRadius=true;
	if (astre.num >= 0) {
		astre.r = slider;
		if(cbFixed->isChecked() && !settingMass){
			astre.m=Astre::MassFromRadius(astre.r);
			sliderMass->setValue(astre.m);
		}
		emit astreChanged(astre.num, astre);
		update();
	}
	settingRadius=false;
}

void AstreProperties::removeAstre(){
	if (astre.num >= 0) {
		astre.m = 0;
		astre.r = 0;
		emit astreChanged(astre.num, astre);
		update();
	}
}

void AstreProperties::paintEvent(QPaintEvent*){
	if(astre.num >= 0) {
		char tmp[32];
		
		//Update display
		sprintf(tmp, "%f", astre.m);
		dispMass->setText(tmp);
		sprintf(tmp, "%f", astre.r);
		dispRadius->setText(tmp);
	}
}

/// When a new planet is selected
void AstreProperties::setAstre(int num, const Astre& a)
{
	//cout<<"setAstre"<<num<<a.name<<endl;
	if(num == -1) hide();
	else{
		show();
		label->setText(a.name.c_str());
		astre = a;
		astre.num = num;

		update();
	}
}