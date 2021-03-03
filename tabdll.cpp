#include "tabdll.h"
#include <QLabel>
tabDll::tabDll(QWidget *parent) : QWidget(parent)
{
	QVBoxLayout *v = new QVBoxLayout();
	QGroupBox *box = new QGroupBox("DLL");
	DLL = new QVBoxLayout();
	box->setLayout(DLL);
	v->addWidget(box);
	setLayout(v);
	QHBoxLayout *b = new QHBoxLayout;
	cancel = new QPushButton("Отмена");cancel->setHidden(true);
	ok = new QPushButton("Применить");ok->setHidden(true);
	b->addWidget(cancel);b->addWidget(ok);
	v->addLayout(b);
	connect(cancel,	&QPushButton::clicked , this , &tabDll::cancel_slot);
	connect(ok,		&QPushButton::clicked , this , &tabDll::ok_slot);
}
void tabDll::updateReg(fileRegistry *reg){
	regWidgets.clear();QVector<tabSettingsRegkey*> regs;
	foreach(tabSettingsRegkey *r,tabSettings::regSearch("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]",160,reg,regs,
														QStringList() << "builtin" << "native" << "native,builtin" << "builtin,native")){
		regWidgets.append(r);DLL->addLayout(r);
		connect(r->value,	&QComboBox::currentIndexChanged  , this , &tabDll::cancelOkEnable);
	}
}
void tabDll::cancel_slot(){
	cancel->setHidden(true);ok->setHidden(true);
	foreach(tabSettingsRegkey *r,regWidgets){r->reset();}
}
void tabDll::ok_slot(){
	cancel->setHidden(true);ok->setHidden(true);
	foreach(tabSettingsRegkey *r,regWidgets){r->setValue();}
	emit write();
}
void tabDll::cancelOkEnable(){
	cancel->setHidden(false);ok->setHidden(false);
}
