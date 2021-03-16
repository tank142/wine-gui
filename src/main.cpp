#include "settings.h"
#include "mainwindow.h"
#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QString>
#include <QTimer>
#include <QTreeView>
#include <QHBoxLayout>
#include <iostream>
#include <QDebug>
#include <QIcon>
#include "settings.h"
#include <QVector>
#include <tuple>
#include "openexe.h"
#include "main_target.h"
#include "storageread.h"
#include <QStyleFactory>
#include <QTranslator>
#include <QStandardPaths>
#include <QMessageBox>
using namespace std;
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTranslator translator;
	//QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation).at(2) + "/lang")
	if(!translator.load("wine-gui_" + QLocale::system().name(),"/usr/share/wine-gui/lang")){
		if(!translator.load("wine-gui_en_US","/usr/share/wine-gui/lang")){
			QMessageBox *err = new QMessageBox;
			err->setWindowTitle("Attention");
			err->setText("Localization loading error!");
			err->show();
		}
	}
	a.installTranslator(&translator);
	qApp->setWindowIcon(QIcon::fromTheme("wine"));
	main_target *target = new main_target;
	target->home = QDir::homePath();
	target->CONF = target->home + "/.config/wine-gui.conf";
	target->model_storages.append("");
	QApplication::setStyle(QStyleFactory::create("Fusion"));
	if(QFile::exists(target->CONF)){
		QSettings settings_conf(target->CONF,QSettings::IniFormat);
		target->DXVK = settings_conf.value("main/dxvk").toString();
		target->NINE = settings_conf.value("main/nine").toString();
		target->WINE_VER = settings_conf.value("main/wine").toString();
		if(settings_conf.value("main/dark_theme").toBool()){
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
		}
		QStringList wine_storages = settings_conf.childGroups();
		for (int i = 0; i < wine_storages.size(); i++){
			if (wine_storages.at(i) != "main"){
				if(settings_conf.contains(wine_storages.at(i) + "/path")){
					target->storages.append(tuple(settings_conf.value(wine_storages.at(i) + "/icon").toString(),
												  settings_conf.value(wine_storages.at(i) + "/name").toString(),
												  settings_conf.value(wine_storages.at(i) + "/path").toString()));
				}
			}
		}
	}
	if(argc > 1){
		openEXE *open = new openEXE(target,qApp->arguments());
		open->show();

	}else{
		mainwindow *main = new mainwindow(target);
		main->show();
		if(target->storages.size() == 0){main->settings_slot();}
	}
	return a.exec();
}
