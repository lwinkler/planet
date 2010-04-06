#include <QLCDNumber>
#include <QLabel>
#include <QSlider>
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

AstreProperties::AstreProperties(const QString &text, QWidget *parent)
: QWidget(parent)
{
	init();
	//setText(text);
}

//void AstreProperties::SetAstre(Astre* astre1){
	//astre=astre1;
//}

void AstreProperties::changeMass(int slider){
	if(astre.num >= 0) {
		char tmp[32];
		float mass = ((float)slider);
		astre.m= mass;
		sprintf(tmp, "%f", mass);
		dispMass->setText(tmp);
		emit astreChanged(astre.num, astre);
		update();
	}
	/*if(astre!=NULL){
		astre->m=exp(mass);
		
	}
	else cout<<"Error : No astre set !"<<endl;*/
}

void AstreProperties::changeRadius(int slider){
	if (astre.num >= 0) {
		char tmp[32];
		astre.r = slider;
		sprintf(tmp, "%f", astre.r);
		dispRadius->setText(tmp);
		emit astreChanged(astre.num, astre);
		update();
	}
	/*
	if(astre!=NULL)astre->r=exp(radius);
	else cout<<"Error : No astre set !"<<endl;*/
}

void AstreProperties::init()
{
	astre.num = -1;
	hide();
	dispMass = new QLabel("0");
	//lcdMass->setSegmentStyle(QLCDNumber::Filled);
	
	dispRadius = new QLabel("0");
	//lcdRadius->setSegmentStyle(QLCDNumber::Filled);

	sliderMass = new QSlider(Qt::Horizontal);
	sliderMass->setRange(0, 9999);
	//sliderMass->setValue(100);
	
	sliderRadius = new QSlider(Qt::Horizontal);
	sliderRadius->setRange(0, 999);
	//sliderRadius->setValue(10);
	
	label = new QLabel;
	label->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
	label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	
	connect(sliderMass, SIGNAL(valueChanged(int)),this, SLOT(changeMass(int)));
	//connect(sliderMass, SIGNAL(valueChanged(int)),dispMass, SLOT(display(int)));
	connect(sliderRadius, SIGNAL(valueChanged(int)),this, SLOT(changeRadius(int)));
	//connect(sliderRadius, SIGNAL(valueChanged(int)),dispRadius, SLOT(display(int)));
			
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(label);
	layout->addWidget(dispMass);
	layout->addWidget(sliderMass);
	layout->addWidget(dispRadius);
	layout->addWidget(sliderRadius);
	setLayout(layout);
			
	setFocusProxy(sliderMass);
	setUpdatesEnabled(true);
	//astre=NULL;
}

void AstreProperties::setAstre(int num, const Astre& a)
{
	cout<<"setAstre"<<num<<endl;
	if(num == -1) hide();
	else{
		show();
		char tmp[32];
		
		astre = a;
		astre.num = num;

		sliderMass->setValue((a.m));
		sliderRadius->setValue(a.r);
		
		/*sprintf(tmp, "%f", a.m);
		dispMass->setText(tmp);
		sprintf(tmp, "%f", a.r);
		dispRadius->setText(tmp);*/
		
		update();
	}
}

/*int AstreProperties::value() const
{
	return slider->value();
	}
	
	QString AstreProperties::text() const
	{
		return label->text();
		}
		
		void AstreProperties::setValue(int value)
		{
			slider->setValue(value);
		}
		
		void AstreProperties::setRange(int minValue, int maxValue)
		{
			if (minValue < 0 || maxValue > 99 || minValue > maxValue) {
       qWarning("AstreProperties::setRange(%d, %d)\n"
       "\tRange must be 0..99\n"
       "\tand minValue must not be greater than maxValue",
       minValue, maxValue);
       return;
       }
       slider->setRange(minValue, maxValue);
		}
		
		void AstreProperties::setText(const QString &text)
		{
			label->setText(text);
		}*/
