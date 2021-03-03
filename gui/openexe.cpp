#include "openexe.h"
#include "main_target.h"
#include "shelloutput.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSizePolicy>
#include <QFileDialog>
#include <QSettings>
#include <QRegExp>
#include <QModelIndex>
#include <QHeaderView>
#include <iostream>
using namespace std;
openEXE::openEXE(main_target *t,QStringList arg,QWidget *parent) : QWidget(parent)
{
	target = t;
	Key_Return = new QShortcut(this);
	Key_Return->setKey(Qt::Key_Return);
	open = new QPushButton(this);
	if(arg.size() > 0){
		arg.removeAt(0);
		QString argf;
		foreach(QString a,arg){
			argf = argf + a;
		}
		argf.replace(QRegExp("^~/"),QDir::homePath() + "/");
		argf.replace(QRegExp("^./"),QDir::currentPath() + "/");
		if(QFile(argf).exists()){open->setText(argf);}
	}
	setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding );
	QVBoxLayout *VBOX = new QVBoxLayout(this);
	table = new prefixTable(target,this);
	QHBoxLayout *hbox = new QHBoxLayout();
	debug = new QCheckBox("Отладка",this);
	debug->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
	hbox->addWidget(open);
	hbox->addWidget(debug);
	VBOX->addLayout(hbox);
	VBOX->addWidget(table->treeView);
	table->model->setHorizontalHeaderLabels(QStringList() << "Префиксы" << "Wine" );
	table->treeView->header()->setStretchLastSection(false);
	table->treeView->header()->setSectionResizeMode(0,QHeaderView::Stretch);
	table->model_update();
	setLayout(VBOX);
	connect(open, &QPushButton::clicked , this , &openEXE::exe);
	connect(table, &prefixTable::readFinished , this , &openEXE::tableRestore);
	connect(table->treeView, &QTreeView::clicked , this , &openEXE::run);
	connect(Key_Return, &QShortcut::activated , this , &openEXE::key_return);
}
void openEXE::key_return(){
	run(table->treeView->currentIndex());
}
void openEXE::tableRestore(){
	QSettings settings_conf(target->CONF,QSettings::IniFormat);settings_conf.setIniCodec( "UTF-8" );
	debug->setChecked(settings_conf.value("main/saveDebug").toBool());
	QStringList wine_storages = settings_conf.childGroups();
	saveStorage = settings_conf.value("main/saveStorage").toString();
	if(saveStorage == "0"){saveStorageIndex = 0;
		QModelIndex indexFromModel = table->model->item(0)->index();
		table->treeView->setCurrentIndex(indexFromModel);return;}
	savePrefix = settings_conf.value("main/savePrefix").toString();
	saveStorageIndex = -1;
	if(saveStorage != ""){
		for (int i = 0; i < wine_storages.size(); i++){
			if (wine_storages.at(i) != "main"){
				if(saveStorage == settings_conf.value(wine_storages.at(i) + "/path").toString()){
					saveStorageIndex = i; break;
				}
			}
		}
	}
	if(saveStorageIndex > 0){
		for(int i = 0;i < table->model->item(saveStorageIndex)->rowCount();i++){
			if(savePrefix == table->model->item(saveStorageIndex)->child(i)->text()){
				QModelIndex indexFromModel = table->model->item(saveStorageIndex)->child(i)->index();
				table->treeView->setCurrentIndex(indexFromModel);
			}
		}
	}
}
void openEXE::exe(){
	QString d=(QFileDialog::getOpenFileName(this,
				QString::fromUtf8("Выбор файла"),
				open->text()));
	if(d!=""){open->setText(d);};
}
QString openEXE::findWineBin(QModelIndex storage_index){
	if(storage_index.parent().row() == -1){
		if(table->model->item(0,1)->text() == "System"){
			return "wine";
		}else{
			return target->WINE_VER + "/" + table->model->item(0,1)->text() + "/bin/wine";
		}
	}
	if(table->model->item(storage_index.parent().row())->child(storage_index.row(),1)->text() == "System"){
		return "wine";
	}else{
		if(storage_index.parent().row() == -1){
			return target->WINE_VER + "/" + table->model->item(storage_index.row(),1)->text() + "/bin/wine";
		}else{
			return target->WINE_VER + "/" + table->model->item(storage_index.parent().row())->child(storage_index.row(),1)->text() + "/bin/wine";
		}
	}
}
QString openEXE::findWinePrefix(QModelIndex storage_index){
	if(storage_index.parent().row() == -1){
		if(storage_index.row() == 0){
			return QDir::homePath() + "/.wine";
		}
	}else{
		return get<2>(target->storages.at(storage_index.parent().row() - 1 )) + "/"
		+ table->model->item(storage_index.parent().row())->child(storage_index.row())->text();
	}
	return "";
}
void openEXE::run(QModelIndex storage_index){
	QSettings settings_conf(target->CONF,QSettings::IniFormat);settings_conf.setIniCodec( "UTF-8" );
	if(storage_index.parent().row() > -1){
		settings_conf.setValue("main/saveStorage",get<2>(target->storages.at(storage_index.parent().row() - 1)));
		settings_conf.setValue("main/savePrefix",table->model->item(storage_index.parent().row())->child(storage_index.row())->text());
	}else{
		if(storage_index.row() > 0){return;}
		settings_conf.setValue("main/saveStorage",0);
	}
	settings_conf.setValue("main/saveDebug",debug->isChecked());
	settings_conf.sync();settings_conf.deleteLater();
	if(!QFile(open->text()).exists()){return;}
	QString wineBin = findWineBin(storage_index);
	if(debug->isChecked()){
		shellOutputDebugging *wine = new shellOutputDebugging(wineBin,QStringList() << open->text());
		wine->exec->env->insert("WINEPREFIX",findWinePrefix(storage_index));
		wine->exec->env->insert("WINEDEBUG","");
		wine->start();
	}else{
		setAttribute(Qt::WA_QuitOnClose,false);
		shell *wine = new shell(wineBin,QStringList() << open->text());
		wine->env->insert("WINEPREFIX",findWinePrefix(storage_index));
		wine->env->insert("WINEDEBUG","-all");
		connect(wine, &shell::destroyed , qApp , &QApplication::quit);
		wine->start();
	}
	this->deleteLater();
}
