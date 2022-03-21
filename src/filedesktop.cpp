#include "filedesktop.h"
#include <QRegularExpression>
#include <QTextStream>
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
	QRegularExpression rx("^Name=");
	if(rx.match(l).hasMatch()){Name = l.remove(rx);return;}
	rx.setPattern("^Exec=");
	if(rx.match(l).hasMatch()){Exec = l.remove(rx);return;}
	rx.setPattern("^Comment=");
	if(rx.match(l).hasMatch()){Comment = l.remove(rx);return;}
	rx.setPattern("^Path=");
	if(rx.match(l).hasMatch()){Path = l.remove(rx);return;}
	rx.setPattern("^Icon=");
	if(rx.match(l).hasMatch()){Icon = l.remove(rx);return;}
	rx.setPattern("^Categories=");
	if(rx.match(l).hasMatch()){Categories = l.remove(rx);return;}
	rx.setPattern("^StartupWMClass=");
	if(rx.match(l).hasMatch()){StartupWMClass = l.remove(rx);return;}
	return;
}
bool fileDesktop::check_type(QStringList list){
	bool check = false;
	foreach(QString l,list){
		if(QRegularExpression("^Type=").match(l).hasMatch()){check = true;break;}
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
	if(list.size() == 0 || !QRegularExpression("^\\[").match(list.at(0)).hasMatch()){
		list.insert(0,"[Desktop Entry]");
	}
	if(!check_type(list)){list.append("Type=Application");}
	foreach(QString l,list){
		if(l != ""){
			QRegularExpression rx("^Name=");
			if(rx.match(l).hasMatch()){continue;}
			rx.setPattern("^Exec=");
			if(rx.match(l).hasMatch()){continue;}
			rx.setPattern("^Comment=");
			if(rx.match(l).hasMatch()){continue;}
			rx.setPattern("^Path=");
			if(rx.match(l).hasMatch()){continue;}
			rx.setPattern("^Icon=");
			if(rx.match(l).hasMatch()){continue;}
			rx.setPattern("^Categories=");
			if(rx.match(l).hasMatch()){continue;}
			rx.setPattern("^StartupWMClass=");
			if(rx.match(l).hasMatch()){continue;}
			file.operator<<(l + "\n");
		}
	}
	file.operator<<("Name=" + Name + "\n");
	file.operator<<("Comment=" + Comment + "\n");
	file.operator<<("Exec=" + Exec + "\n");
	file.operator<<("Path=" + Path + "\n");
	file.operator<<("Icon=" + Icon + "\n");
	if(!QRegularExpression(";$").match(Categories).hasMatch()){Categories.append(";");}
	file.operator<<("Categories=" + Categories + "\n");
	file.operator<<("StartupWMClass=" + StartupWMClass + "\n");
	FILE->close();
	FILE->setPermissions(FILE->permissions() | QFileDevice::ExeOwner | QFileDevice::ExeUser | QFileDevice::ExeGroup | QFileDevice::ExeOther);
}
