#include "shelloutput.h"
#include "sizewin.h"
#include <QLabel>
shellOutputErrCk::shellOutputErrCk(QString e,QStringList o,QWidget *parent) : QWidget(parent){
	setAttribute(Qt::WA_DeleteOnClose);
	exec = new shell(e,o);
	sizeWin(this,"winOutputDebugging").restore();
}
shellOutputErrCk::~shellOutputErrCk(){
	sizeWin(this,"winOutputDebugging").save();
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
		setWindowTitle(tr("err"));
		this->show();
	}else{
		this->deleteLater();
	}
}
shellOutputDebugging::shellOutputDebugging(QString e,QStringList o,QWidget *parent) : QWidget(parent){
	setAttribute(Qt::WA_DeleteOnClose);
	exec = new shell(e,o);
	connect(exec->proc, &QProcess::finished , this , &shellOutputDebugging::exit);
	sizeWin(this,"winOutputDebugging").restore();
}
shellOutputDebugging::~shellOutputDebugging(){
	sizeWin(this,"winOutputDebugging").save();
}
void shellOutputDebugging::start(){
	QVBoxLayout *vbox = new QVBoxLayout(this);
	text = new QTextEdit(this);
	text->setTextInteractionFlags(Qt::TextSelectableByMouse);
	vbox->addWidget(text);
	setWindowTitle(tr("debug"));
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
	text->insertPlainText(tr("exit_code") + QString::number(code));
	text->moveCursor(QTextCursor::End);
}
