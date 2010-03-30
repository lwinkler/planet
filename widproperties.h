
#ifndef ASTREPROPERTIES_H
#define ASTREPROPERTIES_H

#include <QWidget>
#include "astre.h"

class QLabel;
class QSlider;

class AstreProperties : public QWidget
{
	Q_OBJECT
	
	public:
		AstreProperties(QWidget *parent = 0);
		AstreProperties(const QString &text, QWidget *parent = 0);
		
		//int value() const;
		QString text() const;
		
	public slots:
		//void setValue(int value);
		//void setRange(int minValue, int maxValue);
		//void setText(const QString &text);
		void SetAstre(Astre* astre1);
		void changeMass(int mass);
		void changeRadius(int radius);
		
	signals:
		//void valueChanged(int newValue);
		
	private:
		Astre* astre;
		void init();
		
		QSlider *sliderMass;
		QSlider *sliderRadius;
		QLabel *label;
};

#endif
