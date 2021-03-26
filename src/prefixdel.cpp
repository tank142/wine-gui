#include "prefixdel.h"
#include "findlink.h"
#include <QStandardPaths>
#include <QDir>
#include <QLabel>
using namespace std;
threadDel::threadDel(QString d){
	dir = new QDir;
	dir->setPath(d);
	connect(this, &threadDel::finished , this , &threadDel::deleteLater);
}
threadDel::~threadDel(){
	dir->~QDir();
}
void threadDel::run(){
	findLink(dir->path() + "/shortcuts/",QDir::homePath() + "/.local/share/applications").rmRc();
	findLink(dir->path() + "/shortcuts/",QDir::homePath() + "/.local/share/icons").rmRc();
	findLink(dir->path() + "/shortcuts/",QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)).rm();
	dir->removeRecursively();
	emit model_update();
}
void threadDel::work(){
	start();
}
prefixDel::prefixDel(threadDel*,QString text,QWidget *parent) : QWidget(parent)
{
	vbox = new QVBoxLayout();
	hbox = new QHBoxLayout();
	vbox->addWidget(new QLabel(tr("del") + text,this));
	vbox->addLayout(hbox);
	ok = new QPushButton(tr("ok"),this);
	QPushButton *cancel = new QPushButton(tr("cancel"),this);
	hbox->addWidget(ok);hbox->addWidget(cancel);
	setLayout(vbox);
	connect(ok, &QPushButton::clicked , this , &prefixDel::ok_slot);
	connect(cancel, &QPushButton::clicked , this , &prefixDel::cancel_slot);
}
void prefixDel::ok_slot(){
	emit start();
	cancel_slot();
}
void prefixDel::cancel_slot(){
	emit main_tool_widget_hide(false);
	this->deleteLater();
}
