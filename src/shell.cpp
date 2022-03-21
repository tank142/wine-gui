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
	if(target->storage > 0 && target->prefix.size() > 0){
		env->insert("WINEPREFIX",get<2>(target->storages.at(target->storage - 1)) + "/" + target->prefix);
		if(target->arch != ""){
			env->insert("WINEARCH",target->arch);
		}
	}else{
		env->insert("WINEPREFIX",target->home + "/.wine");
	}
	if(target->WINE.size() > 0){
		exec = target->WINE_VER + "/" + target->WINE + "/bin/" + exec;
	}
	switch (target->sync) {
	case 1:
		env->insert("WINEESYNC","1");
	break;
	case 2:
		env->insert("WINEFSYNC","1");
	break;
	}
}
void shell::exitShell(){
	deleteLater();
}
