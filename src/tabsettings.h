#ifndef TABSETTINGS_H
#define TABSETTINGS_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QPushButton>
#include <QGroupBox>
#include <QEventLoop>
#include "main_target.h"
#include "fileregistry.h"
#include "tabsettingsregkey.h"
#include "tabdxsettings.h"
#include "tabsyncsettings.h"
class tabSettings : public QWidget
{
	Q_OBJECT
public:
	explicit tabSettings(main_target *,QWidget *parent = nullptr);
	tabdxSettings *DX;
	tabsyncSettings *SYNC;
public slots:
	void updateReg(fileRegistry*);
	void cancel_slot();
	void ok_slot();
	void cancelOkEnable();
	static QVector<tabSettingsRegkey*> regSearch(QString,int,fileRegistry*,QVector<tabSettingsRegkey*>,QStringList);
private:
	main_target *target;
	QPushButton *cancel;
	QPushButton *ok;
	QVector<tabSettingsRegkey*> regWidgets;
	QVBoxLayout *REGEDIT;
	QVBoxLayout *Direct3D_l;
	QVBoxLayout *DirectInput_l;
signals:
	void write();
};

#endif // TABSETTINGS_H
