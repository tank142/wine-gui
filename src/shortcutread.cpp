#include "shortcutread.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <iostream>
using namespace std;
shortcutRead::shortcutRead(QString dir,QString wine,main_target *t,QObject *parent) : QObject(parent)
{
	target = t;
	fixWinePath(dir,wine);
	QString path;
	if(target->storage > 0){
		path = ( get<2>(target->storages.at(target->storage -1)) );
	}else{
		path = ( target->home + "/.wine" );
	}
	fixWinePathDesktop(dir);
}
void shortcutRead::fixWinePath(QString dir,QString wine){
	QFile FILE(dir + "/exec.sh");
	if (FILE.exists()){
		FILE.open(QFile::ReadWrite);
		QTextStream t(&FILE);
		while(!t.atEnd()){
			QString l(t.readLine());
			if(QRegularExpression("^WINE=").match(l).hasMatch()){
				if(QRegularExpression("\"" + wine + "\"").match(l).hasMatch()){
					FILE.close();list.clear();
					break;
				}else{
					list.append("WINE=\"" + wine + "\"");
					while(!t.atEnd()){
						QString L(t.readLine());
						list.append(L);
					}
					FILE.resize(0);
					foreach(QString l,list){
						t.operator<<(l + "\n");
					}
					FILE.close();
					break;
				}
				FILE.close();list.clear();
				break;
			}
			list.append(l);
		}
		FILE.close();
	}
}
void shortcutRead::fixWinePathDesktop(QString dir){
	QFile FILE(dir + "/exec.desktop");
	if (FILE.exists()){
		FILE.open(QFile::ReadWrite);
		QTextStream t(&FILE);
		while(!t.atEnd()){
			QString l(t.readLine());
			if(QRegularExpression("^Path=").match(l).hasMatch()){
				if(QRegularExpression(dir).match(l).hasMatch()){
					FILE.close();list.clear();
					break;
				}else{
					list.append("Path=" + dir);
					while(!t.atEnd()){
						QString L(t.readLine());
						list.append(L);
					}
					FILE.resize(0);
					foreach(QString l,list){
						t.operator<<(l + "\n");
					}
					FILE.close();
					break;
				}
				FILE.close();list.clear();
				break;
			}
			list.append(l);
		}
		FILE.close();
	}
}
