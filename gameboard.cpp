
#include <QApplication>
#include <QFont>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QShortcut>
#include <QVBoxLayout>
#include <QWidget>

#include <vector>
#include <stdio.h>
#include <iostream>

#include "astrefield.h"
#include "gameboard.h"
#include "widproperties.h"

using namespace std;

GameBoard::GameBoard(QWidget *parent)
: QWidget(parent)
{
	QPushButton *quit = new QPushButton(tr("&Quit"));
	quit->setFont(QFont("Times", 18, QFont::Bold));
	
	connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
	
	/*LCDRange *angle = new LCDRange(tr("ANGLE"));
	angle->setRange(5, 70);
	
	LCDRange *force = new LCDRange(tr("FORCE"));
	force->setRange(10, 50);
	*/
	QFrame *cannonBox = new QFrame;
	cannonBox->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
	
	astreField = new AstreField;
	sys= &astreField->sys;
	
	/*connect(angle, SIGNAL(valueChanged(int)),
	cannonField, SLOT(setAngle(int)));
	connect(cannonField, SIGNAL(angleChanged(int)),
	angle, SLOT(setValue(int)));
	
	connect(force, SIGNAL(valueChanged(int)),
	cannonField, SLOT(setForce(int)));
	connect(cannonField, SIGNAL(forceChanged(int)),
	force, SLOT(setValue(int)));
	
	connect(cannonField, SIGNAL(hit()),
	this, SLOT(hit()));
	connect(cannonField, SIGNAL(missed()),
	this, SLOT(missed()));*/
	
	QPushButton *shoot = new QPushButton(tr("&Shoot"));
	shoot->setFont(QFont("Times", 18, QFont::Bold));
	
	//connect(shoot, SIGNAL(clicked()),this, SLOT(fire()));
	//connect(cannonField, SIGNAL(canShoot(bool)),shoot, SLOT(setEnabled(bool)));
	
	QPushButton *restart = new QPushButton(tr("&New Simulation"));
	restart->setFont(QFont("Times", 18, QFont::Bold));
	connect(restart, SIGNAL(clicked()), astreField, SLOT(init()));
	
	QPushButton *pause = new QPushButton(tr("&Play/Pause"));
	pause->setFont(QFont("Times", 18, QFont::Bold));
	
	connect(pause, SIGNAL(clicked()), astreField, SLOT(pauseSimulation()));
	
	//hits = new QLCDNumber(2);
	//hits->setSegmentStyle(QLCDNumber::Filled);
	
	//shotsLeft = new QLCDNumber(2);
	//shotsLeft->setSegmentStyle(QLCDNumber::Filled);
	
	QLabel *hitsLabel = new QLabel(tr("HITS"));
	QLabel *shotsLeftLabel = new QLabel(tr("SHOTS LEFT"));
	
	//(void) new QShortcut(Qt::Key_Enter, this, SLOT(fire()));
	//(void) new QShortcut(Qt::Key_Return, this, SLOT(fire()));
	//(void) new QShortcut(Qt::CTRL + Qt::Key_Q, this, SLOT(close()));
	
	/// View layout
	QHBoxLayout *viewLayout = new QHBoxLayout;
	QLabel* viewTitle = new QLabel(tr("Center"));
	viewLayout->addWidget(viewTitle);
	viewCenter = new QComboBox;
	fillViewCenter();
	connect(viewCenter, SIGNAL(activated(int)), astreField, SLOT(changeViewCenter(int)));
	connect(astreField, SIGNAL(nbAstreChanged(int)), this, SLOT(fillViewCenter()));
	

	//QComboBox* viewPlanetList = new QComboBox;
	
	viewLayout->addWidget(viewCenter);

//	viewLayout->addWidget(viewPlanetList);

	
	AstreProperties* widProp = new AstreProperties();
	
	QHBoxLayout *topLayout = new QHBoxLayout;
	//topLayout->addWidget(shoot);
	//topLayout->addWidget(hits);
	topLayout->addWidget(hitsLabel);
	//topLayout->addWidget(shotsLeft);
	topLayout->addWidget(shotsLeftLabel);
	topLayout->addStretch(1);
	topLayout->addWidget(restart);
	topLayout->addWidget(pause);
	
	QVBoxLayout *leftLayout = new QVBoxLayout;
	leftLayout->addWidget(widProp);
	leftLayout->addLayout(viewLayout);
	//leftLayout->addWidget(force);
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(astreField);
	cannonBox->setLayout(mainLayout);
	
	QGridLayout *gridLayout = new QGridLayout;
	gridLayout->addWidget(quit, 0, 0);
	gridLayout->addLayout(topLayout, 0, 1);
	gridLayout->addLayout(leftLayout, 1, 0);
	gridLayout->addWidget(cannonBox, 1, 1, 2, 1);
	gridLayout->setColumnStretch(1, 10);
	setLayout(gridLayout);
	
	/*angle->setValue(60);
	force->setValue(25);
	angle->setFocus();
	
	newGame();*/
}

/*void GameBoard::fire()
{
	if (cannonField->gameOver() || cannonField->isShooting())
	return;
	shotsLeft->display(shotsLeft->intValue() - 1);
	cannonField->shoot();
	}
	
	void GameBoard::hit()
	{
		hits->display(hits->intValue() + 1);
		if (shotsLeft->intValue() == 0)
		cannonField->setGameOver();
		else
			cannonField->newTarget();
	}
	
	void GameBoard::missed()
	{
		if (shotsLeft->intValue() == 0)
		cannonField->setGameOver();
	}
	
	void GameBoard::newGame()
	{
		shotsLeft->display(15);
		hits->display(0);
		cannonField->restartGame();
		cannonField->newTarget();
	}*/

void GameBoard::fillViewCenter(){
	//std::cout<<"fillViewCenter"<<endl;
	viewCenter->clear();

	viewCenter->addItem(tr("on gravity center"), -3);
	viewCenter->addItem(tr("on biggest planet"), -2);
	viewCenter->addItem(tr("global"), -1);
	
	char str[256];
	if(sys != NULL) 
		for(vector<Astre>::iterator a=sys->astre.begin(); a != sys->astre.end(); a++)
			if(a->m >0)
			{
				sprintf(str, "%s %d", "on planet", a->num);
				cout<<"add Item "<<a->num<<endl;
				viewCenter->addItem(str, a->num);
			}
}