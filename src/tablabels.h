#ifndef TABLABELS_H
#define TABLABELS_H

#include "main_target.h"
#include "filedesktopwidget.h"
#include "shell.h"
#include <QVBoxLayout>
#include <QTreeView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QWidget>
#include <QThread>
class tabLabels : public QWidget
{
	Q_OBJECT
public:
	explicit tabLabels(main_target*,QWidget *parent = nullptr);
	fileDesktopWidget *file;
	QTreeView *labels;
private:
	QVBoxLayout *vbox;
	QStandardItemModel *model;
	main_target *target;
	QIcon find_icon(QString);
	QString read_name(QString);;
	bool fileExists(QString,QString);
	void updateIcon();
private slots:
	void clicked2();
	void runOpen();
	void runDebugging();
	void rightClicked();
	void add_menu_slot();
	void add_desktop_slot();
	void del_menu_slot();
	void del_desktop_slot();
	void add_slot();
	void editor_slot();
	void change_icon_slot();
	void del_slot();
	void labels_connect();
public slots:
	void shortcuts();
signals:
	void hide();
};
class update_file : public QThread
{
	Q_OBJECT
public:
	explicit update_file(QProcess *proc){update = proc;};
		QProcess *update;
private:
		void run() override;
};
#endif // TABLABELS_H
