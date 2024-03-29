#include "sizewin.h"
#include <iostream>
using namespace std;
sizeWin::sizeWin(QWidget *w,QString n){win = w;name = n;
	settings_conf = new QSettings(
				QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).at(0) + "/wine-gui.conf",
				QSettings::IniFormat);
	#if QT_VERSION < 0x060000
		settings_conf->setIniCodec("UTF-8");
	#endif
}
sizeWin::~sizeWin(){
	settings_conf->sync();
	settings_conf->deleteLater();
}
void sizeWin::restore(){
	if(!settings_conf->contains("win/" + name)){return;}
	QList size = settings_conf->value("win/" + name).toList();
	win->move(size.at(0).toInt(),
			  size.at(1).toInt());
	win->resize(size.at(2).toInt(),
				size.at(3).toInt());
	switch ( size.at(4).toInt() ) {
	case 0:
	break;
	case 2:
		win->setWindowState(Qt::WindowState::WindowMaximized);
	break;
	case 4:
		win->setWindowState(Qt::WindowState::WindowFullScreen);
	break;
	case 6:
		win->setWindowState(Qt::WindowState::WindowFullScreen);
	break;
	case 8:
		win->setWindowState(Qt::WindowState::WindowFullScreen);
	break;
	}
}
void sizeWin::save(){
	QStringList size = settings_conf->value("win/" + name).toStringList();
	if( size.size() == 5 && win->windowState() != 0){
		if(size.at(4) != QString::number(win->windowState())){
			settings_conf->setValue("win/" + name,QVariant(QStringList()
				<< QString::number(size.at(0).toInt())
				<< QString::number(size.at(1).toInt())
				<< QString::number(size.at(2).toInt())
				<< QString::number(size.at(3).toInt())
				<< QString::number(win->windowState())
			));
		}else{
			if( size.at(0) != QString::number(win->pos().x()) ||
				size.at(1) != QString::number(win->pos().y()) ||
				size.at(2) != QString::number(win->size().width()) ||
				size.at(3) != QString::number(win->size().height())){
					settings_conf->setValue("win/" + name,QVariant(QStringList()
						<< QString::number(win->pos().x())
						<< QString::number(win->pos().y())
						<< QString::number(win->size().width())
						<< QString::number(win->size().height())
						<< QString::number(win->windowState())));
			}
		}
	}else{
		if(		size.size() < 1 ||
			size.at(0) != QString::number(win->pos().x()) ||
				size.size() < 2 ||
			size.at(1) != QString::number(win->pos().y()) ||
				size.size() < 3 ||
			size.at(2) != QString::number(win->size().width()) ||
				size.size() < 4 ||
			size.at(3) != QString::number(win->size().height()) ||
				size.size() < 5 ||
			size.at(4) != QString::number(win->windowState())){
			settings_conf->setValue("win/" + name,QVariant(QStringList()
				<< QString::number(win->pos().x())
				<< QString::number(win->pos().y())
				<< QString::number(win->size().width())
				<< QString::number(win->size().height())
				<< QString::number(win->windowState())));
		}
	}
}
