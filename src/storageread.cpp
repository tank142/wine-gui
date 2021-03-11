#include "storageread.h"
#include <QDir>
#include <QSettings>
storageRead::storageRead(QStandardItem *b,QString d)
{branch = b; directory = d;}
void storageRead::run(){
	QString text = branch->text();branch->setText(text + " Загрузка...");
	QDir dir;dir.setPath(directory);dir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Hidden);dir.setSorting(QDir::Name);
	foreach (QFileInfo fileInfo,dir.entryInfoList()) {
		QList<QStandardItem *> *item = new QList<QStandardItem *>;
		item->append(new QStandardItem(fileInfo.fileName()));
		if(QFile::exists(directory + "/" + fileInfo.fileName() + "/WINE.cfg")){
			QSettings settings(directory + "/" + fileInfo.fileName() + "/WINE.cfg", QSettings::IniFormat);
			item->append(new QStandardItem(settings.value("WINE").toString()));
		}else{
			item->append(new QStandardItem("System"));
		}
		emit model_add_prefix(branch,item);
	}
	branch->setText(text);
	this->deleteLater();
}
