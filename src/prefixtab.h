#ifndef PREFIXTAB_H
#define PREFIXTAB_H
#include "tabdll.h"
#include "tabsettings.h"
#include "main_target.h"
#include "tablabels.h"
#include "tabtools.h"
#include <QWidget>
#include <QTabWidget>
#include <QDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QGroupBox>
class ToolsTab : public QWidget
{
	Q_OBJECT

public:
	explicit ToolsTab(main_target*,QWidget *parent = nullptr);
	tabLabels *l;
	tabTools *t;
public slots:
	void hide();
	void unhide();
};
class dllTab : public QWidget
{
	Q_OBJECT

public:
	explicit dllTab(QWidget *parent = nullptr);
	tabDll *dll;
};

class settingsTab : public QWidget
{
	Q_OBJECT
public:
	explicit settingsTab(main_target *target,fileRegistry *reg,QWidget *parent = nullptr);
	tabSettings *settings;
public slots:
private:
	fileRegistry *reg;
};
/*
class TabDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TabDialog(QWidget *parent = nullptr);

private:
	QTabWidget *tabWidget;
};
*/
class prefixTab : public QWidget
{
	Q_OBJECT
public:
	explicit prefixTab(main_target*,QWidget *parent = nullptr);
	QTabWidget *tabWidget;
	ToolsTab *tools;
	main_target *target;
	fileRegistry *reg;
	tabdxSettings *DX;
	tabsyncSettings *SYNC;
private:
	bool regExist;
public slots:
	void readReg(int);
	void deleteReg();
	void write();
signals:
	void updateReg(fileRegistry*);
};
#endif // PREFIXTAB_H
