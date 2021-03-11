#ifndef PREFIXDEL_H
#define PREFIXDEL_H

//#include "mainwindow.h"
#include <QWidget>
#include <QDir>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QThread>
#include <QStandardItemModel>
#include <QPushButton>
#include <tuple>
using namespace std;
class threadDel : public QThread
{
	Q_OBJECT
public:
	explicit threadDel(QString);
	~threadDel();
	void run()override;
	QDir *dir;
public slots:
	void work();
signals:
	void model_update();
};

class prefixDel : public QWidget
{
	Q_OBJECT
public:
	explicit prefixDel(threadDel*,QString,QWidget *parent = nullptr);
	QHBoxLayout *hbox;
	QVBoxLayout *vbox;
	threadDel *del;
	QPushButton *ok;
	void Del();
	void ok_slot();
	void cancel_slot();
private:
	QString target;
signals:
	void start();
	void model_update();
	void main_tool_widget_hide(bool);
};

#endif // PREFIXDEL_H
