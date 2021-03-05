#include "findlink.h"
#include "shortcutread.h"
#include "tablabels.h"
#include "shell.h"
#include "shelloutput.h"
#include <QGridLayout>
#include <QDir>
#include <QIcon>
#include <QHeaderView>
#include <QStandardPaths>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>
#include <iostream>
using namespace std;
tabLabels::tabLabels(main_target *t,QWidget *parent) : QWidget(parent)
{
	target = t;
	vbox = new QVBoxLayout(this);
	model = new QStandardItemModel;
	model->setHorizontalHeaderLabels(QStringList() << "Имя" << "Ярлык" );
	labels = new QTreeView(this);
	labels->setModel(model);
	labels->header()->setStretchLastSection(false);
	labels->header()->setSectionResizeMode(0,QHeaderView::Stretch);
	labels->setEditTriggers(QTreeView::NoEditTriggers);
	labels->setContextMenuPolicy(Qt::CustomContextMenu);
	vbox->addWidget(labels);
	setLayout(vbox);
	connect(labels,	&QTreeView::doubleClicked , this , &tabLabels::clicked2);
	connect(labels,	&QTreeView::customContextMenuRequested, this, &tabLabels::rightClicked);
}
void tabLabels::labels_connect(){
	connect(labels,	&QTreeView::customContextMenuRequested, this, &tabLabels::rightClicked);
}
void tabLabels::rightClicked()
{
	QMenu *m_menu = new QMenu("&SubMenu",this);
	if(labels->currentIndex().row() > -1){
		QAction *actionDebugging = new QAction("Отладка",m_menu);
		m_menu->addAction(actionDebugging);
		connect(actionDebugging, &QAction::triggered, this, &tabLabels::runDebugging);
		QAction *actionOpen = new QAction("Открыть папку",m_menu);
		m_menu->addAction(actionOpen);
		connect(actionOpen, &QAction::triggered, this, &tabLabels::runOpen);
		QAction *editorOpen = new QAction("Редактировать",m_menu);
		m_menu->addAction(editorOpen);
		connect(editorOpen, &QAction::triggered, this, &tabLabels::editor_slot);
		if(!findLink(prefixPath() + + "/shortcuts/" + model->item(labels->currentIndex().row(),1)->text(),
					target->home + "/.local/share/applications/wine/").ckRc()){
			QAction *action = new QAction("Добавить в главное меню");
			connect(action, &QAction::triggered , this , &tabLabels::add_menu_slot);
			m_menu->addAction(action);
		}else{
			QAction *action = new QAction("Удалить из главного меню");
			connect(action, &QAction::triggered , this , &tabLabels::del_menu_slot);
			m_menu->addAction(action);
		}
		if(!findLink(prefixPath() + "/shortcuts/" + model->item(labels->currentIndex().row(),1)->text(),
					QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)).ck()){
			QAction *action = new QAction("Добавить на рабочий стол");
			connect(action, &QAction::triggered , this , &tabLabels::add_desktop_slot);
			m_menu->addAction(action);
		}else{
			QAction *action = new QAction("Удалить с рабочего стола");
			connect(action, &QAction::triggered , this , &tabLabels::del_desktop_slot);
			m_menu->addAction(action);
		}
		QAction *del = new QAction("Удалить");
		connect(del, &QAction::triggered , this , &tabLabels::del_slot);
		m_menu->addAction(del);
		m_menu->addSeparator();
	}
	QAction *add = new QAction("Создать");
	connect(add, &QAction::triggered , this , &tabLabels::add_slot);
	m_menu->addAction(add);
	m_menu->popup(QCursor::pos());
	m_menu->show();
}
void tabLabels::clicked2(){
	shell *wine;
	QString prefix = prefixPath();
	wine = new shell("wine",QStringList());
	wine->envSetup(target);
	wine->env->insert("WINEDEBUG","-all");
	wine->proc->setWorkingDirectory(prefix + "/shortcuts/" + model->item(labels->currentIndex().row(),1)->text());
	wine->exec = prefix + "/shortcuts/" + model->item(labels->currentIndex().row(),1)->text() + "/exec.sh";
	wine->start();
}
QString tabLabels::prefixPath(){
	if(target->prefix != ""){
		return QString(target->model_storages.at(target->storage) + "/" + target->prefix);
	}
	return QString(target->home + "/.wine");
}
void tabLabels::runDebugging(){
	shellOutputDebugging *wine;
	QString prefix = prefixPath();
	wine = new shellOutputDebugging(prefix + "/shortcuts/" + model->item(labels->currentIndex().row(),1)->text() + "/exec.sh",QStringList());
	wine->exec->proc->setWorkingDirectory(prefix + "/shortcuts/" + model->item(labels->currentIndex().row(),1)->text());
	QProcessEnvironment env(QProcessEnvironment::systemEnvironment());
	env.insert("WINEDEBUG","");
	wine->exec->proc->setProcessEnvironment(env);
	wine->start();
}
void tabLabels::runOpen(){
	QString prefix = prefixPath();
	QFile FILE(prefix + "/shortcuts/" + model->item(labels->currentIndex().row(),1)->text() + "/exec.sh");
	if (FILE.exists()){
		FILE.open(QFile::ReadOnly);
		QTextStream t(&FILE);
		QRegExp rx("WORKDIR=");
		while(!t.atEnd()){
		  QString l = t.readLine();
		  if(!rx.indexIn(l)){
			  FILE.close();
			  l.replace(rx,"");
			  l.replace(QRegExp("\\$WINEPREFIX"),prefix);
			  l.remove(QRegExp("^\""));
			  l.remove(QRegExp("\"$"));
			  shell *open = new shell("xdg-open",QStringList() << l);
			  open->start();
		  }
		}
		FILE.close();
	}
}
QIcon tabLabels::find_icon(QString dir){
	foreach(QString s,QStringList() << "16x16" << "20x20" << "22x22" << "24x24" << "36x36"
		<< "32x32" << "40x40" << "41x47" << "48x48" << "48x96" << "60x60" << "64x64"
		<< "72x72" << "80x80" << "96x96"<< "128x128" << "150x150" << "160x160" << "192x192"
		<< "256x256" << "14x14" << "13x256" << "13x13" << "10x10" << "8x8"){
		if(QDir(dir + "/icon/" + s).exists()){
			QDir i;i.setFilter(QDir::NoDotAndDotDot | QDir::Files);i.setPath(dir + "/icon/" + s);
			return QIcon(i.path() + "/" + i.entryList().at(0));
		}
	}
	return QIcon();
}
QString tabLabels::read_name(QString s){
	QFile FILE(s + "/exec.desktop");
	if (FILE.exists()){
		FILE.open(QFile::ReadOnly);
		QTextStream t(&FILE);
		while(!t.atEnd()){
		  QString l = t.readLine();
		  QRegExp rx("^Name=");
		  if(!rx.indexIn(l)){
			  FILE.close();
			  return l.remove(rx);
		  }
		}
		FILE.close();
	}
	return "";
}
void tabLabels::shortcuts(){
	while (model->rowCount() > 0){
		model->item(0)->~QStandardItem();
		model->removeRow(0);
	}
	QDir d;d.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	QString prefix = prefixPath();
	d.setPath(prefix + "/shortcuts");
	QSettings conf(prefix + "/WINE.cfg",QSettings::IniFormat);conf.setIniCodec( "UTF-8" );
	QString wine = conf.value("WINE").toString();
	if(wine == "System" || wine.size() == 0){
		wine = "wine";
	}else{
		wine = (target->WINE_VER + "/" + wine + "/bin/wine");
	}
	foreach(QString s,d.entryList()){
		shortcutRead( d.path() + "/" + s,wine,target);
		conf.deleteLater();
		QList<QStandardItem *> shortcut;
		shortcut.append(new QStandardItem(find_icon(d.path() + "/" + s),read_name(d.path() + "/" + s)));
		shortcut.append(new QStandardItem(s.right(s.size() - s.lastIndexOf("/")-1)));
		model->appendRow(shortcut);
	}
}
bool tabLabels::fileExists(QString s,QString f){
	QFile file(f);
	if(!file.exists()){return true;}
	QFileInfo fileInf(f);
	if(fileInf.isSymbolicLink()){
		if(fileInf.symLinkTarget() == s){return true;}
	}
	QMessageBox msgBox(this);
	msgBox.setText("Файл cуществует: \"" + f +  "\"\nПерезаписать?");
	QAbstractButton *Yes = msgBox.addButton(tr("Да"), QMessageBox::YesRole);
	msgBox.addButton(tr("Нет"), QMessageBox::NoRole);
	msgBox.exec();
	if(msgBox.clickedButton()==Yes){
		file.remove();
		return true;
	}
	return false;
}
void tabLabels::add_menu_slot(){
	if(!QDir(target->home + "/.local/share/applications/wine/Programs/").exists()){
		QDir().mkpath(target->home + "/.local/share/applications/wine/Programs");
	}
	QString prefix = prefixPath();
	QDir d(prefix + "/shortcuts/" + model->item(labels->currentIndex().row(),1)->text() + "/icon");
	QDir().mkpath(target->home + "/.local/share/icons/hicolor");
	d.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
	foreach(QString dir,d.entryList()){
		QDir().mkpath(target->home + "/.local/share/icons/hicolor/" + dir);
		QString name = QDir(d.path() + "/" + dir ).entryList().at(2);
		QFile file(d.path() + "/" + dir + "/" + name);
		QFile l(target->home + "/.local/share/icons/hicolor/" + dir + "/apps/" + name);
		if(l.exists()){l.remove();}
		if(!fileExists(d.path() + "/" + dir + "/" + name,
					   target->home + "/.local/share/icons/hicolor/" + dir + "/apps/" + name)){return;}
		file.link(target->home + "/.local/share/icons/hicolor/" + dir + "/apps/" + name);
	}
	if(!fileExists(prefix + "/shortcuts/" + model->item(labels->currentIndex().row(),1)->text() + "/exec.desktop",
				target->home + "/.local/share/applications/wine/Programs/" + model->item(labels->currentIndex().row(),1)->text() + ".desktop")){return;}
	QFile::link(prefix + "/shortcuts/" + model->item(labels->currentIndex().row(),1)->text() + "/exec.desktop",
				target->home + "/.local/share/applications/wine/Programs/" + model->item(labels->currentIndex().row(),1)->text() + ".desktop");
}
void tabLabels::add_desktop_slot(){
	if(!QDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)).exists()){
		QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
	}
	QString prefix = prefixPath();
	QDir d(prefix + "/shortcuts/" + model->item(labels->currentIndex().row(),1)->text() + "/icon");
	QDir().mkpath(target->home + "/.local/share/icons/hicolor");
	d.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
	foreach(QString dir,d.entryList()){
		QDir().mkpath(target->home + "/.local/share/icons/hicolor/" + dir);
		QString name = QDir(d.path() + "/" + dir ).entryList().at(2);
		QFile file(d.path() + "/" + dir + "/" + name);
		if(!fileExists(d.path() + "/" + dir + "/" + name,
					   target->home + "/.local/share/icons/hicolor/" + dir + "/apps/" + name)){return;}
		file.link(target->home + "/.local/share/icons/hicolor/" + dir + "/apps/" + name);
	}
	if(!fileExists(prefix + "/shortcuts/" + model->item(labels->currentIndex().row(),1)->text() + "/exec.desktop",
				   QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" + model->item(labels->currentIndex().row(),1)->text() + ".desktop")){return;}
	QFile::link(prefix + "/shortcuts/" + model->item(labels->currentIndex().row(),1)->text() + "/exec.desktop",
				QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" + model->item(labels->currentIndex().row(),1)->text() + ".desktop");
}
void tabLabels::del_menu_slot(){
	findLink(prefixPath() + "/shortcuts/" + model->item(labels->currentIndex().row(),1)->text(),
			 target->home + "/.local/share/applications").rmRc();
}
void tabLabels::del_desktop_slot(){
	findLink(prefixPath() + "/shortcuts/" + model->item(labels->currentIndex().row(),1)->text(),
			 QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)).rm();
}
void tabLabels::add_slot(){
	file = new fileDesktopWidget(prefixPath(), "");
	vbox->addWidget(file);
	connect(file, &fileDesktopWidget::shortcuts_update , this , &tabLabels::shortcuts);
	disconnect(labels,	&QTreeView::customContextMenuRequested, this, &tabLabels::rightClicked);
	connect(file,	&fileDesktopWidget::destroyed, this, &tabLabels::labels_connect);
	emit hide();
}
void tabLabels::editor_slot(){
	fileDesktopWidget *file;
	file = new fileDesktopWidget(prefixPath(), model->item(labels->currentIndex().row(),1)->text());
	vbox->addWidget(file);
	connect(file, &fileDesktopWidget::shortcuts_update , this , &tabLabels::shortcuts);
}
void tabLabels::del_slot(){
	QString prefix = prefixPath();
	findLink(prefix + "/shortcuts/" + model->item(labels->currentIndex().row(),1)->text(),
			 target->home + "/.local/share/applications").rmRc();
	findLink(prefix + "/shortcuts/" + model->item(labels->currentIndex().row(),1)->text(),
			 target->home + "/.local/share/icons").rmRc();
	findLink(prefix + "/shortcuts/" + model->item(labels->currentIndex().row(),1)->text(),
			 QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)).rm();
	QDir(prefix + "/shortcuts/" + model->item(labels->currentIndex().row(),1)->text()).removeRecursively();
	shortcuts();
}
