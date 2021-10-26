#include "settings_storage_slots.h"
#include "settings.h"
#include <QDir>
#include <QObject>
#include <QStyle>
#include <QStringLiteral>
#include <QToolButton>
#include <QDebug>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVector>
#include <tuple>
void settings_storage_slots::set_dir(){
	QString d=(QFileDialog::getExistingDirectory(this,
				QString::fromUtf8("Выбор папки"),
				QDir::homePath() + "/.local",
				QFileDialog::ShowDirsOnly));
	if(d != "" ){
		get<2>(s->s_ui->at(s->storage_vbox2->indexOf(hbox)))->setText(d);
	}
	s->check();
}
void settings_storage_slots::move_up(){
	quint16 index = (s->storage_vbox2->indexOf(hbox));
	quint16 index_up = index - 1;
	quint16 size = s->s_ui->size() - 1;
	s->s_ui->move(index,index_up);
	s->storage_vbox2->removeItem(hbox);
	s->storage_vbox2->insertLayout(index_up,hbox);
	if (index_up == 0){
		get<3>(s->s_ui->at(0))->setHidden(true);
		get<4>(s->s_ui->at(0))->setHidden(false);
		get<3>(s->s_ui->at(1))->setHidden(false);
	}
	if (index == size){
		get<4>(s->s_ui->at(index_up))->setHidden(false);
		get<4>(s->s_ui->at(index))->setHidden(true);
	}
}
void settings_storage_slots::move_down(){
	quint16 index = (s->storage_vbox2->indexOf(hbox));
	quint16 index_d = index + 1;
	quint16 size = s->s_ui->size() - 1;
	s->s_ui->move(index,index_d);
	s->storage_vbox2->removeItem(hbox);
	s->storage_vbox2->insertLayout(index_d,hbox);
	if (index_d == size){
		get<4>(s->s_ui->at(size))->setHidden(true);
		get<4>(s->s_ui->at(size - 1))->setHidden(false);
	}
	if (index == 0){
		get<3>(s->s_ui->at(index_d))->setHidden(false);
		get<3>(s->s_ui->at(index))->setHidden(true);
	}
}
void settings_storage_slots::del(){
	quint16 index = s->storage_vbox2->indexOf(hbox);
	if (s->s_ui->size() == 1){
		get<3>(s->s_ui->at(0))->setHidden(true);
		get<4>(s->s_ui->at(0))->setHidden(true);
	}
	if(index == 0 and s->s_ui->size() > 1){
		get<3>(s->s_ui->at(1))->setHidden(true);
	}
	if(index != 0 and index + 1 == s->s_ui->size()){
		get<4>(s->s_ui->at(index - 1))->setHidden(true);
	}
	get<0>(s->s_ui->at(index))->deleteLater();
	get<1>(s->s_ui->at(index))->deleteLater();
	get<2>(s->s_ui->at(index))->deleteLater();
	get<3>(s->s_ui->at(index))->deleteLater();
	get<4>(s->s_ui->at(index))->deleteLater();
	get<5>(s->s_ui->at(index))->deleteLater();
	s->s_ui->at(index).~tuple();
	s->s_ui->remove(index);
	if(s->s_ui->size() == 1){
		get<4>(s->s_ui->at(0))->setHidden(true);
	}
	s->check();
	this->deleteLater();
}
