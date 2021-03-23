#ifndef TABDXSETTINGS_H
#define TABDXSETTINGS_H

#include "main_target.h"
#include "fileregistry.h"
#include <QHBoxLayout>
#include <QRadioButton>
#include <QGroupBox>
#include <QDir>
#include <QFile>
#include <QTimer>
#include <QCheckBox>
class tabdxSettings : public QVBoxLayout
{
	Q_OBJECT
public:
	tabdxSettings(main_target *);
public slots:
	void update();
private:
	void check64();
	void wineDll(QString,bool*);
	QString winedirBit(QString,QString);
	void old(QString);
	void updateDX9();
	void updateDX11();
	void updateDXGI();
	main_target *target;
	QRadioButton *dx9wine;
	QRadioButton *dx9nine;
	QRadioButton *dx9dxvk;
	QRadioButton *dx11wine;
	QRadioButton *dx11dxvk;
	QCheckBox *dxgi;
	bool x64;
	QString nine;
	QString nine32;
private slots:
	void setDX9wine(bool);
	void setDX9nine(bool);
	void setDX9dxvk(bool);
	void setDX11wine(bool);
	void setDX11dxvk(bool);
	void setDXGI(bool);
};

#endif // TABDXSETTINGS_H
