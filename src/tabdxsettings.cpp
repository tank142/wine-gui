#include "tabdxsettings.h"
#include "shelloutput.h"
#include <QRegularExpression>
#include <QHBoxLayout>
tabdxSettings::tabdxSettings(main_target *t)
{
	QHBoxLayout *hbox = new QHBoxLayout;
	QHBoxLayout *hbox2 = new QHBoxLayout;
	target = t;
	// DX9
	dx9wine = new QRadioButton("Wine");
	QFont font = dx9wine->font();
	font.setPointSize(9);dx9wine->setFont(font);
	dx9nine = new QRadioButton("Gallium-nine");dx9nine->setFont(font);
	dx9dxvk = new QRadioButton("DXVK");dx9dxvk->setFont(font);
	QGroupBox *dx9 = new QGroupBox("DirectX9");
	QVBoxLayout *DX9 = new QVBoxLayout;
	dx9->setMaximumHeight(100);
	//dx9nine->setEnabled(false);
	DX9->addWidget(dx9wine);
	DX9->addWidget(dx9nine);
	DX9->addWidget(dx9dxvk);
	dx9->setLayout(DX9);
	hbox->addWidget(dx9);
	//DX11
	dx11wine = new QRadioButton("Wine");dx11wine->setFont(font);
	dx11dxvk = new QRadioButton("DXVK");dx11dxvk->setFont(font);
	QGroupBox *dx11 = new QGroupBox("DirectX11");
	QVBoxLayout *DX11 = new QVBoxLayout;
	dx11->setMaximumHeight(100);
	DX11->addWidget(dx11wine);
	DX11->addWidget(dx11dxvk);
	dx11->setLayout(DX11);
	addLayout(hbox);
	addLayout(hbox2);
	hbox->addWidget(dx11);
	dxgi = new QCheckBox(tr("use_dxgu_dxvk"));
	hbox2->addWidget(dxgi);
	if(QFile(target->NINE + "/lib/wine/x86_64-unix/d3d9-nine.dll.so").exists()){
		nine = target->NINE + "/lib/wine/x86_64-unix/d3d9-nine.dll.so";
		if(QFile(target->NINE + "/lib32/wine/i386-unix/d3d9-nine.dll.so").exists()){
			nine32 = target->NINE + "/lib32/wine/i386-unix/d3d9-nine.dll.so";
		}
	}
	if(nine == ""){
		if(QFile(target->NINE + "/lib/wine/d3d9-nine.dll.so").exists()){
			nine = target->NINE + "/lib/wine/d3d9-nine.dll.so";
			if(QFile(target->NINE + "/lib32/wine/d3d9-nine.dll.so").exists()){
				nine32 = target->NINE + "/lib32/wine/d3d9-nine.dll.so";
			}
		}else{
			if(QFile(target->NINE + "/lib64/d3d9-nine.dll.so").exists()){
				nine = target->NINE + "/lib64/d3d9-nine.dll.so";
			}else{
				if(QFile(target->NINE + "/lib64/d3d9-nine.dll.so").exists()){
					nine32 = target->NINE + "/lib32/d3d9-nine.dll.so";
				}
			}
		}
	}
	if(nine != ""){dx9nine->setEnabled(true);}else{dx9nine->setEnabled(false);}
	if(QFile(target->DXVK + "/x64/d3d9.dll").exists()){
		dx9dxvk->setEnabled(true);
	}else{dx9dxvk->setEnabled(false);}
	if(QFile(target->DXVK + "/x64/d3d11.dll").exists()){
		dx11dxvk->setEnabled(true);
	}else{dx11dxvk->setEnabled(false);}
	if(QFile(target->DXVK + "/x64/dxgi.dll").exists() || QFile(target->DXVK + "/x32/dxgi.dll").exists()){
		dxgi->setEnabled(true);
	}else{dxgi->setEnabled(false);}
	connect(dx9wine, &QRadioButton::clicked , this , &tabdxSettings::setDX9wine);
	connect(dx9nine, &QRadioButton::clicked , this , &tabdxSettings::setDX9nine);
	connect(dx9dxvk, &QRadioButton::clicked , this , &tabdxSettings::setDX9dxvk);
	connect(dx11wine, &QRadioButton::clicked , this , &tabdxSettings::setDX11wine);
	connect(dx11dxvk, &QRadioButton::clicked , this , &tabdxSettings::setDX11dxvk);
	connect(dxgi, &QCheckBox::stateChanged , this , &tabdxSettings::setDXGI);
}
void tabdxSettings::update(){
	disconnect(dx9wine, &QRadioButton::clicked , this , &tabdxSettings::setDX9wine);
	disconnect(dx9nine, &QRadioButton::clicked , this , &tabdxSettings::setDX9nine);
	disconnect(dx9dxvk, &QRadioButton::clicked , this , &tabdxSettings::setDX9dxvk);
	disconnect(dx11wine, &QRadioButton::clicked , this , &tabdxSettings::setDX11wine);
	disconnect(dx11dxvk, &QRadioButton::clicked , this , &tabdxSettings::setDX11dxvk);
	disconnect(dxgi, &QCheckBox::stateChanged , this , &tabdxSettings::setDXGI);
	updateDX9();
	updateDX11();
	updateDXGI();
	check64();
	connect(dx9wine, &QRadioButton::clicked , this , &tabdxSettings::setDX9wine);
	connect(dx9nine, &QRadioButton::clicked , this , &tabdxSettings::setDX9nine);
	connect(dx9dxvk, &QRadioButton::clicked , this , &tabdxSettings::setDX9dxvk);
	connect(dx11wine, &QRadioButton::clicked , this , &tabdxSettings::setDX11wine);
	connect(dx11dxvk, &QRadioButton::clicked , this , &tabdxSettings::setDX11dxvk);
	connect(dxgi, &QCheckBox::stateChanged , this , &tabdxSettings::setDXGI);
}
void tabdxSettings::check64(){
	QFile *reg = new QFile(target->prefix_path + "/user.reg");
	if(reg->exists()){
		reg->open(QFile::ReadOnly);
		QTextStream t(reg);
		QString line = t.readLine();
		QRegularExpression a("^#arch=");//"^#arch=win64"
		while(!t.atEnd()){
			if(a.match(line).hasMatch()){
				if(QRegularExpression("^#arch=win64").match(line).hasMatch()){
					x64 = true;
					break;
				}
				x64 = false;
				break;
			}
			line = t.readLine();
		}
		reg->close();
	}else{
		x64 = true;
	}
	reg->deleteLater();
}
void tabdxSettings::updateDX9(){
	QString link = QFile(target->prefix_path + "/drive_c/windows/system32/d3d9.dll").symLinkTarget();
	if(link != ""){
		if(link.indexOf("d3d9-nine.dll") > -1){
			dx9wine->setChecked(false);
			dx9nine->setChecked(true);
			dx9dxvk->setChecked(false);
		}
		if(link.indexOf(target->DXVK) > -1){
			dx9wine->setChecked(false);
			dx9nine->setChecked(false);
			dx9dxvk->setChecked(true);
		}
	}else{
		dx9wine->setChecked(true);
		dx9nine->setChecked(false);
		dx9dxvk->setChecked(false);
	}
}
void tabdxSettings::updateDX11(){
	QString link = QFile(target->prefix_path + "/drive_c/windows/system32/d3d11.dll").symLinkTarget();
	if(link != ""){
		if(link.indexOf(target->DXVK) > -1){
			dx11wine->setChecked(false);
			dx11dxvk->setChecked(true);
		}
	}else{
		dx11wine->setChecked(true);
		dx11dxvk->setChecked(false);
	}
}
void tabdxSettings::updateDXGI(){
	QString link = QFile(target->prefix_path + "/drive_c/windows/system32/dxgi.dll").symLinkTarget();
	if(link != ""){
		if(link.indexOf(target->DXVK) > -1){
			dxgi->setChecked(true);
		}
	}else{
		dxgi->setChecked(false);
	}
}
void tabdxSettings::old(QString path){
	QFile file(path);
	if(file.exists() && file.symLinkTarget().size() == 0){
		if(QFile(path + ".old").exists()){
			QFile::remove(path + ".old");
		}
		file.rename(path + ".old");
	}else{
		file.remove();
	}
}
void tabdxSettings::setDX9wine(bool enable){
	if(!enable){return;}
	shell *wine = new shell("wineserver",QStringList() << "-k" << "-w");
	wine->envSetup(target);
	wine->exec = "wineserver";
	wine->start();
	wine->wait(-1);
	returnDll wineDll(target);
	if(!dx11dxvk->isChecked()){wineDll.dll("dxgi.dll");}
	wineDll.dll("d3d9.dll");
	if(wineDll.updateNeed){
		shell *wine = new shell("wineboot",QStringList("-u"));
		wine->envSetup(target);
		wine->start();
	}
	fileRegistry reg( target->prefix_path + "/user.reg" ,QStringList()<< "\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]" );
	if(!dx11dxvk->isChecked()){reg.remove("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]","dxgi");}
	reg.remove("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]","d3d9");
	reg.write();
}
void tabdxSettings::setDX11wine(bool enable){
	if(!enable){return;}
	shell *wine = new shell("wineserver",QStringList() << "-k" << "-w");
	wine->envSetup(target);
	wine->exec = "wineserver";
	wine->start();
	wine->wait(-1);

	returnDll wineDll(target);
	if(!dx9dxvk->isChecked()){wineDll.dll("dxgi.dll");}
	wineDll.dll("d3d10.dll");
	wineDll.dll("d3d10core.dll");
	wineDll.dll("d3d10_1.dll");
	wineDll.dll("d3d11.dll");
	if(wineDll.updateNeed){
		shell *wine = new shell("wineboot",QStringList("-u"));
		wine->envSetup(target);
		wine->start();
	}
	fileRegistry reg( target->prefix_path + "/user.reg" ,QStringList()<< "\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]" );
	if(!dx9dxvk->isChecked()){reg.remove("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]","dxgi");}
	reg.remove("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]","d3d10");
	reg.remove("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]","d3d10core");
	reg.remove("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]","d3d10_1");
	reg.remove("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]","d3d11");
	reg.write();
}
QString tabdxSettings::winedirBit(QString dir,QString bit){
	if(QDir(dir + "/lib" + bit).exists()){
		return dir + "/lib/wine/" + bit;
	}
	if(QDir(dir + "/lib").exists()){
		return dir + "/lib/wine/";
	}
	return "";
}
void tabdxSettings::setDX9nine(bool enable){
	if(!enable){return;}
	shell *wine = new shell("wineserver",QStringList() << "-k" << "-w");
	wine->envSetup(target);
	wine->exec = "wineserver";
	wine->start();
	wine->wait(-1);
	old(target->prefix_path + "/drive_c/windows/system32/d3d9.dll");
	if(!dx11dxvk->isChecked()){
		returnDll wineDll(target);
		wineDll.dll("dxgi.dll");
		if(wineDll.updateNeed){
			shell *wine = new shell("wineboot",QStringList("-u"));
			wine->envSetup(target);
			wine->start();
		}
	}
	if(x64){
		QFile::link(nine,target->prefix_path + "/drive_c/windows/system32/d3d9.dll");
		if(QDir(target->prefix_path + "/drive_c/windows/syswow64").exists()){
			old(target->prefix_path + "/drive_c/windows/syswow64/d3d9.dll");
			QFile::link(nine32,target->prefix_path + "/drive_c/windows/syswow64/d3d9.dll");
		}
	}else{
		QFile::link(nine32,target->prefix_path + "/drive_c/windows/system32/d3d9.dll");
	}
	fileRegistry reg( target->prefix_path + "/user.reg" ,QStringList()<< "\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]" );
	if(!dx11dxvk->isChecked()){reg.remove("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]","dxgi");}
	reg.setValue("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]","d3d9","native");
	reg.write();
}
void tabdxSettings::setDX9dxvk(bool enable){
	if(!enable){return;}
	shell *wine = new shell("wineserver",QStringList() << "-k" << "-w");
	wine->envSetup(target);
	wine->exec = "wineserver";
	wine->start();
	wine->wait(-1);
	fileRegistry reg( target->prefix_path + "/user.reg" ,QStringList()<< "\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]" );
	if(x64){
		if(dxgi->isChecked()){
			reg.setValue("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]","dxgi","native");
			old(target->prefix_path + "/drive_c/windows/system32/dxgi.dll");
			QFile::link(target->DXVK + "/x64/dxgi.dll",target->prefix_path + "/drive_c/windows/system32/dxgi.dll");
			if(QDir(target->prefix_path + "/drive_c/windows/syswow64").exists()){
				old(target->prefix_path + "/drive_c/windows/syswow64/dxgi.dll");
				QFile::link(target->DXVK + "/x32/dxgi.dll",target->prefix_path + "/drive_c/windows/syswow64/dxgi.dll");
			}
		}
		old(target->prefix_path + "/drive_c/windows/system32/d3d9.dll");
		QFile::link(target->DXVK + "/x64/d3d9.dll",target->prefix_path + "/drive_c/windows/system32/d3d9.dll");
		if(QDir(target->prefix_path + "/drive_c/windows/syswow64").exists()){
			old(target->prefix_path + "/drive_c/windows/syswow64/d3d9.dll");
			QFile::link(target->DXVK + "/x32/d3d9.dll",target->prefix_path + "/drive_c/windows/syswow64/d3d9.dll");
		}
	}else{
		if(dxgi->isChecked()){
			reg.setValue("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]","dxgi","native");
			old(target->prefix_path + "/drive_c/windows/system32/dxgi.dll");
			QFile::link(target->DXVK + "/x32/dxgi.dll",target->prefix_path + "/drive_c/windows/system32/dxgi.dll");
		}
		old(target->prefix_path + "/drive_c/windows/system32/d3d9.dll");
		QFile::link(target->DXVK + "/x32/d3d9.dll",target->prefix_path + "/drive_c/windows/system32/d3d9.dll");
	}
	reg.setValue("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]","d3d9","native");
	reg.write();
}
void tabdxSettings::setDX11dxvk(bool enable){
	shell *wine = new shell("wineserver",QStringList() << "-k" << "-w");
	wine->envSetup(target);
	wine->exec = "wineserver";
	wine->start();
	wine->wait(-1);
	if(!enable){return;}
	fileRegistry reg( target->prefix_path + "/user.reg" ,QStringList()<< "\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]"  );
	if(x64){
		QDir dxvk64(target->DXVK + "/x64");dxvk64.setFilter(QDir::NoDotAndDotDot | QDir::Files);
		foreach(QString file,dxvk64.entryList()){
			if(file != "d3d9.dll"){
				if(!dxgi->isChecked()){if(file == "dxgi.dll"){continue;}}
				old(target->prefix_path + "/drive_c/windows/system32/" + file);
				QFile::link(target->DXVK + "/x64/" + file,target->prefix_path + "/drive_c/windows/system32/" + file);
				reg.setValue("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]",file.remove(file.size() - 4,4),"native");
			}
		}
		if(QDir(target->prefix_path + "/drive_c/windows/syswow64").exists()){
			QDir dxvk32(target->DXVK + "/x32");dxvk32.setFilter(QDir::NoDotAndDotDot | QDir::Files);
			foreach(QString file,dxvk32.entryList()){
				if(file != "d3d9.dll"){
					if(!dxgi->isChecked()){if(file == "dxgi.dll"){continue;}}
					old(target->prefix_path + "/drive_c/windows/syswow64/" + file);
					QFile::link(target->DXVK + "/x32/" + file,target->prefix_path + "/drive_c/windows/syswow64/" + file);
				}
			}
		}
	}else{
		QDir dxvk32(target->DXVK + "/x32");dxvk32.setFilter(QDir::NoDotAndDotDot | QDir::Files);
		foreach(QString file,dxvk32.entryList()){
			if(file != "d3d9.dll"){
				old(target->prefix_path + "/drive_c/windows/system32/" + file);
				QFile::link(target->DXVK + "/x32/" + file,target->prefix_path + "/drive_c/windows/system32/" + file);
				reg.setValue("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]",file.remove(file.size() - 4,4),"native");
			}
		}
	}
	reg.write();
}
void tabdxSettings::setDXGI(bool enable){
	shell *wine = new shell("wineserver",QStringList() << "-k" << "-w");
	wine->envSetup(target);
	wine->exec = "wineserver";
	wine->start();
	wine->wait(-1);
	fileRegistry reg( target->prefix_path + "/user.reg" ,QStringList()<< "\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]" );
	if(enable){
		if(dx11dxvk->isChecked() || dx9dxvk->isChecked()){
			if(x64){
				old(target->prefix_path + "/drive_c/windows/system32/dxgi.dll");
				QFile::link(target->DXVK + "/x64/dxgi.dll",target->prefix_path + "/drive_c/windows/system32/dxgi.dll");
				if(QDir(target->prefix_path + "/drive_c/windows/syswow64").exists()){
					old(target->prefix_path + "/drive_c/windows/syswow64/dxgi.dll");
					QFile::link(target->DXVK + "/x32/dxgi.dll",target->prefix_path + "/drive_c/windows/syswow64/dxgi.dll");
				}
			}else{
				old(target->prefix_path + "/drive_c/windows/system32/dxgi.dll");
				QFile::link(target->DXVK + "/x32/dxgi.dll",target->prefix_path + "/drive_c/windows/system32/dxgi.dll");
			}
			reg.setValue("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]","dxgi","native");
			reg.write();
		}
	}else{
		reg.remove("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]","dxgi");
		reg.write();
		returnDll wineDll(target);
		wineDll.dll("dxgi.dll");
		if(wineDll.updateNeed){
			shell *wine = new shell("wineboot",QStringList("-u"));
			wine->envSetup(target);
			wine->start();
		}
	}
}

