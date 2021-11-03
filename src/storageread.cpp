#include "storageread.h"
#include <QDir>
#include <QSettings>
storageRead::storageRead(QStandardItem *b,QString d)
{branch = b; directory = d;}
void storageRead::run(){
	QString text = branch->text();branch->setText(text + tr("loading"));
	QDir dir;dir.setPath(directory);dir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Hidden);dir.setSorting(QDir::Name);
	foreach (QFileInfo fileInfo,dir.entryInfoList()) {
		QList<QStandardItem *> *item = new QList<QStandardItem *>;
		item->append(new QStandardItem(fileInfo.fileName()));
		if(QFile::exists(directory + "/" + fileInfo.fileName() + "/WINE.cfg")){
			QSettings settings(directory + "/" + fileInfo.fileName() + "/WINE.cfg", QSettings::IniFormat);
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
		emit model_add_prefix(branch,item);
	}
	branch->setText(text);
	this->deleteLater();
}
