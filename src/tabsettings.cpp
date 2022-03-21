#include <QLabel>
#include "tabsettings.h"
#include "shell.h"
tabSettings::tabSettings(main_target *t,QWidget *parent) : QWidget(parent)
{
	target = t;
	QVBoxLayout *MAIN = new QVBoxLayout;
	DX = new tabdxSettings(target);
	SYNC = new tabsyncSettings(target);
	QGroupBox *regedit = new QGroupBox(tr("regedit"));
	regedit->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
	REGEDIT = new QVBoxLayout;REGEDIT->setAlignment(Qt::AlignTop);
	Direct3D_l = new QVBoxLayout;REGEDIT->setAlignment(Qt::AlignTop);
	regedit->setLayout(REGEDIT);
	MAIN->addLayout(DX);
	MAIN->addWidget(SYNC);
	MAIN->addWidget(regedit);
	setLayout(MAIN);
	QGroupBox *Direct3D = new QGroupBox("Direct3D");
	Direct3D->setLayout(Direct3D_l);
	REGEDIT->addWidget(Direct3D);
	QGroupBox *DirectInput = new QGroupBox("DirectInput");
	DirectInput_l = new QVBoxLayout;DirectInput_l->setAlignment(Qt::AlignTop);
	DirectInput->setLayout(DirectInput_l);
	REGEDIT->addWidget(DirectInput);
	QHBoxLayout *b = new QHBoxLayout;
	cancel = new QPushButton(tr("cancel"));cancel->setHidden(true);
	ok = new QPushButton(tr("ok"));ok->setHidden(true);
	b->addWidget(cancel);b->addWidget(ok);
	MAIN->addLayout(b);
	connect(cancel,	&QPushButton::clicked , this , &tabSettings::cancel_slot);
	connect(ok,		&QPushButton::clicked , this , &tabSettings::ok_slot);
}
QVector<tabSettingsRegkey*> tabSettings::regSearch(QString branch,int size,fileRegistry *reg,QVector<tabSettingsRegkey*> regs, QStringList l ){
	QMap<QString, QString>::iterator r = reg->registryBranchs.value(branch)->end();
	for (QMap<QString, QString>::iterator i = reg->registryBranchs.value(branch)->begin();i != r;i++){
		bool find = false;
		foreach(tabSettingsRegkey *x,regs){
			if(x->key->text() == i.key()){
				find = true;
				break;
			}
		}
		if(find){break;}else{
			regs.append(new tabSettingsRegkey(reg,size,branch,i.key(),l));
		}
	}
	return regs;
}
void tabSettings::updateReg(fileRegistry *reg){
	foreach(tabSettingsRegkey *w,regWidgets){
		w->deleteLater();
	}
	regWidgets.clear();QVector<tabSettingsRegkey*> regs;
	regs.append(new tabSettingsRegkey(reg,160,"\\[Software\\\\\\\\Wine\\\\\\\\Direct3D\\]","renderer",
									  QStringList() << "gdi" << "gl" << "vulkan" ));
	regs.append(new tabSettingsRegkey(reg,160,"\\[Software\\\\\\\\Wine\\\\\\\\Direct3D\\]","shader_backend",
									  QStringList() << "glsl" << "arb" << "none" ));
	regs.append(new tabSettingsRegkey(reg,160,"\\[Software\\\\\\\\Wine\\\\\\\\Direct3D\\]","VideoMemorySize",
									  QStringList() << "32" << "64" << "128" << "256" << "512" << "1024" << "2048"
										 << "3072" << "4096" << "8192" << "11264"  << "12288" << "16384" << "24576" ));
	regs.append(new tabSettingsRegkey(reg,160,"\\[Software\\\\\\\\Wine\\\\\\\\Direct3D\\]","OffscreenRenderingMode",
									  QStringList() << "backbuffer" << "fbo" ));
	regs.append(new tabSettingsRegkey(reg,160,"\\[Software\\\\\\\\Wine\\\\\\\\Direct3D\\]","MultisampleTextures",
									  QStringList() << "0x1" << "0x0" ));
	foreach(tabSettingsRegkey *r,regSearch("\\[Software\\\\\\\\Wine\\\\\\\\Direct3D\\]",160,reg,regs,QStringList())){
		regWidgets.append(r);Direct3D_l->addLayout(r);
		connect(r->value,	&QComboBox::currentIndexChanged  , this , &tabSettings::cancelOkEnable);
	}
	regs.clear();
	regs.append(new tabSettingsRegkey(reg,160,"\\[Software\\\\\\\\Wine\\\\\\\\DirectInput\\]","MouseWarpOverride",
									  QStringList() << "enable" << "disable" << "force" ));
	foreach(tabSettingsRegkey *r,regSearch("\\[Software\\\\\\\\Wine\\\\\\\\DirectInput\\]",160,reg,regs,QStringList())){
		regWidgets.append(r);DirectInput_l->addLayout(r);
		connect(r->value,	&QComboBox::currentIndexChanged  , this , &tabSettings::cancelOkEnable);
	}
}
void tabSettings::cancel_slot(){
	cancel->setHidden(true);ok->setHidden(true);
	foreach(tabSettingsRegkey *r,regWidgets){r->reset();}
}
void tabSettings::ok_slot(){
	cancel->setHidden(true);ok->setHidden(true);
	shell *wine = new shell("wineserver",QStringList() << "-k" << "-w");
	wine->envSetup(target);
	wine->exec = "wineserver";
	wine->start();
	wine->wait(-1);
	foreach(tabSettingsRegkey *r,regWidgets){r->setValue();}
	emit write();
}
void tabSettings::cancelOkEnable(){
	cancel->setHidden(false);ok->setHidden(false);
}

