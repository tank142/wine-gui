#include "tabdxsettings.h"
#include "shelloutput.h"
#include <iostream>
using namespace std;
tabdxSettings::tabdxSettings(main_target *t)
{
	target = t;
	// DX9
	dx9wine = new QRadioButton(tr("Wine"));
	QFont font = dx9wine->font();
	font.setPointSize(9);dx9wine->setFont(font);
	dx9nine = new QRadioButton(tr("Gallium-nine"));dx9nine->setFont(font);
	dx9dxvk = new QRadioButton(tr("DXVK"));dx9dxvk->setFont(font);
	QGroupBox *dx9 = new QGroupBox(tr("DirectX9"));
	QVBoxLayout *DX9 = new QVBoxLayout;
	dx9->setMaximumHeight(100);
	//dx9nine->setEnabled(false);
	DX9->addWidget(dx9wine);
	DX9->addWidget(dx9nine);
	DX9->addWidget(dx9dxvk);
	dx9->setLayout(DX9);
	addWidget(dx9);
	//DX11
	dx11wine = new QRadioButton(tr("Wine"));dx11wine->setFont(font);
	dx11dxvk = new QRadioButton(tr("DXVK"));dx11dxvk->setFont(font);
	QGroupBox *dx11 = new QGroupBox(tr("DirectX11"));
	QVBoxLayout *DX11 = new QVBoxLayout;
	dx11->setMaximumHeight(100);
	DX11->addWidget(dx11wine);
	DX11->addWidget(dx11dxvk);
	dx11->setLayout(DX11);
	addWidget(dx11);
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
	if(nine != ""){dx9nine->setEnabled(true);}else{dx9nine->setEnabled(false);}
	if(QFile(target->DXVK + "/x64/d3d9.dll").exists()){
		dx9dxvk->setEnabled(true);
	}else{dx9dxvk->setEnabled(false);}
	if(QFile(target->DXVK + "/x64/d3d11.dll").exists()){
		dx11dxvk->setEnabled(true);
	}else{dx11dxvk->setEnabled(false);}
	connect(dx9wine, &QRadioButton::clicked , this , &tabdxSettings::setDX9wine);
	connect(dx9nine, &QRadioButton::clicked , this , &tabdxSettings::setDX9nine);
	connect(dx9dxvk, &QRadioButton::clicked , this , &tabdxSettings::setDX9dxvk);
	connect(dx11wine, &QRadioButton::clicked , this , &tabdxSettings::setDX11wine);
	connect(dx11dxvk, &QRadioButton::clicked , this , &tabdxSettings::setDX11dxvk);
}
void tabdxSettings::update(){
	updateDX9();
	updateDX11();
	check64();
}
void tabdxSettings::check64(){
	QFile *reg = new QFile(target->prefix_path + "/user.reg");
	if(reg->exists()){
		reg->open(QFile::ReadOnly);
		QTextStream t(reg);
		QString line = t.readLine();
		QRegExp a("^#arch=win64");
		QRegExp b("^#arch=win32");
		while(!t.atEnd()){
			if(a.indexIn(line) > -1){
				x64 = true;
				break;
			}
			if(b.indexIn(line) > -1){
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
void tabdxSettings::setDX9wine(bool enable){
	if(!enable){return;}
	shell *wine = new shell("wineserver",QStringList("-k"));
	wine->envSetup(target);
	wine->exec = "wineserver";
	wine->start();
	wine->wait(-1);
	bool *winebootUpdate = new bool; *winebootUpdate = false;
	wineDll("d3d9.dll",winebootUpdate);
	if(*winebootUpdate){
		shell *wine = new shell("wineboot",QStringList("-u"));
		wine->envSetup(target);
		wine->start();
	}
	delete winebootUpdate;
	fileRegistry reg( target->prefix_path + "/user.reg" ,QStringList()<< "\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]" );
	reg.remove("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]","d3d9");
	reg.write();
}
void tabdxSettings::setDX11wine(bool enable){
	if(!enable){return;}
	shell *wine = new shell("wineserver",QStringList("-k"));
	wine->envSetup(target);
	wine->exec = "wineserver";
	wine->start();
	wine->wait(-1);

	bool *winebootUpdate = new bool; *winebootUpdate = false;
	wineDll("dxgi.dll",winebootUpdate);
	wineDll("d3d10.dll",winebootUpdate);
	wineDll("d3d10core.dll",winebootUpdate);
	wineDll("d3d10_1.dll",winebootUpdate);
	wineDll("d3d11.dll",winebootUpdate);
	if(*winebootUpdate){
		shell *wine = new shell("wineboot",QStringList("-u"));
		wine->envSetup(target);
		wine->start();
	}
	fileRegistry reg( target->prefix_path + "/user.reg" ,QStringList()<< "\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]" );
	reg.remove("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]","dxgi");
	reg.remove("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]","d3d10");
	reg.remove("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]","d3d10core");
	reg.remove("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]","d3d10_1");
	reg.remove("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]","d3d11");
	reg.write();
	delete winebootUpdate;
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
void tabdxSettings::wineDll(QString lib, bool *winebootUpdate){
	if(x64){
		QFile(target->prefix_path + "/drive_c/windows/system32/" + lib).remove();
		QFile old(target->prefix_path + "/drive_c/windows/system32/" + lib + ".old");
		if(old.exists() && old.symLinkTarget() == ""){
			old.rename(target->prefix_path + "/drive_c/windows/system32/" + lib);
		}else{
			if(target->prefix_wine == "System"){
				if(QFile("/usr/lib64/wine/" + lib).exists()){
					QFile("/usr/lib64/wine/" + lib).copy(target->prefix_path + "/drive_c/windows/system32/" + lib);
				}else{
					*winebootUpdate = true;
				}
			}else{
				QString lib64 = winedirBit(target->WINE_VER + "/" + target->prefix_wine, "64");
				QFile(lib64 + lib).copy(target->prefix_path + "/drive_c/windows/system32/" + lib);
			}
		}
		if(QDir(target->prefix_path + "/drive_c/windows/syswow64").exists()){
			QFile(target->prefix_path + "/drive_c/windows/syswow64/" + lib).remove();
			QFile old(target->prefix_path + "/drive_c/windows/syswow64/" + lib + ".old");
			if(old.exists() && old.symLinkTarget() == ""){
				QFile(target->prefix_path + "/drive_c/windows/syswow64/" + lib).remove();
				old.rename(target->prefix_path + "/drive_c/windows/syswow64/" + lib);
			}else{
				QFile(target->prefix_path + "/drive_c/windows/syswow64/" + lib).remove();
				if(target->prefix_wine == "System"){
					if(QFile("/usr/lib32/wine/" + lib).exists()){
						QFile("/usr/lib32/wine/" + lib).copy(target->prefix_path + "/drive_c/windows/syswow64/" + lib);
					}else{
						*winebootUpdate = true;
					}
				}else{
					QString lib32 = winedirBit(target->WINE_VER + "/" + target->prefix_wine, "32");
					QFile(lib32 + lib).copy(target->prefix_path + "/drive_c/windows/syswow64/" + lib);
				}
			}
		}
	}else{
		QFile(target->prefix_path + "/drive_c/windows/system32/" + lib).remove();
		QFile old(target->prefix_path + "/drive_c/windows/system32/" + lib + ".old");
		if(old.exists() && old.symLinkTarget() == ""){
			old.rename(target->prefix_path + "/drive_c/windows/system32/" + lib);
		}else{
			if(target->prefix_wine == "System"){
				if(QFile("/usr/lib32/wine/" + lib).exists()){
					QFile("/usr/lib32/wine/" + lib).copy(target->prefix_path + "/drive_c/windows/system32/" + lib);
				}else{
					*winebootUpdate = true;
				}
			}else{
				QString lib32 = winedirBit(target->WINE_VER + "/" + target->prefix_wine, "32");
				QFile(lib32 + lib).copy(target->prefix_path + "/drive_c/windows/system32/" + lib);
			}
		}
	}
}
void tabdxSettings::old(QString path){
	QFile file(path);
	if(file.exists() && file.symLinkTarget() == ""){
		if(QFile(path + ".old").exists()){
			QFile::remove(path + ".old");
		}
		file.rename(path + ".old");
	}else{
		file.remove();
	}
}
void tabdxSettings::setDX9nine(bool enable){
	if(!enable){return;}
	shell *wine = new shell("wineserver",QStringList("-k"));
	wine->envSetup(target);
	wine->exec = "wineserver";
	wine->start();
	wine->wait(-1);
	old(target->prefix_path + "/drive_c/windows/system32/d3d9.dll");
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
	reg.setValue("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]","d3d9","native");
	reg.write();
}
void tabdxSettings::setDX9dxvk(bool enable){
	if(!enable){return;}
	shell *wine = new shell("wineserver",QStringList("-k"));
	wine->envSetup(target);
	wine->exec = "wineserver";
	wine->start();
	wine->wait(-1);
	if(x64){
		old(target->prefix_path + "/drive_c/windows/system32/d3d9.dll");
		QFile::link(target->DXVK + "/x64/d3d9.dll",target->prefix_path + "/drive_c/windows/system32/d3d9.dll");
		if(QDir(target->prefix_path + "/drive_c/windows/syswow64").exists()){
			old(target->prefix_path + "/drive_c/windows/syswow64/d3d9.dll");
			if(QFile(target->prefix_path + "/drive_c/windows/syswow64/d3d9.dll").exists() &&
					QFile(target->prefix_path + "/drive_c/windows/syswow64/d3d9.dll").symLinkTarget() == ""){
				if(QFile(target->prefix_path + "/drive_c/windows/syswow64/d3d9.dll.old").exists()){
					QFile::remove(target->prefix_path + "/drive_c/windows/syswow64/d3d9.dll.old");
				}
				QFile::rename(target->prefix_path + "/drive_c/windows/syswow64/d3d9.dll",target->prefix_path + "/drive_c/windows/syswow64/d3d9.dll.old");
			}else{
				QFile::remove(target->prefix_path + "/drive_c/windows/syswow64/d3d9.dll");
			}
			QFile::link(target->DXVK + "/x32/d3d9.dll",target->prefix_path + "/drive_c/windows/syswow64/d3d9.dll");
		}
	}else{
		old(target->prefix_path + "/drive_c/windows/system32/d3d9.dll");
		QFile::link(target->DXVK + "/x32/d3d9.dll",target->prefix_path + "/drive_c/windows/system32/d3d9.dll");
	}
	fileRegistry reg( target->prefix_path + "/user.reg" ,QStringList()<< "\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]" );
	reg.setValue("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]","d3d9","native");
	reg.write();
}
void tabdxSettings::setDX11dxvk(bool enable){
	shell *wine = new shell("wineserver",QStringList("-k"));
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
				old(target->prefix_path + "/drive_c/windows/system32/" + file);
				QFile::link(target->DXVK + "/x64/" + file,target->prefix_path + "/drive_c/windows/system32/" + file);
				reg.setValue("\\[Software\\\\\\\\Wine\\\\\\\\DllOverrides\\]",file.remove(file.size() - 4,4),"native");
			}
		}
		if(QDir(target->prefix_path + "/drive_c/windows/syswow64").exists()){
			QDir dxvk32(target->DXVK + "/x32");dxvk32.setFilter(QDir::NoDotAndDotDot | QDir::Files);
			foreach(QString file,dxvk32.entryList()){
				if(file != "d3d9.dll"){
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
