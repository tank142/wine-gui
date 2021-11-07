#include "fileshell.h"
#include <QTextStream>
#include <QRegularExpression>
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
		if( EXEC.size() == 0 ){
			t.seek(0);
			QString l;
			while(!t.atEnd()){
				existsEXEC=false;
				l = t.readLine();
				if(QRegularExpression("^.*\"\\$WINE\"").match(l).hasMatch()){
					EXEC = l.remove(QRegularExpression("^.*exec "));break;
				}
			}
		}else{ existsEXEC=true; }
		FILE->close();
		QRegularExpression rx("gamemoderun ");
		if(rx.match(EXEC).hasMatch()){
			EXEC.remove(rx);
			gamemoderun = true;
		}
		rx.setPattern("strangle ");
		if(rx.match(EXEC).hasMatch()){
			EXEC.remove(rx);
			STRANGLE_enable = true;
		}else{
			STRANGLE_enable = false;
		}
		rx.setPattern("mangohud ");
		if(rx.match(EXEC).hasMatch()){
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
	if(QRegularExpression("^#").match(t).hasMatch()){return false;}
	return true;
}
void fileShell::line(QString l){
	if( l == "" ){return;}
	if(check(l) && QRegularExpression("WINEESYNC").match(l).hasMatch()){
		if(QRegularExpression("1").match(l).hasMatch()){
			WINEESYNC = true;
		}else{
			WINEESYNC = false;
		}
		return;
	}
	if(check(l) && QRegularExpression("WINEFSYNC").match(l).hasMatch()){
		if(QRegularExpression("1").match(l).hasMatch()){
			WINEFSYNC = true;
		}else{
			WINEFSYNC = false;
		}
		return;
	}
	if(check(l) && QRegularExpression("^.*export ENABLE_VKBASALT=1").match(l).hasMatch()){
		if(QRegularExpression("1").match(l).hasMatch()){
			VKBASALT = true;
		}else{
			VKBASALT = false;
		}
		return;
	}
	if(QRegularExpression("#STRANGLE").match(l).hasMatch()){
		if(check(l)){
			STRANGLE_enable = true;
		}else{
			STRANGLE_enable = false;
		}
		STRANGLE = l.remove(QRegularExpression("^.*export "));
		STRANGLE.remove(QRegularExpression("#STRANGLE*$"));
		STRANGLE.remove(QRegularExpression("^#"));
		STRANGLE.remove(QRegularExpression(" $"));
		return;
	}
	if(QRegularExpression("^.*EXE=").match(l).hasMatch()){
		EXE = l.remove(QRegularExpression("^.*EXE="));
		EXE.remove(QRegularExpression("^\""));
		EXE.remove(QRegularExpression("\"$"));
		return;
	}
	if(QRegularExpression("^.*WORKDIR=").match(l).hasMatch()){
		if(check(l)){
			WORKDIR = l.remove(QRegularExpression("^.*WORKDIR=\""));
			WORKDIR.remove(QRegularExpression("\"$"));
		}
		return;
	}
	if(check(l) && QRegularExpression("^.*WINE").match(l).hasMatch()){
		if(QRegularExpression("1").match(l).hasMatch()){
			WINE = l.remove(QRegularExpression("^.*WINE"));
			WINE.remove(QRegularExpression("\"$"));
			return;
		}
	}
	if(QRegularExpression("^.*DXVK_HUD=").match(l).hasMatch()){
		if(check(l)){
			DXVK_HUD_enable = true;
		}else{
			DXVK_HUD_enable = false;
		}
		DXVK_HUD = l.remove(QRegularExpression("^.*export DXVK_HUD="));
		DXVK_HUD.remove(QRegularExpression("^#"));
		return;
	}
	if(QRegularExpression("^.*GALLIUM_HUD=").match(l).hasMatch()){
		if(check(l)){
			GALLIUM_HUD_enable = true;
		}else{
			GALLIUM_HUD_enable = false;
		}
		GALLIUM_HUD = l.remove(QRegularExpression("^.*export GALLIUM_HUD="));
		GALLIUM_HUD.remove(QRegularExpression("^#"));
		return;
	}
	if(QRegularExpression("^.*MANGOHUD_CONFIG=").match(l).hasMatch()){
		if(check(l)){
			MANGOHUD_CONFIG_enable = true;
		}else{
			MANGOHUD_CONFIG_enable = false;
		}
		MANGOHUD_CONFIG = l.remove(QRegularExpression("^.*export MANGOHUD_CONFIG="));
		MANGOHUD_CONFIG.remove(QRegularExpression("^#"));
		return;
	}
	if(QRegularExpression("^.*exec ").match(l).hasMatch()){
		EXEC = l.remove(QRegularExpression("^.*exec "));return;
	}
	return;
}
void fileShell::write(){
	QStringList list;
	QStringList list2;
	QTextStream file(FILE);
	if(FILE->open(QFile::ReadWrite)){
		QRegularExpression rx1("exec ");
		QRegularExpression rx2("export WINEPREFIX=");
		QRegularExpression rx3("WORKDIR=");
		QRegularExpression rx4("WINE=");
		QRegularExpression rx5("EXE=");
		QRegularExpression rx6("cd \"\\$WORKDIR\"");
		QRegularExpression rx7("WINEESYNC=");
		QRegularExpression rx8("WINEFSYNC=");
		QRegularExpression rx9("DXVK_HUD=");
		QRegularExpression rx10("GALLIUM_HUD=");
		QRegularExpression rx11("MANGOHUD_CONFIG=");
		QRegularExpression rx12("STRANGLE");
		QRegularExpression rx13("^.*\"\\$WINE\"");
		QRegularExpression rx14("export ENABLE_VKBASALT=1");
		while(!file.atEnd()){
			QString l = file.readLine();
			if(	rx1.match(l).hasMatch() ||
				rx2.match(l).hasMatch() ||
				rx3.match(l).hasMatch() ||
				rx4.match(l).hasMatch() ||
				rx5.match(l).hasMatch() ||
				rx6.match(l).hasMatch() ||
				rx7.match(l).hasMatch() ||
				rx8.match(l).hasMatch() ||
				rx9.match(l).hasMatch() ||
				rx10.match(l).hasMatch() ||
				rx11.match(l).hasMatch() ||
				rx12.match(l).hasMatch() ||
				rx13.match(l).hasMatch() ||
				rx14.match(l).hasMatch()) {
				continue;
			}
			list2.append(l);
		}
		FILE->resize(0);
	}
	if(	list2.size() == 0 || !QRegularExpression("^#\\!").match(list2.at(0)).hasMatch() ){
		list.insert(0,"#!/bin/sh");
	}else{
		list.insert(0,list2.at(0));
		list2.removeAt(0);
	}
	list.append("export WINEPREFIX=\"${PWD%/*/*}\"");
	list.append("WORKDIR=\"" + WORKDIR + QString("\""));
	list.append("cd " + QString("\"") + "$WORKDIR" + QString("\""));
	if(WINE.size() > 0 ){
		list.append("WINE=" + QString("\"") + WINE + QString("\""));
	}else{
		list.append("WINE=\"wine\"");
	}
	list.append("EXE=" + QString("\"") + EXE + QString("\""));
	if(WINEESYNC){list.append("export WINEESYNC=1");}
	if(WINEFSYNC){list.append("export WINEFSYNC=1");}
	if(VKBASALT){list.append("export ENABLE_VKBASALT=1");}
	if(DXVK_HUD.size() > 0){
		if(DXVK_HUD_enable){
			list.append("export DXVK_HUD=" + DXVK_HUD);
		}else{
			list.append("#export DXVK_HUD=" + DXVK_HUD);
		}
	}
	if(GALLIUM_HUD.size() > 0){
		if(GALLIUM_HUD_enable){
			list.append("export GALLIUM_HUD=" + GALLIUM_HUD);
		}else{
			list.append("#export GALLIUM_HUD=" + GALLIUM_HUD);
		}
	}
	if(MANGOHUD_CONFIG.size() > 0){
		if(MANGOHUD_CONFIG_enable){
			list.append("export MANGOHUD_CONFIG=" + MANGOHUD_CONFIG);
		}else{
			list.append("#export MANGOHUD_CONFIG=" + MANGOHUD_CONFIG);
		}
	}
	if(STRANGLE.size() > 0){
		if(STRANGLE_enable){
			list.append("export " + STRANGLE + " #STRANGLE");
		}else{
			list.append("#export " + STRANGLE + " #STRANGLE");
		}
	}
	foreach(QString l,list){if(l != ""){
		file.operator<<(QString::fromUtf8( l.toUtf8() + "\n" ));}}
	QString exec;
	if(gamemoderun){exec.append("gamemoderun ");}
	if(STRANGLE_enable){exec.append("strangle ");}
	if(MANGOHUD_CONFIG_enable){exec.append("mangohud ");}
	foreach(QString l,list2){if(l != ""){
		file.operator<<(QString::fromUtf8( l.toUtf8() + "\n" ));}}
	if(existsEXEC){
		file.operator<<( "exec " + exec + EXEC + "\n");
	}else{
		file.operator<<( exec + EXEC + "\n");
	}
	FILE->close();
	FILE->setPermissions(FILE->permissions() | QFileDevice::ExeOwner | QFileDevice::ExeUser | QFileDevice::ExeGroup | QFileDevice::ExeOther);
}
