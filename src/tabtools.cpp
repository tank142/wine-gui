#include "tabtools.h"
#include "shelloutput.h"
#include <QPushButton>
#include <QToolButton>
#include <QSettings>
#include <QVBoxLayout>
#include <QLabel>
#include <QDir>
tabTools::tabTools(main_target *t,QWidget *parent) : QWidget(parent)
{
	target = t;

	QVBoxLayout *vbox = new QVBoxLayout;vbox->setAlignment(Qt::AlignTop);
	gbox = new QGridLayout;gbox->setAlignment(Qt::AlignTop);
	QPushButton *button0 = new QPushButton(tr("regedit"));//"Редактор реестра"
	QFont font = button0->font();
	font.setPointSize(9);button0->setFont(font);
	QPushButton *button1 = new QPushButton(tr("winecfg"));button1->setFont(font);//"Настройка Wine"
	QPushButton *button2 = new QPushButton(tr("control"));button2->setFont(font);//"Панель управления"
	QPushButton *button10 = new QPushButton(tr("cmd"));button10->setFont(font);//"Терминал"
	QPushButton *button11 = new QPushButton(tr("taskmgr"));button11->setFont(font);//"Диспетчер задач"
	QPushButton *button12 = new QPushButton(tr("kill"));button12->setFont(font);//"Завершить процессы"
	QPushButton *button20 = new QPushButton(tr("open"));button20->setFont(font);//"Открыть"
	QPushButton *button21 = new QPushButton(tr("update"));button21->setFont(font);//"Обновить"
	QPushButton *button22 = new QPushButton(tr("clear"));button22->setFont(font);//"Очистить"
	QHBoxLayout *hbox = new QHBoxLayout;
	QPushButton *installComponents = new QPushButton(tr("install_сomponents"));installComponents->setFont(font);//"Установка компонентов"
	QPushButton *installFonts = new QPushButton(tr("install_fonts"));installFonts->setFont(font);//"Установка шрифтов"
	QHBoxLayout *hbox2 = new QHBoxLayout;
	wine = new QComboBox(this);updateWineDir();
	wineD = new QFileSystemWatcher(this);
	wineD->addPath(target->WINE_VER);
	QToolButton *wineButton = new QToolButton(this);
	wineButton->setIcon(QIcon::fromTheme("folder"));
	QLabel *x = new QLabel(tr("wine_ver"));//"Версия Wine"
	x->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
	hbox2->addWidget(x);
	hbox2->addWidget(wine);
	hbox2->addWidget(wineButton);
	hbox->addWidget(installComponents);
	hbox->addWidget(installFonts);
	gbox->addWidget(button0,0,0);
	gbox->addWidget(button1,0,1);
	gbox->addWidget(button2,0,2);
	gbox->addWidget(button10,1,0);
	gbox->addWidget(button11,1,1);
	gbox->addWidget(button12,1,2);
	gbox->addWidget(button20,2,0);
	gbox->addWidget(button21,2,1);
	gbox->addWidget(button22,2,2);
	vbox->addLayout(gbox);
	vbox->addLayout(hbox);
	vbox->addLayout(hbox2);
	setLayout(vbox);
	connect(button0, &QPushButton::clicked , this , &tabTools::regedit);
	connect(button1, &QPushButton::clicked , this , &tabTools::winecfg);
	connect(button2, &QPushButton::clicked , this , &tabTools::control);
	connect(button10, &QPushButton::clicked , this , &tabTools::wineconsole);
	connect(button11, &QPushButton::clicked , this , &tabTools::taskmgr);
	connect(button12, &QPushButton::clicked , this , &tabTools::kill);
	connect(button20, &QPushButton::clicked , this , &tabTools::open);
	connect(button21, &QPushButton::clicked , this , &tabTools::update);
	connect(button22, &QPushButton::clicked , this , &tabTools::clear);
	connect(wine, &QComboBox::currentTextChanged , this , &tabTools::updateWineVer);
	connect(wineD, &QFileSystemWatcher::directoryChanged , this , &tabTools::updateWineDir);
	connect(wineButton, &QPushButton::clicked , this , &tabTools::openWineDir);
	connect(installComponents, &QPushButton::clicked , this , &tabTools::installComponents_slot);
	connect(installFonts, &QPushButton::clicked , this , &tabTools::installFonts_slot);
}
void tabTools::wineDir(){
	wineD->addPath(target->WINE_VER);
	updateWineDir();
}
void tabTools::installComponents_slot(){
	shellOutputDebugging *winetricks = new shellOutputDebugging("wine",QStringList() << "-q" << "-f" << "dlls");
	winetricks->exec->envSetup(target);
	winetricks->exec->env->insert("WINE",winetricks->exec->exec);
	winetricks->exec->exec = "winetricks";
	winetricks->start();
}
void tabTools::installFonts_slot(){
	shellOutputDebugging *winetricks = new shellOutputDebugging("wine",QStringList() << "-q" << "-f" << "fonts");
	winetricks->exec->envSetup(target);
	winetricks->exec->env->insert("WINE",winetricks->exec->exec);
	winetricks->exec->exec = "winetricks";
	winetricks->start();
}
void tabTools::regedit(){
	shell *wine = new shell("wine",QStringList("regedit"));
	wine->envSetup(target);
	wine->start();
}
void tabTools::winecfg(){
	shell *wine = new shell("wine",QStringList("winecfg"));
	wine->envSetup(target);
	wine->start();
}
void tabTools::control(){
	shell *wine = new shell("wine",QStringList("control"));
	wine->envSetup(target);
	wine->start();
}
void tabTools::wineconsole(){
	shell *wine = new shell("wineconsole",QStringList());
	wine->envSetup(target);
	wine->start();
}
void tabTools::taskmgr(){
	shell *wine = new shell("wine",QStringList("taskmgr"));
	wine->envSetup(target);
	wine->start();
}
void tabTools::kill(){
	shell *wine = new shell("wineserver",QStringList("-k"));
	wine->envSetup(target);
	wine->exec = "wineserver";
	wine->start();
}
QString tabTools::prefix(){
	if(target->storage > 0 && target->prefix != ""){
		return (target->model_storages.at(target->storage) + "/" + target->prefix + "/");
	}
	return (target->home + "/.wine/");
}
void tabTools::updateWineVer(QString t){
	shell *wineShell = new shell("wineserver",QStringList() << "-k" << "-w");
	wineShell->envSetup(target);
	wineShell->exec = "wineserver";
	wineShell->start();
	wineShell->wait(-1);
	QString prefix;
	if(target->storage > 0 && target->prefix != ""){
		prefix = (target->model_storages.at(target->storage) + "/" + target->prefix + "/");
	}else{
		prefix = (target->home + "/.wine/");
	}
	QSettings conf(prefix + "/WINE.cfg",QSettings::IniFormat);
	conf.setValue("WINE",t);
	conf.sync();
	emit WineVer(t);
}
void tabTools::updateWineDir(){
	wine->clear();
	wine->addItem("System");
	if(target->WINE_VER.size() < 1){return;}
	QDir dir(target->WINE_VER);dir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Hidden);
	foreach(QString x, dir.entryList() ){
		wine->addItem(x);
	}
}
void tabTools::openWineDir(){
	shell *wine = new shell("xdg-open",QStringList() << target->WINE_VER);
	wine->start();
}
void tabTools::updateWine(){
	QString prefix;
	if(target->storage > 0 && target->prefix != ""){
		prefix = (target->model_storages.at(target->storage) + "/" + target->prefix + "/");
	}else{
		prefix = (target->home + "/.wine/");
	}
	if(QFile(prefix + "/WINE.cfg").exists()){
		QSettings conf(prefix + "/WINE.cfg",QSettings::IniFormat);
		QString w = conf.value("WINE").toString();
		if(w != "" && w != "System"){
			wine->setCurrentText(w);
			return;
		}
	}
	wine->setCurrentText("System");
	return;
}
void tabTools::open(){QStringList path;
	if(target->storage > 0 && target->prefix != ""){
		path.append(target->model_storages.at(target->storage) + "/" + target->prefix + "/drive_c");
	}else{
		path.append(target->home + "/.wine/drive_c");
	}
	shell *wine = new shell("xdg-open",path);
	wine->start();
}
void tabTools::update(){
	shell *wine = new shell("wineserver",QStringList() << "-k" << "-w");
	wine->envSetup(target);
	wine->exec = "wineserver";
	wine->start();
	wine->wait(-1);
	shell *wine2 = new shell("wineboot",QStringList("-u"));
	wine2->envSetup(target);
	wine2->start();
}
void tabTools::clear(){QString path;
	if(target->storage > 0 && target->prefix != ""){
		path.append(target->model_storages.at(target->storage) + "/" + target->prefix);
	}else{
		path.append(target->home + "/.wine");
	}
	shell *wine = new shell("wineserver",QStringList() << "-k" << "-w");
	wine->envSetup(target);
	wine->exec = "wineserver";
	wine->start();
	wine->wait(-1);
	QDir(path + "/drive_c/windows").removeRecursively();
	QFile(path + "/system.reg").remove();
	QFile(path + "/user.reg").remove();
	shell *wine2 = new shell("wineboot",QStringList("-u"));
	wine2->envSetup(target);
	wine2->start();
}
