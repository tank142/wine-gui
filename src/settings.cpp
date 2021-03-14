#include "settings.h"
#include "settings_storage_slots.h"
#include <QtWidgets>
#include <QDir>
#include <QFileSystemModel>
#include <QTreeView>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <iostream>
#include <QList>
#include <QToolButton>
#include <fstream>
#include <filesystem>
#include <main_target.h>
#include <QFile>
#include <QByteArray>
#include <QTableWidget>
#include <QStandardItem>
#include <QGroupBox>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <QComboBox>
#include <QVector>
#include <tuple>
settings::settings(QStandardItemModel *m, main_target *t, QWidget *parent) : QWidget(parent)
{
	target = t;
	model = m;
	s_ui = new QVector<tuple<QHBoxLayout*,QComboBox *,QPushButton *,QToolButton *,QToolButton *,QToolButton *>>;
	QGroupBox *storage = new QGroupBox(this);
	QVBoxLayout *vbox = new QVBoxLayout(this);
	QGroupBox *wine_vers = new QGroupBox(this);
	QGroupBox *nine = new QGroupBox(this);
	QHBoxLayout *wine_vers_hbox = new QHBoxLayout(this);
	QGroupBox *dxvk = new QGroupBox(this);
	QVBoxLayout *dxvk_vbox = new QVBoxLayout(this);
	winever = new QPushButton(this);
	ok = new QToolButton(this);
	dxvk_b = new QPushButton(this);
	nine_b = new QPushButton(this);
	dark_theme = new QCheckBox("Тёмное оформление",this);
	QVBoxLayout *nine_l = new QVBoxLayout(this);
	storage_vbox2 = new QVBoxLayout(this);
	QHBoxLayout *hbox = new QHBoxLayout(this);
	QHBoxLayout *hbox2 = new QHBoxLayout(this);
	QVBoxLayout *storage_vbox = new QVBoxLayout(this);
	QPushButton *storage_button_add = new QPushButton("Добавить",this);
	dxvk_b->setText(target->DXVK);
	nine_b->setText(target->NINE);
	winever->setText(target->WINE_VER);
	ok->setText("ОК");
	storage->setTitle("Расположения префиксов");
	wine_vers->setTitle("Расположение верcий Wine");
	dxvk->setTitle("Расположение dxvk");
	nine->setTitle("Расположение Gallium Nine");
	vbox->addWidget(storage);
	vbox->addWidget(dark_theme);
	vbox->addWidget(wine_vers);
	vbox->addWidget(nine);
	vbox->addLayout(hbox);
	vbox->addLayout(hbox2);
	storage_vbox2->setAlignment(Qt::AlignTop); //Qt::AlignHCenter
	vbox->setAlignment(Qt::AlignTop);
	hbox->setAlignment(Qt::AlignTop);
	dxvk->setAlignment(Qt::AlignTop);
	connect(storage_button_add, &QPushButton::clicked , this , &settings::storage_add);
	connect(winever, &QPushButton::clicked , this , &settings::winever_s);
	connect(dxvk_b, &QPushButton::clicked , this , &settings::dxvk_b_s);
	connect(nine_b, &QPushButton::clicked , this , &settings::nine_b_s);
	connect(ok, &QPushButton::clicked , this , &settings::ok_s);
	storage->setLayout(storage_vbox);
	storage_vbox->addLayout(storage_vbox2);
	storage_vbox->addWidget(storage_button_add);
	wine_vers->setLayout(wine_vers_hbox);
	wine_vers_hbox->addWidget(winever);
	dxvk_vbox->addWidget(dxvk_b);
	nine_l->addWidget(nine_b);
	dxvk->setLayout(dxvk_vbox);
	nine->setLayout(nine_l);
	hbox2->addWidget(dxvk);
	setLayout(vbox);
	setWindowTitle(tr("Wine"));
	hbox2->addWidget(ok);
	ok->setFixedSize(70,70);
	if(QFile::exists(target->CONF)){
		QSettings settings_conf(target->CONF,QSettings::IniFormat);
		QStringList wine_storages = settings_conf.childGroups();
		for (int i = 0; i < wine_storages.size(); i++){
			if (wine_storages.at(i) != "main"){
				if(settings_conf.contains(wine_storages.at(i) + "/path")){
					storage_add();
					int s = s_ui->size() - 1;
					int x = get<1>(s_ui->at(s))->findText(settings_conf.value(wine_storages.at(i) + "/icon").toString());
					if(i != -1){
						get<1>(s_ui->at(s))->setCurrentIndex(x);
					}
					get<2>(s_ui->at(s))->setText(settings_conf.value(wine_storages.at(i) + "/path").toString());
				}
			}
		}
	}
	if(s_ui->size() == 0){
		ok->setDisabled(true);
	}
	setAttribute(Qt::WA_DeleteOnClose);
	if(target->NINE == ""){
		if(QFile("/usr/lib/wine/fakedlls/d3d9-nine.dll").exists()){
			nine_b->setText("/usr");
		}
	}
	if(target->DXVK == ""){
		if(QDir("/usr/share/dxvk").exists()){
			dxvk_b->setText("/usr/share/dxvk");
		}
	}
	QSettings conf(target->CONF,QSettings::IniFormat);
	if(conf.value("main/dark_theme").toBool()){
		QPalette darkPalette;
		darkPalette.setColor(QPalette::Window, QColor(53,53,53));
		darkPalette.setColor(QPalette::WindowText, Qt::white);
		darkPalette.setColor(QPalette::Base, QColor(58,58,58));
		darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
		darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
		darkPalette.setColor(QPalette::ToolTipText, Qt::white);
		darkPalette.setColor(QPalette::Text, Qt::white);
		darkPalette.setColor(QPalette::Button, QColor(53,53,53));
		darkPalette.setColor(QPalette::ButtonText, Qt::white);
		darkPalette.setColor(QPalette::BrightText, Qt::red);
		darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
		darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
		darkPalette.setColor(QPalette::HighlightedText, Qt::black);
		qApp->setPalette(darkPalette);
		qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
		dark_theme->setChecked(true);
	}
}
void settings::del(){
	short int size = s_ui->size();
	if(size > 0){
		for(short int index = 0;index != size;index++){
			get<5>(s_ui->at(index))->clicked();
		}
	}
}
void settings::storage_add(){
	QHBoxLayout *hbox = new QHBoxLayout(this);
	QComboBox *icon = icons(target->ICON);
	QToolButton *storage_button_del = new QToolButton(this);
	QPushButton *dir = new QPushButton(directory,this);
	QToolButton *up = new QToolButton(this);
	QToolButton *down = new QToolButton(this);
	up->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
	down->setIcon(style()->standardIcon(QStyle::SP_ArrowDown));
	storage_button_del->setText("Удалить");
	icon->setMaximumWidth(43);
	hbox->addWidget(icon);
	hbox->addWidget(dir);
	if(directory != ""){
		if(QFile::exists(directory)){
			dir->setText(directory);
		}else{
			dir->setText(directory + " (Не найдено)");
		}
	}
	hbox->addWidget(up);
	hbox->addWidget(down);
	hbox->addWidget(storage_button_del);
	s_ui->append(tuple(hbox,icon,dir,up,down,storage_button_del));
	settings_storage_slots *slot = new settings_storage_slots(this, hbox);
	connect(dir, &QPushButton::clicked , slot , &settings_storage_slots::set_dir);
	connect(up, &QPushButton::clicked , slot , &settings_storage_slots::move_up);
	connect(down, &QPushButton::clicked , slot , &settings_storage_slots::move_down);
	connect(storage_button_del, &QPushButton::clicked , slot , &settings_storage_slots::del);
	storage_vbox2->addLayout(hbox);
	down->setHidden(true);
	if( storage_vbox2->indexOf(hbox) == 0 ) {
		up->setHidden(true);
	}
	if( storage_vbox2->indexOf(hbox) == 1 ) {
		get<4>(s_ui->at(0))->setHidden(false);
	}
	if(storage_vbox2->indexOf(hbox) > 1 ){
		get<4>(s_ui->at(storage_vbox2->indexOf(hbox) - 1))->setHidden(false);
	}
}
QComboBox* settings::icons(QString directory){
	QDir dir;dir.setPath(directory);
	QComboBox* icons = new QComboBox(this);
	dir.setFilter(QDir::NoDotAndDotDot | QDir::Files);dir.setSorting(QDir::Name);
	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		icons->addItem(QIcon(directory + "/" + fileInfo.fileName()),fileInfo.fileName());
	}
	return icons;
}
void settings::check(){
	short int size = s_ui->size();
	for(short int index = 0;index != size;index++){
		if("" != get<2>(s_ui->at(index))->text()){
			ok->setDisabled(false);
			return;
		}
	}
	ok->setDisabled(true);
}
void settings::setdir(QPushButton *b){
	QString d=(QFileDialog::getExistingDirectory(this,
				QString::fromUtf8("Выбор папки"),
				QDir::homePath() + "/.local",
				QFileDialog::ShowDirsOnly));
	if(d != "" ){
		b->setText(d);
	}
}
void settings::winever_s(){
	setdir(winever);
}
void settings::dxvk_b_s(){
	setdir(dxvk_b);
}
void settings::nine_b_s(){
	setdir(nine_b);
}
void settings::ok_s(){
	target->WINE_VER = winever->text();
	target->DXVK = dxvk_b->text();
	target->NINE = nine_b->text();
	QVector<QString> *icons = new QVector<QString>;
	for (int i = 0; i < s_ui->size(); i++){
		icons->append(get<1>(s_ui->at(i))->currentText());
	}
	save_conf(icons);
	storages_update();
	emit model_update();
	this->deleteLater();
}
QString settings::find_name(QString path){
	for(quint16 I = 0; target->model_storages.count() > I ;I++){
		if(target->model_storages.at(I) == path ){
			if(	model->rowCount() > I && model->item( I )->text() != ""){
				return model->item( I )->text();
			}
		}
	}
	return QFileInfo(path).baseName();
}
void settings::save_conf(QVector<QString> *icons){
	QSettings conf(target->CONF,QSettings::IniFormat);
	conf.clear();
	conf.setValue("main/dxvk",target->DXVK);
	conf.setValue("main/nine",target->NINE);
	conf.setValue("main/wine",target->WINE_VER);
	conf.setValue("main/dark_theme",dark_theme->isChecked());
	for (int i = 0; i < s_ui->count(); i++){
		conf.setValue("storage_" + QString::number(i) + "/icon",get<1>(s_ui->at(i))->currentText());
		conf.setValue("storage_" + QString::number(i) + "/name",find_name(get<2>(s_ui->at(i))->text()));
		conf.setValue("storage_" + QString::number(i) + "/path",get<2>(s_ui->at(i))->text());
	}
	conf.sync();
	icons->~QVector<QString>();
	if(dark_theme->isChecked()){
		QPalette darkPalette;
		darkPalette.setColor(QPalette::Window, QColor(53,53,53));
		darkPalette.setColor(QPalette::WindowText, Qt::white);
		darkPalette.setColor(QPalette::Base, QColor(58,58,58));
		darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
		darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
		darkPalette.setColor(QPalette::ToolTipText, Qt::white);
		darkPalette.setColor(QPalette::Text, Qt::white);
		darkPalette.setColor(QPalette::Button, QColor(53,53,53));
		darkPalette.setColor(QPalette::ButtonText, Qt::white);
		darkPalette.setColor(QPalette::BrightText, Qt::red);
		darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
		darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
		darkPalette.setColor(QPalette::HighlightedText, Qt::black);
		qApp->setPalette(darkPalette);
		//qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
	}else{
		qApp->setPalette(style()->standardPalette());
		qApp->setStyleSheet("");
	}
}
QString settings::find_storage_name(QStringList *paths,QStringList *names,QString path){
	for (int i = 0; i < paths->size(); i++){
		if(paths->at(i) == path){
			return names->at(i);
		}
	}return "";
}
void settings::storages_update(){
	QStringList names;
	QStringList paths;
	while(target->storages.size()){
		names.append(get<1>(target->storages.last()));
		paths.append(get<2>(target->storages.last()));
		target->storages.removeLast();
	}
	for (int i = 0; i < s_ui->size(); i++){
		target->storages.append(tuple(get<1>(s_ui->at(i))->currentText(),
									  find_storage_name(&paths,&names,get<2>(s_ui->at(i))->text()),
									  get<2>(s_ui->at(i))->text()));
	}
}
