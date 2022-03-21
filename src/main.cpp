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
#include <QRegularExpression>
#include <sys/resource.h>
#include <QStandardPaths>
void checkLimits(main_target *target){
	struct rlimit limits;
	getrlimit(RLIMIT_NOFILE,&limits);
	if (limits.rlim_cur >= 1048576 && limits.rlim_max >= 1048576){
		target->esync = true;
	}else{
		target->esync = false;
	}
}
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	if(qApp->arguments().size() > 1 && qApp->arguments().at(1) == "-h"){
		cout << "Usage: wine-gui [option] [exe]" << endl
			 << "This information : -h" << endl
			 << "Debug window     : --wine-debug" << endl;
		return 0;
	}
	QTranslator translator;
	const QStringList uiLanguages = QLocale::system().uiLanguages();
	for (const QString &locale : uiLanguages) {
		const QString baseName = "wine-gui_" + QLocale(locale).name();
		#if BUILD_FLAG != debug
		if (translator.load(baseName,QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).at(2) + "/lang")) {
		#else
		if (translator.load(baseName)){
		#endif
			a.installTranslator(&translator);
			break;
		}
	}
	a.installTranslator(&translator);
	qApp->setWindowIcon(QIcon::fromTheme("wine"));
	main_target *target = new main_target;
	checkLimits(target);
	target->home = QDir::homePath();
	target->CONF = target->home + "/.config/wine-gui.conf";
	target->model_storages.append("");
	QApplication::setStyle(QStyleFactory::create("Fusion"));
	if(QFile::exists(target->CONF)){
		QSettings settings_conf(target->CONF,QSettings::IniFormat);
		#if QT_VERSION < 0x060000
			settings_conf.setIniCodec("UTF-8");
		#endif
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
			QRegularExpression rx("^storage_[0-9]*$");
			if (rx.match(wine_storages.at(i)).hasMatch()){
				if(settings_conf.contains(wine_storages.at(i) + "/path")){
					target->storages.append(tuple(settings_conf.value(wine_storages.at(i) + "/icon").toString(),
												  settings_conf.value(wine_storages.at(i) + "/name").toString(),
												  settings_conf.value(wine_storages.at(i) + "/path").toString()));
				}
			}
		}
	}
	if(argc > 1){
		if(!EXEinStorages(target,qApp->arguments()).exec()){
			openEXE *open = new openEXE(target,qApp->arguments());
			open->show();
		}
	}else{
		mainwindow *main = new mainwindow(target);
		main->show();
		if(target->storages.size() == 0){main->settings_slot();}
	}
	return a.exec();
}
