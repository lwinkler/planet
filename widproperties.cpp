#include <QLCDNumber>
#include <QLabel>
#include <QSlider>
#include <QCheckBox>
#include <QVBoxLayout>

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
	sliderMass->setRange(0, Astre::MassFromRadius(100));
	
	//sliderMass->setValue(100);
	
	sliderRadius = new QSlider(Qt::Horizontal);
	sliderRadius->setRange(0, 100);
	//sliderRadius->setValue(10);
	
	connect(sliderMass, SIGNAL(valueChanged(int)),this, SLOT(changeMass(int)));
	connect(sliderRadius, SIGNAL(valueChanged(int)),this, SLOT(changeRadius(int)));

	cbFixed  = new QCheckBox(tr("Link mass with radius"));
	
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(label);
	layout->addWidget(labMass,1,0);
	layout->addWidget(dispMass,1,1);
	layout->addWidget(sliderMass,1,2);
	layout->addWidget(labRadius,2,0);
	layout->addWidget(dispRadius,2,1);
	layout->addWidget(sliderRadius,2,2);
	layout->addWidget(cbFixed, 3, 1);
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