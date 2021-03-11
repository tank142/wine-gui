#include "fileregistry.h"
#include <QRegularExpression>
#include <QMap>
#include <iostream>
using namespace std;
fileRegistry::fileRegistry(QString reg,QStringList branchs,QObject *parent) : QObject(parent)
{
	foreach(QString b,branchs){
		registryBranchs.insert(b, new QMap<QString, QString>);
	}
	FILE = new QFile(reg,this);
	if (FILE->exists()){
		FILE->open(QFile::ReadOnly);
		QTextStream t(FILE);
		QString line = t.readLine();
		bool br;
		while(!t.atEnd()){
			br = false;
			for (QMap<QString, QMap<QString, QString>*>::iterator I = registryBranchs.begin(); I != registryBranchs.end();I++){

				if(QRegularExpression(I.key()).match(line).hasMatch()){
					line = t.readLine();
					while(!t.atEnd() && QRegularExpression("^\\[").match(line).hasMatch()){
						if(QRegularExpression("\"").match(line).hasMatch()){
							QString keyL = line;
							keyL.remove(QRegularExpression("\""));
							keyL.remove(keyL.indexOf("\""),keyL.size() - 1);
							QString keyX = line;
							keyX.remove(0,keyX.indexOf("=") + 1);
							keyX.remove(QRegularExpression("^\""));
							keyX.remove(QRegularExpression("\"$"));
							I.value()->insert(keyL,keyX);
						}
						line = t.readLine();
					}
					br = true;break;
				}
			}
			if(!br){line = t.readLine();}
		}
		FILE->close();
	}
}
fileRegistry::~fileRegistry(){
	for (QMap<QString, QMap<QString, QString>*>::iterator I = registryBranchs.begin(); I != registryBranchs.end();I++){
		 I.value()->~QMap();
	}
}
QString fileRegistry::value(QString branch, QString key){
	for (QMap<QString, QMap<QString, QString>*>::iterator I = registryBranchs.begin(); I != registryBranchs.end();I++){
		if(I.key() == branch){
			if(I.value()->contains(key)){
				return I.value()->value(key);
			}
		}
	}
	return "";
}
void fileRegistry::setValue(QString branch,QString key,QString value){
	if(!registryBranchs.contains(branch)){
		registryBranchs.insert(branch, new QMap<QString, QString>);
	}
	registryBranchs.value(branch)->insert(key,value);
}
void fileRegistry::remove(QString branch,QString key){
	if(!registryBranchs.count(branch)){return;}
	registryBranchs.value(branch)->remove(key);
}
void fileRegistry::write(){
	FILE->open(QFile::ReadWrite);
	QTextStream t(FILE);
	QStringList list;
	QString line = t.readLine();
	bool br;
	while(!t.atEnd()){
		br = false;
		for (QMap<QString, QMap<QString, QString>*>::iterator I = registryBranchs.begin(); I != registryBranchs.end();I++){
			if(QRegularExpression(I.key()).match(line).hasMatch()){
				line = t.readLine();
				while(!t.atEnd() && !QRegularExpression("^\\[").match(line).hasMatch()){
					line = t.readLine();
				}
				br = true;break;
			}
		}
		if(!br){
			list.append(line);
			line = t.readLine();
		}
	}
	for (QMap<QString, QMap<QString, QString>*>::iterator I = registryBranchs.begin(); I != registryBranchs.end();I++){
		if(I.value()->size() > 0){
			QString branch = I.key();
			branch.remove(0,1);
			branch.remove(branch.size() -2,1);
			branch.replace("\\\\","\\");
			list.append(branch);
			for (QMap<QString, QString>::iterator i = I.value()->begin(); i != I.value()->end();i++){
				list.append("\"" + i.key() + "\"=\"" + i.value() + "\"");
			}
		}
	}
	FILE->resize(0);
	foreach(QString l,list){
		t.operator<<(l + "\n");
	}
	t.operator<<("\n");
	FILE->close();
}
