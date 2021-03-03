#ifndef dircopyWidget_H
#define dircopyWidget_H

#include "dirwork.h"
#include <QWidget>
#include <QStatusBar>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>

class dirWidget : public QWidget
{
	Q_OBJECT
public:
	explicit dirWidget(dirWork*, QWidget *parent = nullptr);
	QString fileSize(quint64);
	dirWork *work;
private:
	QVBoxLayout *vbox;
	QHBoxLayout *hbox_bar;
	QStatusBar *status;
	QProgressBar *bar;
	QToolButton *close;
	QString maxSize_text;
	int unsigned pctSize;
public slots:
	void createBar(quint64);
	void updateBar(quint64);
	void closeBar();
	void updateStatus(QString);
	void stop();
	void exit();

signals:
	void close_s();
	void main_tool_widget_hide();
};

#endif // dircopyWidget_H
