#include "info-label-w.h"


InfoLabelW::InfoLabelW(QWidget *parent): QWidget(parent){
	name_l = new QLabel(this);
	name_l->setAlignment(Qt::AlignLeft);

	info_l = new QLabel(this);
	info_l->setAlignment(Qt::AlignRight);

	layout = new QHBoxLayout(this);
	layout->addWidget(name_l);
	layout->addWidget(info_l);
}

void InfoLabelW::setName(const QString& name){
	name_l->setText(name);
}

void InfoLabelW::setInfo(const QString& info){
	info_l->setText(info);
}