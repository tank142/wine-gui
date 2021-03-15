#ifndef SIZEWIN_H
#define SIZEWIN_H
#include <QSettings>
#include <QWidget>
#include <QStandardPaths>

class sizeWin
{
public:
	sizeWin(QWidget*,QString);
	~sizeWin();
	void restore();
	void save();
private:
	QWidget *win;
	QString name;
	QSettings *settings_conf;
};

#endif // SIZEWIN_H
