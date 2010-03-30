#include <QLCDNumber>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>

#include "widproperties.h"
#include <cmath>
#include <iostream>

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
void AstreProperties::SetAstre(Astre* astre1){
	astre=astre1;
}
void AstreProperties::changeMass(int mass){
	if(astre!=NULL)astre->m=exp(mass);
	else cout<<"Error : No astre set !"<<endl;
}
void AstreProperties::changeRadius(int radius){
	if(astre!=NULL)astre->r=exp(radius);
	else cout<<"Error : No astre set !"<<endl;
}

void AstreProperties::init()
{
	QLCDNumber *lcdMass = new QLCDNumber(4);
	lcdMass->setSegmentStyle(QLCDNumber::Filled);
	
	QLCDNumber *lcdRadius = new QLCDNumber(3);
	lcdRadius->setSegmentStyle(QLCDNumber::Filled);

	sliderMass = new QSlider(Qt::Horizontal);
	sliderMass->setRange(0, 9999);
	sliderMass->setValue(100);
	
	sliderRadius = new QSlider(Qt::Horizontal);
	sliderRadius->setRange(0, 999);
	sliderRadius->setValue(10);
	
	label = new QLabel;
	label->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
	label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	
	connect(sliderMass, SIGNAL(valueChanged(int)),this, SLOT(changeMass(int)));
	connect(sliderMass, SIGNAL(valueChanged(int)),lcdMass, SLOT(display(int)));
	connect(sliderRadius, SIGNAL(valueChanged(int)),this, SLOT(changeRadius(int)));
	connect(sliderRadius, SIGNAL(valueChanged(int)),lcdRadius, SLOT(display(int)));
			
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(label);
	layout->addWidget(lcdMass);
	layout->addWidget(sliderMass);
	layout->addWidget(lcdRadius);
	layout->addWidget(sliderRadius);
	setLayout(layout);
			
	setFocusProxy(sliderMass);
	setUpdatesEnabled(true);
	astre=NULL;
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
