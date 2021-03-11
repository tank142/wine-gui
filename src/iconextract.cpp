#include "iconextract.h"
#include "shelloutput.h"
#include <QProcess>
#include <QDir>
#include <QRegularExpression>
#include <iostream>
using namespace std;
#include <QPixmap>
iconExtract::iconExtract(QString n,QString s,QString d,QObject *parent) : QObject(parent)
{
	name = n;source = s;dir = d + "/icon/";
}
void iconExtract::mv(QString s,QString o,QString name){
	QDir().mkpath(o);
	QFile mv(s);
	mv.rename(o + name + mv.fileName().right(mv.fileName().size() - mv.fileName().lastIndexOf(".")));
}
void iconExtract::move(QString x){
	QStringList list = QDir(dir).entryList(QDir::Files);
	QRegularExpression r;r.setPattern("_" + x + "x");
	QRegularExpression rx;rx.setPattern( "_" + x + "x32");
	foreach(QString ico, list){
		if(rx.match(ico).hasMatch()){
			mv(dir + ico,dir + x + "/",name);return;
		}
	}
	rx.setPattern("_" + x + "x24");
	foreach(QString ico, list){
		if(rx.match(ico).hasMatch()){
			mv(dir + ico,dir + x + "/",name);return;
		}
	}
	foreach(QString ico, list){
		if(r.match(ico).hasMatch()){
			mv(dir + ico,dir + x + "/",name);return;
		}
	}
}
void iconExtract::start(){
	QDir d;d.mkpath(dir);d.setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Hidden);d.setPath(dir);
	QProcess *proc = new QProcess(this);
	proc->setProcessEnvironment(QProcessEnvironment(QProcessEnvironment::systemEnvironment()));
	proc->start("wrestool",QStringList() << "-t" << "14" << "-x" << source << "-o" << dir + "/icon.ico");proc->waitForFinished(-1);
	proc->start("icotool",QStringList() << "-o" << dir << "-x" << dir + "/icon.ico");proc->waitForFinished(-1);
	if(proc->exitCode() == 1){
		proc->start("convert",QStringList() << dir + "/icon.ico" << dir + "/icon.ico");proc->waitForFinished(-1);
		if(proc->exitCode() == 0){
			proc->start("icotool",QStringList() << "-o" << dir << "-x" << dir + "/icon.ico");proc->waitForFinished(-1);
		}
	}
	QFile(dir + "/icon.ico").remove();
	foreach(QString size,QStringList() << "256x256" << "192x192" << "160x160" << "150x150"
			<< "128x128" << "96x96" << "80x80" << "72x72" << "64x64" << "60x60" << "48x96"
			<< "48x48" << "41x47" << "40x40" << "36x36" << "32x32" << "24x24" << "22x22"
			<< "20x20" << "16x16" << "14x14" << "13x256" << "13x13" << "10x10" << "8x8"){
		move(size);
	}
	foreach(QString ico, QDir(dir).entryList(QDir::Files)){
		QFile::remove(dir + ico);
	}
}
