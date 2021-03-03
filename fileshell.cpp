#include "fileshell.h"
#include <QTextStream>
#include <iostream>
using namespace std;
fileShell::fileShell(QString f,QObject *parent) : QObject(parent)
{
	FILE = new QFile(f + "/exec.sh",this);
	if (FILE->exists()){
		FILE->open(QFile::ReadOnly);
		QTextStream t(FILE);
		while(!t.atEnd()){
		  line(t.readLine());
		}
		FILE->close();
		QRegExp rx("gamemoderun ");
		if(rx.indexIn(EXEC) > -1){
			EXEC.remove(rx);
			gamemoderun = true;
		}
		rx.setPattern("strangle ");
		if(rx.indexIn(EXEC) > -1){
			EXEC.remove(rx);
			STRANGLE_enable = true;
		}else{
			STRANGLE_enable = false;
		}
		rx.setPattern("mangohud ");
		if(rx.indexIn(EXEC) > -1){
			EXEC.remove(rx);
			MANGOHUD_CONFIG_enable = true;
		}else{
			MANGOHUD_CONFIG_enable = false;
		}
	}else{
		EXEC = "\"$WINE\" \"$EXE\"";
	}
}
bool fileShell::check(QString t){
	if(!QRegExp("^#").indexIn(t)){return false;}
	return true;
}
void fileShell::line(QString l){
	if( l == "" ){return;}
	if(check(l) && QRegExp("^*WINEESYNC").indexIn(l) > -1){
		if(QRegExp("1").indexIn(l) > -1){
			WINEESYNC = true;
		}else{
			WINEESYNC = false;
		}
		return;
	}
	if(check(l) && QRegExp("^*WINEFSYNC").indexIn(l) > -1){
		if(QRegExp("1").indexIn(l) > -1){
			WINEFSYNC = true;
		}else{
			WINEFSYNC = false;
		}
		return;
	}
	if(QRegExp("#STRANGLE").indexIn(l) > -1){
		if(check(l)){
			STRANGLE_enable = true;
		}else{
			STRANGLE_enable = false;
		}
		STRANGLE = l.remove(QRegExp("^*export "));
		STRANGLE.remove(QRegExp("#STRANGLE*$"));
		STRANGLE.remove(QRegExp("^#"));
		return;
	}
	if(!QRegExp("^*EXE=").indexIn(l)){
		EXE = l.remove(QRegExp("^*EXE="));
		EXE.remove(QRegExp("^\""));
		EXE.remove(QRegExp("\"$"));
		return;
	}
	if(!QRegExp("WORKDIR=").indexIn(l)){
		if(check(l)){
			WORKDIR = l.remove(QRegExp("^*WORKDIR="));
			WORKDIR.remove(QRegExp("^\""));
			WORKDIR.remove(QRegExp("\"$"));
		}
		return;
	}
	if(check(l) && !QRegExp("WINE").indexIn(l)){
		if(!QRegExp("1").indexIn(l)){WINE = l.remove("*WINE=");return;}
	}
	if(QRegExp("DXVK_HUD=").indexIn(l) > -1){
		if(check(l)){
			DXVK_HUD_enable = true;
		}else{
			DXVK_HUD_enable = false;
		}
		DXVK_HUD = l.remove(QRegExp("^*export DXVK_HUD="));
		DXVK_HUD.remove(QRegExp("^#"));
		return;
	}
	if(QRegExp("GALLIUM_HUD=").indexIn(l) > -1){
		if(check(l)){
			GALLIUM_HUD_enable = true;
		}else{
			GALLIUM_HUD_enable = false;
		}
		GALLIUM_HUD = l.remove(QRegExp("^*export GALLIUM_HUD="));
		GALLIUM_HUD.remove(QRegExp("^#"));
		return;
	}
	if(QRegExp("MANGOHUD_CONFIG=").indexIn(l) > -1){
		if(check(l)){
			MANGOHUD_CONFIG_enable = true;
		}else{
			MANGOHUD_CONFIG_enable = false;
		}
		MANGOHUD_CONFIG = l.remove(QRegExp("^*export MANGOHUD_CONFIG="));
		MANGOHUD_CONFIG.remove(QRegExp("^#"));
		return;
	}
	if(!QRegExp("^*exec ").indexIn(l)){
		EXEC = l.remove(QRegExp("^*exec "));return;
	}
	return;
}
void fileShell::write(){
	FILE->open(QFile::ReadWrite);QTextStream file(FILE);file.setCodec("UTF-8");
	QStringList list;
	FILE->resize(0);
	if(	list.size() == 0 || !QRegExp("^#!").indexIn(list.at(0)) ){
		list.insert(0,"#!/bin/sh");
	}
	list.append("export WINEPREFIX=\"${DIRSTACK%/*/*}\"");
	list.append("WORKDIR=\"" + WORKDIR + QString("\""));
	if(WINE != "" ){list.append("WINE=" + QString("\"") + WINE + QString("\""));}else{
		list.append("WINE=\"wine\"");
	}
	list.append("EXE=" + QString("\"") + EXE + QString("\""));
	list.append("cd " + QString("\"") + "$WORKDIR" + QString("\""));
	if(WINEESYNC){list.append("export WINEESYNC=1");}
	if(WINEFSYNC){list.append("export WINEFSYNC=1");}
	if(DXVK_HUD_enable){list.append("export DXVK_HUD=" + DXVK_HUD);}else{
		if(DXVK_HUD != ""){
			list.append("#export DXVK_HUD=" + DXVK_HUD);
		}
	}
	if(GALLIUM_HUD_enable){list.append("export GALLIUM_HUD=" + GALLIUM_HUD);}else{
		if(GALLIUM_HUD != ""){
			list.append("#export GALLIUM_HUD=" + GALLIUM_HUD);
		}
	}
	if(MANGOHUD_CONFIG_enable){list.append("export MANGOHUD_CONFIG=" + MANGOHUD_CONFIG);}else{
		if(MANGOHUD_CONFIG != ""){
			list.append("#export MANGOHUD_CONFIG=" + MANGOHUD_CONFIG);
		}
	}
	if(STRANGLE_enable){list.append("export " + STRANGLE + "#STRANGLE");}else{
		if(STRANGLE != ""){
			list.append("#export " + STRANGLE + "#STRANGLE");
		}
	}
	while(!file.atEnd()){
		QString l = file.readLine();
		if(	QRegExp("exec").indexIn(l) &&
			QRegExp("WINEESYNC=").indexIn(l) &&
			QRegExp("WINEFSYNC=").indexIn(l) &&
			QRegExp("DXVK_HUD=").indexIn(l) &&
			QRegExp("GALLIUM_HUD=").indexIn(l) &&
			QRegExp("MANGOHUD_CONFIG=").indexIn(l) &&
			QRegExp("STRANGLE").indexIn(l)){
			list.append(l);
			continue;
		}
		list.append(l);
	}
	foreach(QString l,list){if(l != ""){
		file.operator<<(QString::fromUtf8( l.toUtf8() + "\n" ));}}
	QString exec;
	if(gamemoderun){exec.append("gamemoderun ");}
	if(STRANGLE_enable){exec.append("strangle ");}
	if(MANGOHUD_CONFIG_enable){exec.append("mangohud ");}
	file.operator<<( "exec " + exec + EXEC + "\n");

	FILE->close();
	FILE->setPermissions(FILE->permissions() | QFileDevice::ExeOwner | QFileDevice::ExeUser | QFileDevice::ExeGroup | QFileDevice::ExeOther);
}
