#ifndef TABDLL_H
#define TABDLL_H

#include <QWidget>
#include "tabsettings.h"

class tabDll : public QWidget
{
	Q_OBJECT
public:
	explicit tabDll(QWidget *parent = nullptr);
	QPushButton *cancel;
	QPushButton *ok;
	QVector<tabSettingsRegkey*> regWidgets;
private:
	QVBoxLayout *DLL;
public slots:
	void updateReg(fileRegistry*);
	void cancel_slot();
	void ok_slot();
	void cancelOkEnable();

signals:
	void write();
};

#endif // TABDLL_H
