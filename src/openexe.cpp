#include "openexe.h"
#include "sizewin.h"
#include "main_target.h"
#include "shelloutput.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSizePolicy>
#include <QFileDialog>
#include <QSettings>
#include <QRegularExpression>
#include <QModelIndex>
#include <QHeaderView>
#include <QRegularExpression>
openEXE::openEXE(main_target *t,QStringList a,QWidget *parent) : QWidget(parent)
{
	target = t;
	Key_Return = new QShortcut(this);
	Key_Return->setKey(Qt::Key_Return);
	open = new QPushButton(this);
	QString argf(arg(a));
	if(QFile(argf).exists()){open->setText(argf);}
	setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding );
	QVBoxLayout *VBOX = new QVBoxLayout(this);
	table = new prefixTable(target,this);
	QHBoxLayout *hbox = new QHBoxLayout();
	debug = new QCheckBox(tr("debug"),this);
	debug->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
	hbox->addWidget(open);
	hbox->addWidget(debug);
	VBOX->addLayout(hbox);
	VBOX->addWidget(table->treeView);
	table->treeView->header()->setStretchLastSection(false);
	table->treeView->header()->setSectionResizeMode(0,QHeaderView::Stretch);
	table->model_update();
	setLayout(VBOX);
	connect(open, &QPushButton::clicked , this , &openEXE::exe);
	connect(table, &prefixTable::readFinished , this , &openEXE::tableRestore);
	connect(table->treeView, &QTreeView::clicked , this , &openEXE::run);
	connect(Key_Return, &QShortcut::activated , this , &openEXE::key_return);
	setWindowTitle("Wine GUI");
	sizeWin(this,"openEXE").restore();
	setAttribute(Qt::WA_DeleteOnClose);
}
openEXE::~openEXE(){
	sizeWin(this,"openEXE").save();
}
QString openEXE::arg(QStringList arg){
	if(arg.size() > 1){
		if(arg.at(1) == "--wine-debug"){arg.removeAt(1);}
		arg.removeAt(0);
		QString argf;
		foreach(QString a,arg){argf = argf + a;}
		argf.replace(QRegularExpression("^~/"),QDir::homePath() + "/");
		argf.replace(QRegularExpression("^./"),QDir::currentPath() + "/");
		return argf;
	}
	return "";
}
void openEXE::key_return(){
	run(table->treeView->currentIndex());
}
void openEXE::tableRestore(){
	QSettings settings_conf(target->CONF,QSettings::IniFormat);
	#if QT_VERSION < 0x060000
		settings_conf.setIniCodec("UTF-8");
	#endif
	debug->setChecked(settings_conf.value("open/debug").toBool());
	QStringList wine_storages = settings_conf.childGroups();
	saveStorage = settings_conf.value("open/storage").toString();
	if(saveStorage == "0"){saveStorageIndex = 0;
		QModelIndex indexFromModel = table->model->item(0)->index();
		table->treeView->setCurrentIndex(indexFromModel);return;}
	savePrefix = settings_conf.value("open/prefix").toString();
	saveStorageIndex = -1;
	if(saveStorage != ""){
		QRegularExpression rx("^storage_[0-9]*$");
		quint16 index = 1;
		foreach(QString storage,wine_storages){
			if (rx.match(storage).hasMatch()){
				if(saveStorage == settings_conf.value(storage + "/path").toString()){
					saveStorageIndex = index; break;
				}
				index++;
			}
		}
	}
	if(saveStorageIndex > 0){
		for(int i = 0;i < table->model->item(saveStorageIndex)->rowCount();i++){
			if(savePrefix == table->model->item(saveStorageIndex)->child(i)->text()){
				QModelIndex indexFromModel = table->model->item(saveStorageIndex)->child(i)->index();
				table->treeView->setCurrentIndex(indexFromModel);
			}
		}
	}
}
void openEXE::exe(){
	QString d=(QFileDialog::getOpenFileName(this,
				tr("file_selection"),
				open->text()));
	if(d!=""){open->setText(d);};
}
QString openEXE::findWineBin(QModelIndex storage_index){
	if(storage_index.parent().row() == -1){
		if(table->model->item(0,1)->text() == "System"){
			return "wine";
		}else{
			return target->WINE_VER + "/" + table->model->item(0,1)->text() + "/bin/wine";
		}
	}
	if(table->model->item(storage_index.parent().row())->child(storage_index.row(),1)->text() == "System"){
		return "wine";
	}else{
		if(storage_index.parent().row() == -1){
			return target->WINE_VER + "/" + table->model->item(storage_index.row(),1)->text() + "/bin/wine";
		}else{
			return target->WINE_VER + "/" + table->model->item(storage_index.parent().row())->child(storage_index.row(),1)->text() + "/bin/wine";
		}
	}
}
QString openEXE::findWinePrefix(QModelIndex storage_index){
	if(storage_index.parent().row() == -1){
		if(storage_index.row() == 0){
			return QDir::homePath() + "/.wine";
		}
	}else{
		return get<2>(target->storages.at(storage_index.parent().row() - 1 )) + "/"
		+ table->model->item(storage_index.parent().row())->child(storage_index.row())->text();
	}
	return "";
}
void openEXE::run(QModelIndex storage_index){
	QSettings settings_conf(target->CONF,QSettings::IniFormat);
	#if QT_VERSION < 0x060000
		settings_conf.setIniCodec("UTF-8");
	#endif
	if(storage_index.parent().row() > -1){
		settings_conf.setValue("open/storage",get<2>(target->storages.at(storage_index.parent().row() - 1)));
		settings_conf.setValue("open/prefix",table->model->item(storage_index.parent().row())->child(storage_index.row())->text());
	}else{
		if(storage_index.row() > 0){return;}
		settings_conf.setValue("open/storage",0);
	}
	settings_conf.setValue("open/debug",debug->isChecked());
	settings_conf.sync();settings_conf.deleteLater();
	if(!QFile(open->text()).exists()){return;}
	QString wineBin = findWineBin(storage_index);
	if(debug->isChecked()){
		shellOutputDebugging *wine = new shellOutputDebugging(wineBin,QStringList() << open->text());
		wine->exec->env->insert("WINEPREFIX",findWinePrefix(storage_index));
		wine->exec->env->insert("WINEDEBUG","");
		wine->exec->proc->setWorkingDirectory(open->text().left(open->text().lastIndexOf("/")));
		wine->start();
	}else{
		setAttribute(Qt::WA_QuitOnClose,false);
		shell *wine = new shell(wineBin,QStringList() << open->text());
		wine->env->insert("WINEPREFIX",findWinePrefix(storage_index));
		wine->env->insert("WINEDEBUG","-all");
		wine->proc->setWorkingDirectory(open->text().left(open->text().lastIndexOf("/")));
		connect(wine, &shell::destroyed , qApp , &QApplication::quit);
		wine->start();
	}
	this->deleteLater();
}
EXEinStorages::EXEinStorages(main_target *t,QStringList a,QObject *parent) : QObject(parent){
	target = t;exe = openEXE::arg(a);
	if( a.size() > 1  && a.at(1) == "--wine-debug"){debug = true;}
}
bool EXEinStorages::exec(){
	for (int i = 0; i < target->storages.size(); i++){
		if(QRegularExpression("^" + get<2>(target->storages.at(i))).match(exe).hasMatch()){
			QDir storage(get<2>(target->storages.at(i)));storage.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
			foreach(QString prefix,storage.entryList()){
				if(QRegularExpression("^" + get<2>(target->storages.at(i)) + "/" + prefix + "/.*").match(exe).hasMatch()){
					target->storage = i + 1;
					target->prefix = prefix;
					if(debug){
						shellOutputDebugging *wine = new shellOutputDebugging("wine",QStringList() << exe.right(exe.size() - exe.lastIndexOf("/") - 1));
						connect(wine, &shell::destroyed , qApp , &QApplication::quit);
						wine->exec->envSetup(target);
						wine->exec->env->insert("WINEDEBUG","");
						wine->exec->proc->setWorkingDirectory(exe.left(exe.lastIndexOf("/")));
						wine->show();
						wine->start();
					}else{
						shell *wine = new shell("wine",QStringList() << exe.right(exe.size() - exe.lastIndexOf("/") - 1));
						connect(wine, &shell::destroyed , qApp , &QApplication::quit);
						wine->envSetup(target);
						wine->env->insert("WINEDEBUG","-all");
						wine->proc->setWorkingDirectory(exe.left(exe.lastIndexOf("/")));
						wine->start();
					}
					break;
				}
			}
			return true;
		}
	}
	return false;
}
