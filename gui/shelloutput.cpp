#include "shelloutput.h"
#include <QLabel>
shellOutputErrCk::shellOutputErrCk(QString e,QStringList o,QWidget *parent) : QWidget(parent){
	exec = new shell(e,o);
}
void shellOutputErrCk::start(){
	vbox = new QVBoxLayout(this);
	connect(exec, &shell::exit , this , &shellOutputErrCk::output);
	setLayout(vbox);
	this->show();
	this->hide();
	exec->start();
}
void shellOutputErrCk::output(QProcess *proc){
	if(proc->exitCode() != 0){
		QTextEdit *text = new QTextEdit(this);
		QString cmd = proc->program() + " ";
		foreach(QString arg,proc->arguments()){
			cmd.append(arg + " ");
		}
		QLabel *l = new QLabel(cmd,this);
		l->setTextInteractionFlags(Qt::TextSelectableByMouse);
		text->setTextInteractionFlags(Qt::TextSelectableByMouse);
		vbox->addWidget(l);
		text->setText(proc->readAllStandardOutput());
		vbox->addWidget(text);
		setWindowTitle("Ошибка");
		this->show();
	}else{
		this->deleteLater();
	}
}
shellOutputDebugging::shellOutputDebugging(QString e,QStringList o,QWidget *parent) : QWidget(parent){
	exec = new shell(e,o);
	connect(exec->proc, &QProcess::finished , this , &shellOutputDebugging::exit);
}
void shellOutputDebugging::start(){
	QVBoxLayout *vbox = new QVBoxLayout(this);
	text = new QTextEdit(this);
	text->setTextInteractionFlags(Qt::TextSelectableByMouse);
	vbox->addWidget(text);
	setWindowTitle("Отладка");
	setLayout(vbox);
	this->show();
	connect(exec->proc, &QProcess::readyRead , this , &shellOutputDebugging::addText);
	exec->start();
	text->moveCursor(QTextCursor::End);
}

void shellOutputDebugging::addText(){
	text->insertPlainText(exec->proc->readAllStandardOutput());
	text->moveCursor(QTextCursor::End);
}
void shellOutputDebugging::exit(int code){
	text->insertPlainText("Код завершения: " + QString::number(code));
	text->moveCursor(QTextCursor::End);
}
