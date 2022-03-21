#include "tabsyncsettings.h"
#include "fileshell.h"
#include <QHBoxLayout>
#include <QSettings>
#include <QDir>
#include <iostream>
using namespace std;
tabsyncSettings::tabsyncSettings(main_target *t,QWidget *parent) : QGroupBox{parent} {
	target = t;
	setTitle(tr("sync"));
	QHBoxLayout *HBOX = new QHBoxLayout();
	wineserver = new QRadioButton("wineserver");HBOX->addWidget(wineserver);
	esync = new QRadioButton("esync");HBOX->addWidget(esync);
	fsync = new QRadioButton("fsync");HBOX->addWidget(fsync);
	update();
	setLayout(HBOX);
}
void tabsyncSettings::update(){
	disconnect(wineserver,	&QRadioButton::clicked , this , &tabsyncSettings::wineserverSlot);
	disconnect(esync,		&QRadioButton::clicked , this , &tabsyncSettings::esyncSlot);
	disconnect(fsync,		&QRadioButton::clicked , this , &tabsyncSettings::fsyncSlot);
	QSettings conf(target->prefix_path + "/WINE.cfg",QSettings::IniFormat);
	#if QT_VERSION < 0x060000
		conf.setIniCodec("UTF-8");
	#endif
	QString SYNC = conf.value("SYNC").toString();
	if(SYNC == "ESYNC"){
		esync->setChecked(true);
	}else{
		if(SYNC == "FSYNC"){
			fsync->setChecked(true);
		}else{
			wineserver->setChecked(true);
		}
	}
	connect(wineserver, &QRadioButton::clicked , this , &tabsyncSettings::wineserverSlot);
	connect(esync,		&QRadioButton::clicked , this , &tabsyncSettings::esyncSlot);
	connect(fsync,		&QRadioButton::clicked , this , &tabsyncSettings::fsyncSlot);
}
void tabsyncSettings::wineserverSlot(){
	QSettings conf(target->prefix_path + "/WINE.cfg",QSettings::IniFormat);
	#if QT_VERSION < 0x060000
		conf.setIniCodec("UTF-8");
	#endif
	conf.setValue("SYNC","WINESERVER");
	updateFileshells(0);
}

void tabsyncSettings::esyncSlot(){
	QSettings conf(target->prefix_path + "/WINE.cfg",QSettings::IniFormat);
	#if QT_VERSION < 0x060000
		conf.setIniCodec("UTF-8");
	#endif
	conf.setValue("SYNC","ESYNC");
	updateFileshells(1);
}

void tabsyncSettings::fsyncSlot(){
	QSettings conf(target->prefix_path + "/WINE.cfg",QSettings::IniFormat);
	#if QT_VERSION < 0x060000
		conf.setIniCodec("UTF-8");
	#endif
	conf.setValue("SYNC","FSYNC");
	updateFileshells(2);
}
void tabsyncSettings::updateFileshells(char sync){
	QDir dir;dir.setPath(target->prefix_path + "/shortcuts");dir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Hidden);
	foreach (QString shortcut,dir.entryList()) {
		fileShell *sh = new fileShell(target->prefix_path + "/shortcuts/" + shortcut);
		switch (sync) {
		case 0:
			sh->WINEESYNC=false;
			sh->WINEFSYNC=false;
			target->sync=0;
		break;
		case 1:
			sh->WINEESYNC=true;
			sh->WINEFSYNC=false;
			target->sync=1;
		break;
		case 2:
			sh->WINEESYNC=false;
			sh->WINEFSYNC=true;
			target->sync=2;
		break;
		}
		sh->write();sh->deleteLater();
	}
}
