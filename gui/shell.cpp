#include "shell.h"
#include <QTextEdit>
#include <QWidget>
#include <QFile>
#include <QSettings>
#include <iostream>
using namespace std;
shell::shell(QString e,QStringList o){
	proc = new QProcess();
	env  = new QProcessEnvironment(QProcessEnvironment::systemEnvironment());
	exec = e; opt = o;
}
shell::~shell(){
	env->~QProcessEnvironment();
	proc->deleteLater();
}
void shell::run(){
	proc->setProcessChannelMode(QProcess::MergedChannels);
	proc->setProcessEnvironment(*env);
	proc->start(exec,opt);
	proc->waitForStarted(-1);
	proc->waitForFinished(-1);
	emit exit(proc);
	this->deleteLater();
}
void shell::envSetup(main_target *target){
	if(target->storage > 0 && target->prefix != ""){
		env->insert("WINEPREFIX",get<2>(target->storages.at(target->storage - 1)) + "/" + target->prefix);
		if(target->arch != ""){
			env->insert("WINEARCH",target->arch);
		}
	}else{
		env->insert("WINEPREFIX",target->home + "/.wine");
	}
	if(QFile(env->value("WINEPREFIX") + "/WINE.cfg").exists()){
		QSettings conf(env->value("WINEPREFIX") + "/WINE.cfg",QSettings::IniFormat);conf.setIniCodec( "UTF-8" );
		QString v = conf.value("WINE").toString();
		if(v != "System" && v != ""){
			exec = target->WINE_VER + "/" + v + "/bin/" + exec;
			return;
		}
	}
}
