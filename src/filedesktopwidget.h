#ifndef FILEDESKTOPWIDGET_H
#define FILEDESKTOPWIDGET_H

#include "filedesktop.h"
#include "fileshell.h"
#include "main_target.h"
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QCheckBox>
#include <QComboBox>
class fileDesktopWidget : public QWidget
{
	Q_OBJECT
public:
	explicit fileDesktopWidget(QString,QString,main_target*,QWidget *parent = nullptr);
private:
	main_target *Target;
	fileDesktop *target;
	fileShell *shell;
	QLineEdit *command;
	QLineEdit *name;
	QPushButton *exec;
	QLineEdit *comment;
	QLineEdit *path;
	QLineEdit *icon;
	QLineEdit *wmclass;
	QLineEdit *shortcut;
	QComboBox *categories;
	QString prefix;
	QString shortcuts;

	QCheckBox *gamemodeCheck;
	QCheckBox *vkBasaltCheck;
	QCheckBox *libstrangleCheck;
	QCheckBox *dxvkCheck;
	QCheckBox *galliumCheck;
	QCheckBox *mangoCheck;

	QLineEdit *libstrangleEdit;
	QLineEdit *dxvkEdit;
	QLineEdit *galliumEdit;
	QLineEdit *mangoEdit;
	QString fixShortcut(QString);
	bool findShortcut(QString);

private slots:
	void ok_slot();
	void cancel_slot();
	void set_dir();
	void icon_extract();
signals:
	void shortcuts_update();
};

#endif // FILEDESKTOPWIDGET_H
