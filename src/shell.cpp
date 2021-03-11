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
	connect(proc, &QProcess::finished , this , &shell::exitShell);
}
shell::~shell(){
	env->~QProcessEnvironment();
	proc->deleteLater();
}
void shell::start(){
	proc->setProcessChannelMode(QProcess::MergedChannels);
	proc->setProcessEnvironment(*env);
	proc->start(exec,opt);
}
void shell::wait(int t){
	proc->waitForStarted(t);
	proc->waitForFinished(t);
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
		QSettings conf(env->value("WINEPREFIX") + "/WINE.cfg",QSettings::IniFormat);
		QString v = conf.value("WINE").toString();
		if(v != "System" && v != ""){
			exec = target->WINE_VER + "/" + v + "/bin/" + exec;
			return;
		}
	}
}
void shell::exitShell(){
	deleteLater();
}
