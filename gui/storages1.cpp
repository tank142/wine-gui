#include "settings.h"
#include <QDir>
#include <QObject>
#include <QStyle>
#include <QStringLiteral>
#include <QVector>
#include <tuple>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QToolButton>
using namespace std;
QComboBox* storages::icons(QString directory){
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
void storages::add(){
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
		s_ui.append(tuple<QHBoxLayout *,QComboBox *,QPushButton *,QToolButton *,QToolButton *,QToolButton *>
		(HBOX,icon,dir,up,down,storage_button_del));
		//get<5>(s_ui[0])->setHidden(false);
		if( vbox->indexOf(HBOX) == 1 ) {
			get<5>(s_ui[0])->setHidden(false);
		}
		down->setHidden(true);
		qDebug() << vbox->indexOf(HBOX);
};
