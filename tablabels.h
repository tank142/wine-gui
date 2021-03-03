#ifndef TABLABELS_H
#define TABLABELS_H

#include "main_target.h"
#include <QVBoxLayout>
#include <QTreeView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QWidget>

class tabLabels : public QWidget
{
	Q_OBJECT
public:
	explicit tabLabels(main_target*,QWidget *parent = nullptr);

private:
	QVBoxLayout *vbox;
	QTreeView *labels;
	QStandardItemModel *model;
	main_target *target;
	QIcon find_icon(QString);
	QString read_name(QString);
	QString prefixPath();
	bool fileExists(QString,QString);
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
	void del_slot();
public slots:
	void shortcuts();
};

#endif // TABLABELS_H
