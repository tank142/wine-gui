#include "findlink.h"
#include <QDir>
#include <QRegExp>
findLink::findLink(QString t,QString d,QObject *parent) : QObject(parent){
target = t; dir = d;}
bool findLink::ck(){
	QDir d(dir);d.setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
	foreach(QString file,d.entryList()){
		QString link = QFile(dir + "/" + file).symLinkTarget();
		if(QRegExp(target).indexIn(link)  > -1){
			return true;
		}
	}
	return false;
}
bool findLink::ckRc(){
	if(ckrc(dir)){
		return true;
	}else{
		return false;
	}
}
bool findLink::ckrc(QString dir){
	QDir d(dir);d.setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
	foreach(QString file,d.entryList()){
		QString link = QFile(dir + "/" + file).symLinkTarget();
		if(QRegExp(target).indexIn(link)  > -1){
			return true;
		}
		if(link == "" && QDir(dir + "/" + file).exists() ){
			if(ckrc(dir + "/" + file)){
				return true;
			}
		}
	}
	return false;
}
void findLink::rm(){
	links.clear();
	QDir d(dir);d.setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
	foreach(QString file,d.entryList()){
		QString link = QFile(dir + "/" + file).symLinkTarget();
		if(QRegExp(target).indexIn(link)  > -1){
			links.append(dir + "/" + file);
			continue;
		}
	}
	foreach(QString file,links){QFile(file).remove();}
}
void findLink::rmRc(){
	links.clear();
	find(dir);
	foreach(QString file,links){QFile(file).remove();}
}
void findLink::lnMove(QString mv){
	for(int i = 0;links.size() > i;i++){
		QFile(links.at(i)).remove();
		QFile().link( QString(linksTargets.at(i)).replace(QRegExp(target) ,mv), links.at(i) );
	}
}
void findLink::lnFind(){
	links.clear();linksTargets.clear();
	QDir d(dir);d.setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
	foreach(QString file,d.entryList()){
		QString link = QFile(dir + "/" + file).symLinkTarget();
		if(QRegExp(target).indexIn(link) > -1){
			links.append(dir + "/" + file);
			linksTargets.append(link);
		}
	}
}
void findLink::lnFindRc(){
	links.clear();linksTargets.clear();
	findLn(dir);
}
void findLink::findLn(QString dd){
	QDir d(dd);d.setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
	foreach(QString file,d.entryList()){
		QString link = QFile(dd + "/" + file).symLinkTarget();
		if(QRegExp(target).indexIn(link) > -1){
			links.append(dd + "/" + file);
			linksTargets.append(link);
		}
		if(link == "" && QDir(dd + "/" + file).exists() ){
			findLn(dd + "/" + file);
		}
	}
}
void findLink::find(QString dir){
	QDir d(dir);d.setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
	foreach(QString file,d.entryList()){
		QString link = QFile(dir + "/" + file).symLinkTarget();
		if(QRegExp(target).indexIn(link)  > -1){
			links.append(dir + "/" + file);
			continue;
		}
		if(link == "" && QDir(dir + "/" + file).exists() ){
			find(dir + "/" + file);
		}
	}
}
