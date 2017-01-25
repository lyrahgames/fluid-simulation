#include "scroll-w.h"


ScrollW::ScrollW(QWidget *parent): QScrollArea(parent){
	setBackgroundRole(QPalette::Midlight);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setWidgetResizable(true);

	central_w = new QWidget(this);

	layout = new QVBoxLayout(central_w);

	setWidget(central_w);
}

void ScrollW::addWidget(QWidget *w){
	layout->addWidget(w);
	central_w->adjustSize();
}

void ScrollW::addToggleW(ToggleW *tw){
	addWidget(tw);
	connect(tw, SIGNAL(toggled()), this, SLOT(adjustSizeCentralWidget()));
}

void ScrollW::adjustSizeCentralWidget(){
	central_w->adjustSize();
}

void ScrollW::resizeEvent(QResizeEvent *event){
	central_w->updateGeometry();
	adjustSizeCentralWidget();
}