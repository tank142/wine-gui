#include "filedesktop.h"
#include <QRegExp>
#include <QTextStream>
//#include <QText
#include <iostream>
using namespace std;
fileDesktop::fileDesktop(QString f,QObject *parent) : QObject(parent){
	FILE = new QFile(f + "/exec.desktop",this);
	if (FILE->exists()){
		FILE->open(QFile::ReadOnly);
		QTextStream t(FILE);
		while(!t.atEnd()){
		  line(t.readLine());
		}
		FILE->close();
	}
}
void fileDesktop::line(QString l){
	QRegExp rx("^Name=");
	if(!rx.indexIn(l)){Name = l.remove(rx);return;}
	rx.setPattern("^Exec=");
	if(!rx.indexIn(l)){Exec = l.remove(rx);return;}
	rx.setPattern("^Comment=");
	if(!rx.indexIn(l)){Comment = l.remove(rx);return;}
	rx.setPattern("^Path=");
	if(!rx.indexIn(l)){Path = l.remove(rx);return;}
	rx.setPattern("^Icon=");
	if(!rx.indexIn(l)){Icon = l.remove(rx);return;}
	rx.setPattern("^Categories=");
	if(!rx.indexIn(l)){Categories = l.remove(rx);return;}
	rx.setPattern("^StartupWMClass=");
	if(!rx.indexIn(l)){StartupWMClass = l.remove(rx);return;}
	return;
}
bool fileDesktop::check_type(QStringList list){
	bool check = false;
	foreach(QString l,list){
		if(!QRegExp("^Type=").indexIn(l)){check = true;break;}
	}
	return check;
}
void fileDesktop::write(){
	FILE->open(QFile::ReadWrite);QTextStream file(FILE);
	QStringList list;
	while(!file.atEnd()){
		list.append(file.readLine());
	}
	FILE->resize(0);
	if(list.size() == 0 || !QRegExp("^\[").indexIn(list.at(0))){
		list.insert(0,"[Desktop Entry]");
	}
	if(!check_type(list)){list.append("Type=Application");}
	foreach(QString l,list){
		if(l != ""){
			QRegExp rx("^Name=");
			if(!rx.indexIn(l)){continue;}
			rx.setPattern("^Exec=");
			if(!rx.indexIn(l)){continue;}
			rx.setPattern("^Comment=");
			if(!rx.indexIn(l)){continue;}
			rx.setPattern("^Path=");
			if(!rx.indexIn(l)){continue;}
			rx.setPattern("^Icon=");
			if(!rx.indexIn(l)){continue;}
			rx.setPattern("^Categories=");
			if(!rx.indexIn(l)){continue;}
			rx.setPattern("^StartupWMClass=");
			if(!rx.indexIn(l)){continue;}
			file.operator<<(l + "\n");
		}
	}
	file.operator<<("Name=" + Name + "\n");
	file.operator<<("Comment=" + Comment + "\n");
	file.operator<<("Exec=" + Exec + "\n");
	file.operator<<("Path=" + Path + "\n");
	file.operator<<("Icon=" + Icon + "\n");
	if(QRegExp(";$").indexIn(Categories) == -1){Categories.append(";");}
	file.operator<<("Categories=" + Categories + "\n");
	file.operator<<("StartupWMClass=" + StartupWMClass + "\n");
	FILE->close();
	FILE->setPermissions(FILE->permissions() | QFileDevice::ExeOwner | QFileDevice::ExeUser | QFileDevice::ExeGroup | QFileDevice::ExeOther);
}
