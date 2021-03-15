#include "sizewin.h"
#include <iostream>
using namespace std;

sizeWin::sizeWin(QWidget *w,QString n){win = w;name = n;
	settings_conf = new QSettings(
				QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).at(0) + "/wine-gui.conf",
				QSettings::IniFormat);
}
sizeWin::~sizeWin(){
	settings_conf->sync();
	settings_conf->deleteLater();
}
void sizeWin::restore(){
	if(!settings_conf->contains("main/" + name)){return;}
	QList size = settings_conf->value("main/" + name).toList();
	if(size.size() > 4){
		switch ( size.at(4).toInt() ) {
		case 0:
			win->move(size.at(0).toInt(),
					  size.at(1).toInt());
			win->resize(size.at(2).toInt(),
						size.at(3).toInt());
		break;
		case 2:
			win->setWindowState(Qt::WindowState::WindowMaximized);
		break;
		case 8:
			win->setWindowState(Qt::WindowState::WindowFullScreen);
		break;
		}
	}
	cout << size.at(4).toInt() << endl;
}
void sizeWin::save(){
	settings_conf->setValue("main/" + name,QVariant(QStringList()
		<< QString::number(win->pos().x())
		<< QString::number(win->pos().y())
		<< QString::number(win->size().width())
		<< QString::number(win->size().height())
		<< QString::number(win->windowState())
	));
	cout << win->windowState() << endl;
}