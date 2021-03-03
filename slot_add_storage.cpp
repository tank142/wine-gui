#include "slot_add_storage.h"
#include "slot_icon.h"
#include "slot_set_dir.h"
#include "slot_del_storage.h"
#include <QDir>
#include <QObject>
#include <QStyle>
#include <QStringLiteral>
#include <QDebug>
QComboBox* add_storage::icons(QString directory){
	QDir dir;dir.setPath(directory);
	QComboBox* icons = new QComboBox();
	dir.setFilter(QDir::Files);dir.setSorting(QDir::Name);
	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		if (fileInfo.fileName() == "." || fileInfo.fileName() == ".." ) {
			continue;
		}
		icons->addItem(QIcon(directory + "/" + fileInfo.fileName()),fileInfo.fileName());
	}
	return icons;
}
void add_storage::add(){
		QHBoxLayout *HBOX = new QHBoxLayout;
		QComboBox *icon = icons("/home/tank/icons");
		QToolButton *storage_button_del = new QToolButton();
		QPushButton *dir = new QPushButton(directory);
		QToolButton *up = new QToolButton;
		QToolButton *down = new QToolButton;
		up->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
		down->setIcon(style()->standardIcon(QStyle::SP_ArrowDown));
		//up->setMaximumHeight(23);;down->setMaximumHeight(23);
		//up->setMaximumWidth(23);down->setMaximumWidth(23);
		storage_button_del->setText("Удалить");
		icon->setMaximumWidth(43);
		HBOX->addWidget(icon);
		HBOX->addWidget(dir);
		if(directory != ""){
			if(QFile::exists(directory)){
				dir->setText(directory);
			}else{
				dir->setText(directory + " (Не найдено)");
			}
		}
		HBOX->addWidget(up);
		HBOX->addWidget(down);
		HBOX->addWidget(storage_button_del);
		slot_icon *icon_slot = new slot_icon(icon);
		connect(icon, SIGNAL(currentTextChanged(QString)),icon_slot,SLOT(icon()));
		del_storage *storage_button_del_slot = new del_storage(HBOX,icon,dir,storage_button_del,up,down);
		connect(storage_button_del, SIGNAL(clicked()),storage_button_del_slot,SLOT(del()));
		slot_set_dir *dir_slot = new slot_set_dir(dir);
		connect(dir, SIGNAL(clicked()),dir_slot,SLOT(set_dir()));
		vbox->addLayout(HBOX);
		if( vbox->indexOf(HBOX) == 0 ) {
			up->setHidden(true);
			down->setHidden(true);
		}
		if( vbox->indexOf(HBOX) > 0 ) {
			//QToolButton * button = qobject_cast<QPushButton*>(layout->itemAtPosition(i,j)->widget());
			//qDebug() << "ЫЫ:" << vbox->itemAt(0);
			//QLayoutItem *T = vbox->itemAt(0);
			//vbox->itemAt(0)->down->setHidden(false);
		}
		qDebug() << vbox->indexOf(HBOX);
};
