#ifndef WINETRICKS_H
#define WINETRICKS_H
#include "main_target.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTreeView>
#include <QVector>
#include <QSettings>
#include <QProcess>
#include <QTextStream>
#include <QDir>
#include <QFile>
class winetricksWidget : public QWidget{
	Q_OBJECT
public:
	explicit winetricksWidget(main_target*,QWidget *parent = nullptr);
private:
	main_target *target;
	QVBoxLayout *vbox;
	QHBoxLayout *hbox;
	QPushButton *dlls;
	QPushButton *fonts;
	QPushButton *apps;
	QTreeView *menu;
};
class winetricks : public QObject{
	Q_OBJECT
public:
	explicit winetricks(main_target*,QObject *parent = nullptr);
	main_target *target;
	QString ver;
private:
	bool updateMenu();
	bool runWinetricks(QVector<QStringList>*,QString);
	void saveMenu();
	QString winetricksVer();
	bool restoreMenu(QString verNew);
	QVector<QStringList> dlls;
	QVector<QStringList> fonts;
	QVector<QStringList> apps;
signals:
	void err(QString);
};
#endif // WINETRICKS_H
