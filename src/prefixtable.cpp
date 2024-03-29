#include "prefixtable.h"
#include "storageread.h"
#include <QStandardItem>
#include <QEventLoop>
#include <QSettings>
#include <QFile>
prefixTable::prefixTable(main_target *t,QObject *parent) : QObject(parent)
{
	target = t;
	model = new QStandardItemModel;
	model->appendRow(new QStandardItem("System"));
	treeView = new QTreeView();
	treeView->setModel(model);
	model->setHorizontalHeaderLabels(QStringList() << tr("prefix") << "Wine" );
}
prefixTable::~prefixTable(){
	treeView->deleteLater();
	model->deleteLater();
}
void prefixTable::model_clear(){
	while (target->model_storages.size() > 1){
		target->model_storages.removeLast();
	}
	while (model->rowCount() > 0){
		model->item(0)->~QStandardItem();
		model->removeRow(0);
	}
}
void prefixTable::threadEnd(){
	threadsRun--;
	if(threadsRun == 0){
		emit readFinished();
	}
}
void prefixTable::model_update(){
	model_clear();
	threadsRun = 0;
	QList<QStandardItem *> *item = new QList<QStandardItem *>;
	item->append(new QStandardItem("System"));
	if(QFile::exists(target->home + "/.wine/WINE.cfg")){
		QSettings settings(target->home + "/.wine/WINE.cfg", QSettings::IniFormat);
		#if QT_VERSION < 0x060000
			settings.setIniCodec("UTF-8");
		#endif
		QString wine = settings.value("WINE").toString();
		if(wine.size() > 0 && wine != "System"){
			item->append(new QStandardItem(wine));
		}else{
			item->append(new QStandardItem("System"));
		}
	}else{
		item->append(new QStandardItem("System"));
	}
	model->appendRow(*item);
	item->~QList();
	for (int i = 0; i < target->storages.size(); i++){
			QStandardItem *item = new QStandardItem(QIcon(target->ICON + get<0>(target->storages.at(i))),get<1>(target->storages.at(i)));
			storageRead *branch = new storageRead(item,get<2>(target->storages.at(i)));
			target->model_storages.append(get<2>(target->storages.at(i)));
			model->appendRow(item);
			connect(branch, &storageRead::model_add_prefix , this , &prefixTable::model_add_prefix);
			threadsRun++;
			connect(branch, &storageRead::destroyed, this , &prefixTable::threadEnd);
			branch->start();
	}
	target->storage = -1;
	target->prefix = "";
	treeView->expandAll();
}
void prefixTable::model_add_prefix(QStandardItem *branch,QList<QStandardItem*> *item){
	branch->appendRow(*item);
	item->~QList();
}