returnDll::returnDll(main_target *t,QObject *parent) : QObject(parent) {
	target = t;
	updateNeed = false;
	QFile *reg = new QFile(target->prefix_path + "/user.reg");
	if(reg->exists()){
		reg->open(QFile::ReadOnly);
		QTextStream t(reg);
		QString line = t.readLine();
		QRegularExpression a("^#arch=");
		while(!t.atEnd()){
			if(a.match(line).hasMatch()){
				if(QRegularExpression("^#arch=win64").match(line).hasMatch()){
					x64 = true;break;
				}
				x64 = false;break;
			}
			line = t.readLine();
		}
		reg->close();
	}else{
		x64 = true;
	}
	reg->deleteLater();
	if(target->prefix_wine == "System" || target->prefix_wine.size() == 0 ){
		foreach(QString f,QStringList() << "/usr/lib64/wine/fakedlls/" << "/usr/lib/x86_64-linux-gnu/wine/fakedlls/"){
			if(QDir(f).exists()){
				dll64 = f;break;
			}
		}
		if(QDir("/usr/lib32/wine/fakedlls/").exists()){
			dll32 = "/usr/lib32/wine/fakedlls/";
		}
	}else{
		if(QDir(target->WINE_VER + "/" + target->prefix_wine).exists()){
			foreach(QString f,QStringList() << "/lib64" << "/lib"){
				if(QDir(target->WINE_VER + "/" + target->prefix_wine + f).exists()){
					dll64 = f;break;
				}
			}
			foreach(QString f,QStringList() << "/lib32" << "/lib"){
				if(dll64 != f){
					if(QDir(target->WINE_VER + "/" + target->prefix_wine + f).exists()){
						dll32 = target->WINE_VER + "/" + target->prefix_wine + f;break;
					}
				}
			}
			dll64 = target->WINE_VER + "/" + target->prefix_wine + dll64;
		}
	}
}

