#include "tabsettingsregkey.h"
#include <QHBoxLayout>
#include <QSizePolicy>
tabSettingsRegkey::tabSettingsRegkey(fileRegistry *r,int size,QString b,QString k,QStringList v,QWidget *parent) : QHBoxLayout(parent)
{
	reg = r;branch = b;
	key = new QLabel(k);
	key->setMinimumWidth(size);
	value = new QComboBox();
	value->setEditable(true);
	value->addItem("default",Qt::AlignRight);
	foreach(QString x,v){
		value->addItem(x);
	}
	addWidget(key);
	addWidget(value);
	key->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
	QString regV = reg->value(branch,k);
	if(regV != ""){
		foreach(QString x,v){
			if( x.indexOf(regV) > 0 ){
				value->setCurrentText(regV);
				break;
			}
		}
		bool add = true;
		foreach(QString x,v){
			if( x.indexOf(regV) > -1 ){
				add = false;
				break;
			}
		}
		if(add){
			value->addItem(regV);
		}
		value->setCurrentText(regV);
	}else{
		value->setCurrentIndex(0);
	}
}
tabSettingsRegkey::~tabSettingsRegkey(){
	key->deleteLater();
	value->deleteLater();
}
void tabSettingsRegkey::setValue(){
	if(value->currentIndex() == 0){
		reg->remove(branch,key->text());
	}else{
		reg->setValue(branch,key->text(),value->currentText());
	}
}
void tabSettingsRegkey::reset(){
	if(reg->registryBranchs.contains(branch)){
		QString v = reg->registryBranchs.value(branch)->value(key->text());
		if(v == ""){
			value->setCurrentText("default");
		}else{
			value->setCurrentText(v);
		}
	}
}
