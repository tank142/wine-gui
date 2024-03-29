#include "mainwindow.h"
#include "settings.h"
#include "prefixadd.h"
#include "prefixdel.h"
#include "findlink.h"
#include <QtWidgets>
#include <QDir>
#include <QFileSystemModel>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <iostream>
#include <QList>
#include <QToolButton>
#include <QFile>
#include <QByteArray>
#include <QApplication>
#include <QThread>
#include <QTabWidget>
#include <QGridLayout>
#include <QPointer>
#include <tuple>
#include <fstream>
#include <filesystem>
#include "filedesktop.h"
#include "sizewin.h"
mainwindow::mainwindow(main_target *t,QWidget *parent) : QWidget(parent)
{
	target = t;
	target->home = QDir::homePath();
	target->CONF = target->home + "/.config/wine-gui.conf";
	target->model_storages.append("");
	target->prefix_path = target->home + "/.wine";
	button = new QPushButton(tr("add"));
	button2 = new QPushButton(tr("del"));
	button3 = new QToolButton;button3->setText("...");
	table = new prefixTable(target,this);
	table->model_update();
	VBOX = new QVBoxLayout;
	HBOX1 = new QHBoxLayout;
	QHBoxLayout *mainBOX = new QHBoxLayout;
	VBOX->addWidget(table->treeView);
	HBOX1->addWidget(button);
	HBOX1->addWidget(button2);
	HBOX1->addWidget(button3);
	VBOX->addLayout(HBOX1);
	VBOX->setContentsMargins(0,0,0,0);
	tab = new prefixTab(target,this);
	table->treeView->setEditTriggers(QTreeView::NoEditTriggers);
	tab->tabWidget->setMinimumWidth(450);
	setLayout(mainBOX);
	QFrame *widgetVBOX = new QFrame();
	widgetVBOX->setLayout(VBOX);
	splitter = new QSplitter(Qt::Horizontal);
	splitter->addWidget(widgetVBOX);
	splitter->addWidget(tab->tabWidget);
	splitter->setChildrenCollapsible(false);
	mainBOX->addWidget(splitter);
	setWindowTitle("Wine GUI");
	connect(button, &QPushButton::clicked , this , &mainwindow::button_slot);
	connect(button2, &QPushButton::clicked , this , &mainwindow::button2_slot);
	connect(button3, &QPushButton::clicked , this , &mainwindow::settings_slot);
	connect(table->treeView, &QTreeView::clicked , this , &mainwindow::storage_slot);
	connect(table->treeView, &QTreeView::doubleClicked , this , &mainwindow::storage_slot2);
	connect(tab->tools->t, &tabTools::WineVer , this , &mainwindow::modelWineVer);
	target->storage = -1;
	target->prefix = "";
	edit_create = false;
	table->treeView->header()->setStretchLastSection(false);
	table->treeView->header()->setSectionResizeMode(0,QHeaderView::Stretch);
	button->setEnabled(false);button2->setEnabled(false);
	target->prefix_wine = table->model->item(0,1)->text();
	setAttribute(Qt::WA_DeleteOnClose);
	sizeWin(this,"main").restore();
	if(QFile::exists(target->CONF)){
		QSettings settings_conf(target->CONF,QSettings::IniFormat);
		#if QT_VERSION < 0x060000
			settings_conf.setIniCodec("UTF-8");
		#endif
		target->DXVK = settings_conf.value("main/dxvk").toString();
		target->WINE_VER = settings_conf.value("main/wine").toString();
		quint32 toolSize = settings_conf.value("win/toolbar").toUInt();
		if(toolSize >= 450){
			splitter->setSizes(QList<int>() << -1 << toolSize);
		}else{
			splitter->setSizes(QList<int>() << -1 << 450);
		}
	}else{
		splitter->setSizes(QList<int>() << -1 << 450);
	}
	splitter->setStretchFactor(0, 1);
}
mainwindow::~mainwindow(){
	sizeWin size(this,"main");
	quint32 widthToolbar = splitter->sizes().at(1);
	if(size.settings_conf->value("win/toolbar").toUInt() != widthToolbar){
		size.settings_conf->setValue("win/toolbar",widthToolbar);
	}
	size.save();
}
void mainwindow::button_slot(){
	if(table->treeView->currentIndex().parent().row() == -1 && table->treeView->currentIndex().row() < 1 ){return;};
	prefixAdd *add = new prefixAdd(	table->model->item(target->storage)->icon(),
									table->model,
									target,
									this);
	add->wine_ver->setModel(tab->tools->t->wine->model());
	VBOX->insertWidget(1,add);
	connect(table->treeView, &QTreeView::clicked , add , &prefixAdd::update_slot);
	connect(table->treeView, &QTreeView::clicked , add , &prefixAdd::create_check);
	connect(add, &prefixAdd::main_tool_widget_hide , this , &mainwindow::main_tool_widget_hide);
	main_tool_widget_hide();
}
void mainwindow::button2_slot(){
	if(table->treeView->currentIndex().parent().row() == -1 && table->treeView->currentIndex().row() == 0 ){return;};
	threadDel *delWork = new threadDel(target->model_storages.at(target->storage) + "/" + target->prefix);
	prefixDel *del = new prefixDel(delWork,table->model->item(target->storage)->text() + " / " + target->prefix,this);
	main_tool_widget_hide();
	VBOX->insertWidget(1,del);
	connect(delWork, &threadDel::model_update , table , &prefixTable::model_update);
	connect(del, &prefixDel::start , delWork , &threadDel::work);
	connect(del, &prefixDel::main_tool_widget_hide , this , &mainwindow::main_tool_widget_hide);
}
void mainwindow::settings_slot(){
	button3->setEnabled(false);
	settings *s = new settings(table->model,target);
	connect(s, &settings::model_update , table , &prefixTable::model_update);
	connect(s, &settings::destroyed , this , &mainwindow::settingsEnabled);
	connect(s, &settings::destroyed , tab->tools->t , &tabTools::wineDir);
	s->show();
}
void mainwindow::settingsEnabled(){button3->setEnabled(true);}
void mainwindow::storage_edit(){
	if(edit_create == true){
		if(edit_target_wineprefix != ""){
			if(edit_target_wineprefix != edit->text()){
				findLink ln(target->model_storages.at(edit_target) + "/" + edit_target_wineprefix + "/shortcuts",
							QDir::homePath() + "/.local/share/applications");
				ln.lnFindRc();
				ln.lnMove(target->model_storages.at(edit_target) + "/" + edit->text() + "/shortcuts");
				ln.dir = QDir::homePath() + "/.local/share/icons";
				ln.lnFindRc();
				ln.lnMove(target->model_storages.at(edit_target) + "/" + edit->text() + "/shortcuts");
				ln.dir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
				ln.lnFind();
				ln.lnMove(target->model_storages.at(edit_target) + "/" + edit->text() + "/shortcuts");
				QFile::rename(QString(target->model_storages.at(edit_target) + "/" + edit_target_wineprefix),
							  QString(target->model_storages.at(edit_target) + "/" + edit->text()));
				int i = target->storage;
				table->model_update();
				target->storage = i;
				target->prefix = edit->text();
				tab->tools->l->shortcuts();
			}
		}else{
			if(table->model->item(edit_target)->text() != edit->text()){
				QSettings conf(target->CONF,QSettings::IniFormat);
				#if QT_VERSION < 0x060000
					conf.setIniCodec("UTF-8");
				#endif
				conf.setValue("storage_" + QString::number(edit_target - 1) + "/name",edit->text());
				QString icon = get<0>(target->storages.at(edit_target - 1));
				QString path = get<2>(target->storages.at(edit_target - 1));
				target->storages.remove(edit_target - 1);
				target->storages.insert(edit_target - 1,tuple(icon,edit->text(),path));
				table->model_update();
			}
		}
		edit->deleteLater();
		edit_create = false;
	}
}
void mainwindow::storage_slot2(QModelIndex storage_index){
	if(edit_create == false){
		edit = new QLineEdit(this);
		connect(edit, &QLineEdit::returnPressed , this , &mainwindow::storage_edit);
		edit_create = true;
		edit_target = target->storage;
		edit_target_wineprefix = target->prefix;
	}
	if(storage_index.parent().row() == -1){
		if(storage_index.row() != 0){
			edit->setText(table->model->item(storage_index.row(),0)->text());
			VBOX->insertWidget(1,edit);
			edit->setFocus();
			target->storage = storage_index.row();
			target->prefix = "";
		}
	}else{
		edit->setText(table->model->item(storage_index.parent().row(),0)->child(storage_index.row(),0)->text());
		VBOX->insertWidget(1,edit);
		edit->setFocus();
		target->storage = storage_index.parent().row();
		target->prefix = table->model->item(storage_index.parent().row(),0)->child(storage_index.row(),0)->text();
	}
}
void mainwindow::storage_slot(QModelIndex storage_index){
	if(storage_index.parent().row() == -1){
		if(storage_index.row() == 0){
			button->setEnabled(false);
			target->prefix_path = target->home + "/.wine";
			target->prefix_wine = table->model->item(0,1)->text();
			cfgUpdate();
		}else{
			button->setEnabled(true);
		}
		button2->setEnabled(false);
		target->storage = storage_index.row();
		target->prefix = "";
	}else{
		button->setEnabled(true);
		button2->setEnabled(true);
		target->storage = storage_index.parent().row();
		target->prefix = table->model->item(storage_index.parent().row(),0)->child(storage_index.row(),0)->text();
		target->prefix_path = get<2>(target->storages.at(target->storage - 1)) + "/" + target->prefix;
		target->prefix_wine = table->model->item(storage_index.parent().row(),0)->child(storage_index.row(),1)->text();
		cfgUpdate();
	}
	tab->tools->l->shortcuts();
	tab->tools->t->updateWine();
	tab->readReg(tab->tabWidget->currentIndex());
}
void mainwindow::cfgUpdate(){
	QSettings conf(target->prefix_path + "/WINE.cfg",QSettings::IniFormat);
	#if QT_VERSION < 0x060000
		conf.setIniCodec("UTF-8");
	#endif
	QString SYNC = conf.value("SYNC").toString();
	if(SYNC == "ESYNC"){
		target->sync = 1;
	}else{
		if(SYNC == "FSYNC"){
			target->sync = 2;
		}else{
			target->sync = 0;
		}
	}
	QString v = conf.value("WINE").toString();
	if(v != "System" && v.size() > 0){
		target->WINE = v;
	}else{
		target->WINE.clear();
	}
}
void mainwindow::main_tool_widget_hide(bool hidden){
	table->model_update();
	table->treeView->expandAll();
	for(int short l = 0;l < HBOX1->count();l++){
		HBOX1->itemAt(l)->widget()->setHidden(hidden);
	}
	button->setEnabled(false);
	button2->setEnabled(false);
}
void mainwindow::model_add_prefix(QStandardItem *branch,QList<QStandardItem*> *item){
	branch->appendRow(*item);
	item->~QList();
}
void mainwindow::modelWineVer(QString t){
	tab->tools->l->shortcuts();
	if(target->storage > 0 && target->prefix != ""){
		for(quint16 i = 0;i < table->model->item(target->storage)->rowCount();i++ ){
			if(table->model->item(target->storage)->child(i)->text() == target->prefix){
				table->model->item(target->storage)->child(i,1)->setText(t);
				return;
			}
		}
	}else{
		table->model->item(0,1)->setText(t);
	}
}
