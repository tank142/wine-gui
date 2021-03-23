#ifndef OPENEXE_H
#define OPENEXE_H
#include "mainwindow.h"
#include "main_target.h"
#include "prefixtable.h"
#include <QPushButton>
#include <QGroupBox>
#include <QCheckBox>
#include <QShortcut>
#include <QApplication>
#include <QWidget>
#include <QRegularExpression>

class openEXE : public QWidget
{
	Q_OBJECT
public:
	explicit openEXE(main_target*,QStringList,QWidget *parent = nullptr);
	~openEXE();
	QTreeView *treeView;
	static QString arg(QStringList);
private:
	QPushButton *open;
	QCheckBox *debug;
	prefixTable *table;
	main_target *target;
	QString saveStorage;
	QString savePrefix;
	qint16 saveStorageIndex;
	QShortcut *Key_Return;
	void exe();
	void run(QModelIndex);
	QString findWineBin(QModelIndex);
	QString findWinePrefix(QModelIndex);
private slots:
	void key_return();
	void tableRestore();
signals:

};
class EXEinStorages  : public QObject
{
public:
	explicit EXEinStorages(main_target*,QStringList,QObject *parent = nullptr);
	bool exec();
private:
	main_target *target;
	QString exe;
	bool debug = false;
};
#endif // OPENEXE_H
