
#include <QApplication>
#include <QFont>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QShortcut>
#include <QVBoxLayout>
#include <QWidget>

#include <vector>
#include <stdio.h>
#include <iostream>

#include "astrefield.h"
#include "gameboard.h"
#include "widproperties.h"

#include "astre.h"

using namespace std;

GameBoard::GameBoard(QWidget *parent)
: QWidget(parent)
{
	astreField = new AstreField;
	sys= &astreField->sys;
	
	QFrame *astreBox = new QFrame;
	astreBox->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);

	QPushButton *quit = new QPushButton(tr("&Quit"));
	quit->setFont(QFont("Times", 18, QFont::Bold));
	
	connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
		
	QPushButton *restart = new QPushButton(tr("&New Simulation"));
	restart->setFont(QFont("Times", 18, QFont::Bold));
	connect(restart, SIGNAL(clicked()), astreField, SLOT(init()));
	
	QPushButton *pause = new QPushButton(tr("&Play/Pause"));
	pause->setFont(QFont("Times", 18, QFont::Bold));
	
	connect(pause, SIGNAL(clicked()), astreField, SLOT(pauseSimulation()));
	
	//hits->setSegmentStyle(QLCDNumber::Filled);
	
	//shotsLeft = new QLCDNumber(2);
	//shotsLeft->setSegmentStyle(QLCDNumber::Filled);
	
	QLabel* numberLabel = new QLabel(tr("PLANETS"));
	number = new QLCDNumber(3);
	number->setSegmentStyle(QLCDNumber::Filled);
	
	//QLabel *shotsLeftLabel = new QLabel(tr("SHOTS LEFT"));
	
	//(void) new QShortcut(Qt::Key_Enter, this, SLOT(fire()));
	//(void) new QShortcut(Qt::Key_Return, this, SLOT(fire()));
	//(void) new QShortcut(Qt::CTRL + Qt::Key_Q, this, SLOT(close()));
	
	/// View layout
	QHBoxLayout *viewLayout = new QHBoxLayout();
	
	QLabel* viewCenterLab = new QLabel(tr("Center"));
	viewCenter = new QComboBox;
	fillViewCenter(1);
	connect(viewCenter, SIGNAL(activated(int)), astreField, SLOT(changeViewCenter(int)));
	connect(astreField, SIGNAL(nbAstreChanged(int)), this, SLOT(fillViewCenter(int)));
	QLabel* viewScaleLab = new QLabel(tr("Scale"));
	QSpinBox* viewScaleBox= new QSpinBox();
	viewScaleBox->setMinimum(1);
	viewScaleBox->setMaximum(1000);
	viewScaleBox->setAccelerated(true);
	viewScaleBox->setSingleStep(1);
	viewScaleBox->setValue(100);
	viewScaleBox->setSuffix("%");
	connect(viewScaleBox, SIGNAL(valueChanged(int)), astreField, SLOT(changeViewScale(int)));
	
	//QComboBox* viewPlanetList = new QComboBox;
	
	viewLayout->addWidget(viewCenterLab);
	viewLayout->addWidget(viewCenter);
	viewLayout->addWidget(viewScaleLab);
	viewLayout->addWidget(viewScaleBox);
	
	AstreProperties* widProp = new AstreProperties();
	connect(astreField, SIGNAL(astreSelected(int, const Astre&)), widProp, SLOT(setAstre(int, const Astre&)));
	connect(widProp, SIGNAL(astreChanged(int, const Astre&)), astreField, SLOT(astreChanged(int, const Astre&)));
	
	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addWidget(numberLabel);
	topLayout->addWidget(number);
	topLayout->addStretch(1);
	topLayout->addWidget(restart);
	topLayout->addWidget(pause);
	
	QVBoxLayout *leftLayout = new QVBoxLayout;
	leftLayout->addWidget(widProp);
	leftLayout->addLayout(viewLayout);
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(astreField);
	astreBox->setLayout(mainLayout);
	
	QGridLayout *gridLayout = new QGridLayout;
	gridLayout->addWidget(quit, 0, 0);
	gridLayout->addLayout(topLayout, 0, 1);
	gridLayout->addLayout(leftLayout, 1, 0);
	gridLayout->addWidget(astreBox, 1, 1, 2, 1);
	gridLayout->setColumnStretch(1, 10);
	
	setLayout(gridLayout);
}

void GameBoard::fillViewCenter(int /*nb*/){
	
	// Fill view selection menu
	viewCenter->clear();
	viewCenter->addItem(tr("on gravity center"), -3);
	viewCenter->addItem(tr("on biggest planet"), -2);
	viewCenter->addItem(tr("global"), -1);
	
	char str[256];
	if(sys != NULL) 
		for(ARR<Astre>::iterator a=sys->astre.begin(); a != sys->astre.end(); a++)
			if(a->m >0)
			{
				sprintf(str, "%s %d", "on planet", a->num);
				//cout<<"add Item "<<a->num<<endl;
				viewCenter->addItem(str, a->num);
			}
	// LCD
	number->display(sys->NbAstre());
	update();
}
