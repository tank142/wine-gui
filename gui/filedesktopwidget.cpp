#include "filedesktopwidget.h"
#include "iconextract.h"
#include "fileshell.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QDir>
#include <QFileDialog>
#include <QCheckBox>
#include <QSplitter>
#include <QAction>
#include <QRegExp>
#include <iostream>
using namespace std;
fileDesktopWidget::fileDesktopWidget(QString t,QString f,QWidget *parent) : QWidget(parent)
{
	prefix = t;shortcuts = f;
	QVBoxLayout *main = new QVBoxLayout(this);
	main->addWidget(new QSplitter(Qt::Vertical,this));


	QGroupBox *performance = new QGroupBox("Производительность",this);
	QVBoxLayout *performanceV = new QVBoxLayout(performance);
	QHBoxLayout *performanceL = new QHBoxLayout(performance);
	gamemodeCheck = new QCheckBox("GameMode",performance);
	esyncCheck = new QCheckBox("Esync",performance);
	fsyncCheck = new QCheckBox("Fsync",performance);
	performanceL->addWidget(esyncCheck);
	performanceL->addWidget(fsyncCheck);
	performanceL->addWidget(gamemodeCheck);


	QHBoxLayout *performanceLibstrangle = new QHBoxLayout(performance);
	libstrangleEdit = new QLineEdit(performance);
	libstrangleCheck = new QCheckBox(performance);
	performanceLibstrangle->addWidget(new QLabel("libStrangle"));
	performanceLibstrangle->addWidget(libstrangleEdit);
	performanceLibstrangle->addWidget(libstrangleCheck);
	performanceV->addLayout(performanceL);
	performanceV->addLayout(performanceLibstrangle);
	main->addWidget(performance);

	QGroupBox *hud = new QGroupBox("Мониторинг",this);
	QGridLayout *hudL = new QGridLayout(hud);
	hudL->addWidget(new QLabel("DXVK"),0,0);
	hudL->addWidget(new QLabel("Gallium"),1,0);
	hudL->addWidget(new QLabel("MangoHud"),2,0);
	dxvkEdit = new QLineEdit(hud);
	dxvkCheck = new QCheckBox(hud);
	hudL->addWidget(dxvkEdit,0,1);
	hudL->addWidget(dxvkCheck,0,2);

	galliumEdit = new QLineEdit(hud);
	galliumCheck = new QCheckBox(hud);
	hudL->addWidget(galliumEdit,1,1);
	hudL->addWidget(galliumCheck,1,2);

	mangoEdit = new QLineEdit(hud);
	mangoCheck = new QCheckBox(hud);
	hudL->addWidget(mangoEdit,2,1);
	hudL->addWidget(mangoCheck,2,2);
	main->addWidget(hud);

	QPushButton *ok = new QPushButton("Ок",this);
	QPushButton *cancel = new QPushButton("Отмена",this);
	//QToolButton *help = new QToolButton(this);help->setText("Помощь");
	QHBoxLayout *main_battons = new QHBoxLayout(this);
	main_battons->addWidget(ok);
	main_battons->addWidget(cancel);
	//main_battons->addWidget(help);

	QGridLayout *layout = new QGridLayout(this);
	command = new QLineEdit(this);layout->addWidget(command,2,1);layout->addWidget(new QLabel("Command"),2,0);
	name = new QLineEdit(this);layout->addWidget(name,3,1);layout->addWidget(new QLabel("Name"),3,0);
	comment = new QLineEdit(this);layout->addWidget(comment,4,1);layout->addWidget(new QLabel("Comment"),4,0);
	path = new QLineEdit(this);layout->addWidget(path,5,1);layout->addWidget(new QLabel("Path"),5,0);
	wmclass = new QLineEdit(this);layout->addWidget(wmclass,7,1);layout->addWidget(new QLabel("StartupWMClass"),7,0);
	categories = new QComboBox(this);layout->addWidget(categories,0,1);layout->addWidget(new QLabel("Categories"),0,0);
	categories->addItem(QIcon::fromTheme("applications-games"),"Game");
	categories->addItem(QIcon::fromTheme("applications-multimedia"),"AudioVideo");
	categories->addItem(QIcon::fromTheme("applications-internet"),"Network");
	categories->addItem(QIcon::fromTheme("applications-office"),"Office");
	categories->addItem(QIcon::fromTheme("applications-utilities"),"Utility");
	categories->addItem(QIcon::fromTheme("applications-system"),"System");
	if(shortcuts != ""){
		target = new fileDesktop(prefix + "/shortcuts/" + shortcuts,this);
		shell = new fileShell(prefix + "/shortcuts/" + shortcuts,this);
		command->setText(shell->EXEC);
		name->setText(target->Name);
		comment->setText(target->Comment);
		path->setText(shell->WORKDIR);
		wmclass->setText(target->StartupWMClass);
		esyncCheck->setChecked(shell->WINEESYNC);
		fsyncCheck->setChecked(shell->WINEFSYNC);
		gamemodeCheck->setChecked(shell->gamemoderun);
		libstrangleEdit->setText(shell->STRANGLE);
		libstrangleCheck->setChecked(shell->STRANGLE_enable);
		dxvkCheck->setChecked(shell->DXVK_HUD_enable);
		dxvkEdit->setText(shell->DXVK_HUD);
		galliumCheck->setChecked(shell->GALLIUM_HUD_enable);
		galliumEdit->setText(shell->GALLIUM_HUD);
		mangoCheck->setChecked(shell->MANGOHUD_CONFIG_enable);
		mangoEdit->setText(shell->MANGOHUD_CONFIG);
	}else{
		exec = new QPushButton(this);layout->addWidget(exec,1,1);layout->addWidget(new QLabel("Exec"),1,0);
		connect(exec,	&QPushButton::clicked , this , &fileDesktopWidget::set_dir);
		icon = new QLineEdit(this);layout->addWidget(icon,6,1);layout->addWidget(new QLabel("Icon"),6,0);
		shortcut = new QLineEdit(shortcuts,this);layout->addWidget(shortcut,8,1);layout->addWidget(new QLabel("Shortcut"),8,0);
		command->setText("\"$WINE\" \"$EXE\"");

	}
	main->addLayout(layout);
	main->addLayout(main_battons);

	connect(ok,		&QPushButton::clicked , this , &fileDesktopWidget::ok_slot);
	connect(cancel,	&QPushButton::clicked , this , &fileDesktopWidget::cancel_slot);
	setLayout(main);
}
void fileDesktopWidget::icon_extract(){
	QString t = exec->text();
	if(!QRegExp("\\$WINEPREFIX").indexIn(t)){
		t.replace("$WINEPREFIX",prefix);
	}
	iconExtract(icon->text(), t , prefix + "/shortcuts/" + shortcut->text()).start();
}
void fileDesktopWidget::ok_slot(){
	if(shortcuts == ""){
		target = new fileDesktop(prefix + "/shortcuts/" + shortcut->text(),this);
		shell = new fileShell(prefix + "/shortcuts/" + shortcut->text(),this);
		target->Icon = icon->text();
		icon_extract();
		target->Path = prefix + "/shortcuts/" + shortcut->text();
		shell->EXE = exec->text().right(exec->text().size() - exec->text().lastIndexOf("/")-1);
	}
	shell->WORKDIR = path->text();
	shell->gamemoderun = gamemodeCheck->isChecked();
	shell->WINEESYNC = esyncCheck->isChecked();
	shell->WINEFSYNC = fsyncCheck->isChecked();
	shell->STRANGLE_enable = libstrangleCheck->isChecked();
	shell->DXVK_HUD_enable = dxvkCheck->isChecked();
	shell->GALLIUM_HUD_enable = galliumCheck->isChecked();
	shell->MANGOHUD_CONFIG_enable = mangoCheck->isChecked();
	shell->STRANGLE = libstrangleEdit->text();
	shell->DXVK_HUD = dxvkEdit->text();
	shell->GALLIUM_HUD = galliumEdit->text();
	shell->MANGOHUD_CONFIG = mangoEdit->text();
	shell->EXEC = command->text();
	target->Name = name->text();
	target->Exec = "env WINEDEBUG=\"-all\" ./exec.sh";
	target->Comment = comment->text();
	target->Categories = categories->currentText();
	target->StartupWMClass = wmclass->text();
	target->write();
	shell->write();
	emit shortcuts_update();
	deleteLater();
}
void fileDesktopWidget::cancel_slot(){deleteLater();}
void fileDesktopWidget::set_dir(){
	QString d=(QFileDialog::getOpenFileName(this,
				QString::fromUtf8("Выбор файла"),
				prefix + "/drive_c"));
	if(d != "" ){
		if(!QRegExp(prefix).indexIn(d)){
			exec->setText("$WINEPREFIX" + d.remove(prefix));
			path->setText("$WINEPREFIX" + d.left(d.lastIndexOf("/")));
		}else{
			exec->setText(d);
			path->setText(d.left(d.lastIndexOf("/")));
		}
		name->setText(d.right(d.size() - d.lastIndexOf("/")-1));
		icon->setText(name->text().left( name->text().lastIndexOf(".")));
		wmclass->setText(icon->text());
		shortcut->setText(wmclass->text());
		name->setText(icon->text());
	}
}
