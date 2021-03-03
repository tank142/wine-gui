#include "prefixadd.h"
#include <QGridLayout>
#include <QRadioButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QIcon>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QPushButton>
#include <QProcess>
#include <QStringList>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <QTimer>
#include <QSettings>
#include "shell.h"
#include "shelloutput.h"
#include "dircopy.h"
#include "dirWidget.h"
#include <elf.h>
#include <iostream>
#if defined(__LP64__)
#define ElfW(type) Elf64_ ## type
#else
#define ElfW(type) Elf32_ ## type
#endif
using namespace std;
bool prefixAdd::check_64bit(const char* elfFile) {
	ElfW(Ehdr) header;
	FILE* file = fopen(elfFile, "rb");
	fread(&header, sizeof(header), 1, file);
	fclose(file);
	if (memcmp(header.e_ident, ELFMAG, SELFMAG) == 0) {
		if(header.e_machine == 62){
			return true;
		}
	}
	return false;
}
void prefixAdd::wine_arch(){
	QString wine;
	if(wine_ver->currentIndex() == 0 ){
		wine = "/usr/bin/wineserver";
	}else{
		wine = target->WINE_VER + "/" + wine_ver->currentText() + "/bin/wineserver";
	}
	if(check_64bit(QString(wine).toStdString().c_str())){
		bit->setHidden(false);
	}else{
		bit->setHidden(true);
	}
}
prefixAdd::prefixAdd(QIcon i,QStandardItemModel *m,main_target *t,QWidget *parent) : QWidget(parent)
{
	model = m;target = t;
	storage = target->storage;
	storage_target = target->storage;
	button = new QPushButton("Создать",this);
	QPushButton *button2 = new QPushButton("Отмена",this);
	main = new QVBoxLayout(this);
	box_storage = new QHBoxLayout(this);box_storage->setAlignment(Qt::AlignLeft);
	edit = new QLineEdit(this);
	text = new QLabel(this);text->setText(model->item(storage_target,0)->text());
	text_target = new QLabel(this);text_target->setText(target->prefix);
	QLabel *icon = new QLabel(this);icon->setPixmap(i.pixmap(16,16));
	QGroupBox *gbox = new QGroupBox(this);
	QGridLayout *gbox_l = new QGridLayout(this);
	bit = new QComboBox(this);
	bit->insertItem(0,"64bit");bit->insertItem(1,"32bit");
	move = new QRadioButton(tr("Переместить"),this);
	copy = new QRadioButton(tr("Скопировать"),this);
	create = new QRadioButton(tr("Создать новый"),this);
	wine_ver = new QComboBox(this);
	box_storage->addWidget(icon);
	box_storage->addWidget(text);
	gbox->setLayout(gbox_l);
	gbox_l->addWidget(copy,1,0);
	gbox_l->addWidget(text_target,1,1);
	gbox_l->addWidget(move,2,0);
	gbox_l->addWidget(wine_ver,2,1);
	gbox_l->addWidget(create,3,0);
	gbox_l->addWidget(bit,3,1);
	gbox_l->addWidget(button,4,0);
	gbox_l->addWidget(button2,4,1);
	main->addWidget(edit);
	main->addLayout(box_storage);
	main->addWidget(gbox);
	setLayout(main);
	copy->toggle();
	connect(edit, &QLineEdit::textEdited , this , &prefixAdd::create_check);
	connect(move, &QRadioButton::toggled , this , &prefixAdd::create_check);
	connect(copy, &QRadioButton::toggled , this , &prefixAdd::create_check);
	connect(create, &QRadioButton::toggled , this , &prefixAdd::create_check);
	connect(button, &QPushButton::clicked , this , &prefixAdd::create_slot);
	connect(button2, &QPushButton::clicked , this , &prefixAdd::cancel_slot);
	connect(wine_ver, &QComboBox::currentIndexChanged , this , &prefixAdd::wine_arch);
	button->setEnabled(false);
	QTimer::singleShot(0, edit, SLOT(setFocus()));
	if(!check_64bit(QString("/usr/bin/wineserver").toStdString().c_str())){
		bit->setHidden(true);
	}
}
void prefixAdd::create_check(){
	if(edit->text() == ""){button->setEnabled(false);return;}
	if(move->isChecked()){
		if(storage != storage_target){
			button->setEnabled(true);
		}else{
			button->setEnabled(false);return;
		}
	}
	if(copy->isChecked()){
		if(storage != storage_target){
			button->setEnabled(true);
		}else{
			button->setEnabled(false);return;
		}
	}
	for(int short unsigned i = 0; model->item(storage_target)->rowCount() > i;i++){
		if(model->item(storage_target)->child(i)->text() == edit->text()){
			button->setEnabled(false);return;
		}
	}
	button->setEnabled(true);
}
void prefixAdd::update_slot(QModelIndex storage_index){
	if(storage_index.parent().row() != -1){
		storage = storage_index.parent().row() -1;
		text_target->setText(model->item(storage_index.parent().row(),0)->child(storage_index.row(),0)->text());
		if(move->isChecked() || copy->isChecked() ){
			edit->setText(target->prefix);
		}

	}
}
void prefixAdd::create_slot(){
	for(int short l = 0;l < main->count();l++){
		main->itemAt(l)->widget()->deleteLater();
	}
	for(int short l = 0;l < box_storage->count();l++){
		box_storage->itemAt(l)->widget()->deleteLater();
	}box_storage->deleteLater();
	QDir().mkpath(target->model_storages.at(storage_target) + "/" + edit->text());
	if(create->isChecked()){
		shell *wine;
		QSettings conf(target->model_storages.at(storage_target) + "/" + edit->text() + "/WINE.cfg",QSettings::IniFormat);conf.setIniCodec( "UTF-8" );
		if(wine_ver->currentIndex() == 0 ){
			wine = new shell("wineboot",QStringList());
		}else{
			wine = new shell(target->WINE_VER + "/" + wine_ver->currentText() + "/bin/wineboot",QStringList());
			conf.setValue("WINE",wine_ver->currentText());
		}
		wine->env->insert("WINEPREFIX",target->model_storages.at(storage_target) + "/" + edit->text());
		wine->env->insert("WINEDEBUG","");
		if(!bit->isHidden()){
			if(bit->currentIndex() == 1){
				wine->env->insert("WINEARCH","win32");
				conf.setValue("WINEARCH","win32");
			}
		}
		conf.sync();conf.deleteLater();
		connect(wine, &shell::exit , this , &prefixAdd::cancel_slot);
		wine->start();
	}
	if(copy->isChecked()){
		dirWidget *cpdir = new dirWidget(new dirCopy(
			get<2>(target->storages.at(storage)) + "/" + target->prefix,
			get<2>(target->storages.at(storage_target - 1)) + "/" + edit->text())
		);
		connect(cpdir, &dirWidget::main_tool_widget_hide , this , &prefixAdd::cancel_slot);
		cpdir->show();
		cpdir->work->start();
		main->insertWidget(1,cpdir);
	}
	if(move->isChecked()){
		dirWidget *cpdir = new dirWidget(new dirMove(
			get<2>(target->storages.at(storage)) + "/" + target->prefix,
			get<2>(target->storages.at(storage_target - 1)) + "/" + edit->text())
		);
		connect(cpdir, &dirWidget::main_tool_widget_hide , this , &prefixAdd::cancel_slot);
		cpdir->show();
		cpdir->work->start();
		main->insertWidget(1,cpdir);
	}
}
void prefixAdd::cp(){
	dirWidget *cpdir = new dirWidget(new dirCopy(
		get<2>(target->storages.at(target->storage)) + "/" + target->prefix,
		get<2>(target->storages.at(storage_target)) + "/" + edit->text())
	);
	connect(cpdir, &dirWidget::main_tool_widget_hide , this , &prefixAdd::cancel_slot);
	cpdir->show();
	cpdir->work->start();
	main->insertWidget(1,cpdir);
}
void prefixAdd::cancel_slot(){
	emit main_tool_widget_hide(false);
	this->deleteLater();
}
