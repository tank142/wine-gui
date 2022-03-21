#ifndef TABSYNCSETTINGS_H
#define TABSYNCSETTINGS_H
#include <QGroupBox>
#include <QRadioButton>
#include "main_target.h"
class tabsyncSettings : public QGroupBox {
	Q_OBJECT
public:
	explicit tabsyncSettings(main_target *t,QWidget *parent = nullptr);
	main_target *target;
	QRadioButton *wineserver;
	QRadioButton *esync;
	QRadioButton *fsync;
private:
	void wineserverSlot();
	void esyncSlot();
	void fsyncSlot();
	void updateFileshells(char);
public slots:
	void update();
signals:

};

#endif // TABSYNCSETTINGS_H
