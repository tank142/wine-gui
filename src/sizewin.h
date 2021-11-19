#ifndef SIZEWIN_H
#define SIZEWIN_H
#include <QSettings>
#include <QWidget>
#include <QStandardPaths>
class sizeWin {
public:
	sizeWin(QWidget*,QString);
	~sizeWin();
	void restore();
	void save();
	QSettings *settings_conf;
private:
	QWidget *win;
	QString name;
};

#endif // SIZEWIN_H