void returnDll::dll(QString lib){
	if(x64){
		if(!old(target->prefix_path + "/drive_c/windows/system32/" + lib)){
			if(dll64.size() > 0){
				QFile lib64(dll64 + "/wine/fakedlls/" + lib);
				if(!lib64.exists()){lib64.setFileName(dll64 + "/wine/" + lib);}
				if(!lib64.copy(target->prefix_path + "/drive_c/windows/system32/" + lib)){
					updateNeed = true;
				}
			}
		}
		if(QDir(target->prefix_path + "/drive_c/windows/syswow64").exists()){
			if(!old(target->prefix_path + "/drive_c/windows/syswow64/" + lib)){
				if(dll32.size() > 0){
					QFile lib32(dll32 + "/wine/fakedlls/" + lib);
					if(!lib32.exists()){lib32.setFileName(dll64 + "/wine/" + lib);}
					if(!lib32.copy(target->prefix_path + "/drive_c/windows/syswow64/" + lib)){
						updateNeed = true;
					}
				}
				updateNeed = true;
			}
		}
	}else{
		if(!old(target->prefix_path + "/drive_c/windows/system32/" + lib)){
			if(dll32.size() > 0){
				QFile lib32(dll32 + "/wine/fakedlls/" + lib);
				if(!lib32.exists()){lib32.setFileName(dll64 + "/wine/" + lib);}
				if(!lib32.copy(target->prefix_path + "/drive_c/windows/system32/" + lib)){
					updateNeed = true;
				}
			}
		}
	}
}
bool returnDll::old(QString path){
	QFile file(path);QFile old(path + ".old");
	if(file.exists()){
		if(file.symLinkTarget().size() == 0){
			if(old.exists()){
				file.remove();
				if(!old.rename(path)){return false;}
			}
			return true;
		}
	}
	file.remove();
	if(old.exists()){
		if(old.rename(path)){return true;}
	}
	return false;
}
