#include "prefixtab.h"
#include "tabdll.h"
#include <QWidget>
#include <iostream>
using namespace std;
prefixTab::prefixTab(main_target *t,QWidget *parent) : QWidget(parent){
	target = t;
	tabWidget = new QTabWidget(this);
	tools = new ToolsTab(target,this);
	tabWidget->addTab(tools, tr("tools"));
	settingsTab *aTab = new settingsTab(target,reg,this);
	DX = aTab->settings->DX;
	regExist = false;
	//dllTab *bTab = new dllTab(this);
	tabWidget->addTab(aTab, tr("settings"));
	//abWidget->addTab(bTab, tr("Библиотеки"));
	connect(aTab->settings , &tabSettings::write , this, &prefixTab::write );
	//connect(bTab->dll , &tabDll::write , this, &prefixTab::write );
	connect(tabWidget, &QTabWidget::tabBarClicked , this , &prefixTab::readReg);
	connect(this , &prefixTab::updateReg , aTab->settings, &tabSettings::updateReg );
	//connect(this , &prefixTab::updateReg , bTab->dll, &tabDll::updateReg );
}
prefixTab::~prefixTab(){
	tabWidget->~QTabWidget();
}
void prefixTab::readReg(int i){
	if(i == 1){
		DX->update();
	}
	deleteReg();
	if(i > 0){
		if(target->prefix_path == ""){return;}
		reg = new fileRegistry( target->prefix_path + "/user.reg" ,QStringList()
						<< "\\[Software\\\\\\\\Wine\\\\\\\\DirectInput\\]"
						<< "\\[Software\\\\\\\\Wine\\\\\\\\Direct3D\\]"
						//<< "\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]"
						<< "\\[Software\\\\\\\\Wine\\\\\\\\DirectInput\\]"  );
		regExist = true;
		emit updateReg(reg);
	}
}
void prefixTab::write(){
	reg->write();
}

void prefixTab::deleteReg(){
	if(regExist){
		reg->deleteLater();
		regExist = false;
	}
}
ToolsTab::ToolsTab(main_target *target,QWidget *parent) : QWidget(parent)
{
	QVBoxLayout *vbox = new QVBoxLayout;vbox->setAlignment(Qt::AlignTop);
	t = new tabTools(target,this);
	l = new tabLabels(target,this);
	vbox->addWidget(t);
	vbox->addWidget(l);
	connect(l, &tabLabels::hide , this , &ToolsTab::hide);
	setLayout(vbox);
}
void ToolsTab::hide(){
	if(height() < 800){
		t->setHidden(true);l->labels->setHidden(true);
		connect(l->file, &fileDesktopWidget::destroyed , this , &ToolsTab::unhide);
	}
}
void ToolsTab::unhide(){
	t->setHidden(false);l->labels->setHidden(false);
}
dllTab::dllTab(QWidget *parent) : QWidget(parent)
{
	dll = new tabDll(this);
	QVBoxLayout *vbox = new QVBoxLayout;vbox->setAlignment(Qt::AlignTop);
	vbox->addWidget(dll);
	setLayout(vbox);
}

settingsTab::settingsTab(main_target *target,fileRegistry *r,QWidget *parent) : QWidget(parent)
{
	reg = r;
	settings = new tabSettings(target,this);
	QVBoxLayout *vbox = new QVBoxLayout;vbox->setAlignment(Qt::AlignTop);
	vbox->addWidget(settings);
	setLayout(vbox);
}

