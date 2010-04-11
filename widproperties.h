
#ifndef ASTREPROPERTIES_H
#define ASTREPROPERTIES_H

#include <QWidget>
#include "astre.h"

class QLabel;
class QSlider;
class QLabel;
class QCheckBox;

class AstreProperties : public QWidget
{
	Q_OBJECT
	
	public:
		AstreProperties(QWidget *parent = 0);
		//QString text() const;
		
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
