#ifndef TABTOOLS_H
#define TABTOOLS_H

#include <QWidget>
#include <QComboBox>
#include <QGridLayout>
#include "main_target.h"
class tabTools : public QWidget
{
	Q_OBJECT
public:
	explicit tabTools(main_target*,QWidget *parent = nullptr);
	void updateWine();
	QComboBox *wine;
private:
	main_target *target;
	QString prefix();
	QGridLayout *gbox;
private slots:
	void regedit();
	void winecfg();
	void control();
	void wineconsole();
	void taskmgr();
	void kill();
	void updateWineVer(QString);
	void updateWineDir();
	void openWineDir();
	void open();
	void update();
	void clear();
	void installComponents_slot();
	void installFonts_slot();
signals:
	void WineVer(QString);
};

#endif // TABTOOLS_H
