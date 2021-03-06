#include "dirwork.h"
#include <filesystem>
using namespace std;
void dirWork::dirRecurse(QString d) {
	QDir dir(d);dir.setSorting(QDir::Name);dir.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::System | QDir::Hidden | QDir::AllEntries);
	foreach(QString p,dir.entryList()){
		QFileInfo finfo(QString( d + "/" + p ));
		if (finfo.isSymLink()){
			if(dir_source.relativeFilePath(d) == "."){
				files.append(tuple("",p,0));
			}else{
				files.append(tuple(dir_source.relativeFilePath(d),p,0));
			}
			continue;
		}
		if (finfo.isDir()) {
			dirRecurse( d + "/" + p );
		}
		if(dir_source.relativeFilePath(d) == "."){
			files.append(tuple("",p,finfo.size()));
		}else{
			files.append(tuple(dir_source.relativeFilePath(d),p,finfo.size()));
		}
		sizeAll = sizeAll + finfo.size();
		if(close_t){
			break;
		}
	}
}

void dirWork::copyTarget() {
	quint64 size = 0;
	for(int i = 0;files.size() > i;i++){
		if(!close_t){
			if( !QFileInfo(dir_target + "/" + get<0>(files.at(i))).isDir() ){
				if(!QDir().mkpath(dir_target + "/" + get<0>(files.at(i)))){
					emit msg(tr("err_create_dir") + dir_target + "/" + get<0>(files.at(i)));
					close_t = true;continue;
				}
			}
			if(QFileInfo(dir_source.path() + "/" + get<0>(files.at(i)) + "/" + get<1>(files.at(i))).isSymLink()){
				filesystem::copy_symlink(filesystem::path(QString(dir_source.path() + "/" + get<0>(files.at(i)) + "/" + get<1>(files.at(i))).toStdString()),
										 filesystem::path(QString(dir_target + "/" + get<0>(files.at(i)) + "/" + get<1>(files.at(i))).toStdString()));
				if(!QFileInfo(dir_target + "/" + get<0>(files.at(i)) + "/" + get<1>(files.at(i))).isSymLink()){
					emit msg(tr("err_create_ln") +  dir_source.path() + "/" + get<0>(files.at(i)) + "/" + get<1>(files.at(i)) + " >> "
												+ dir_target + "/" + get<0>(files.at(i)) + "/" + get<1>(files.at(i)));
					close_t = true;continue;
				}else{continue;}
			}
			if(QFileInfo(dir_source.path() + "/" + get<0>(files.at(i)) + "/" + get<1>(files.at(i))).isDir()){
				if(!QFileInfo(dir_target + "/" + get<0>(files.at(i)) + "/" + get<1>(files.at(i))).isDir()){
					if(!QDir().mkpath(dir_target + "/" + get<0>(files.at(i)) + "/" + get<1>(files.at(i)))){
						emit msg(tr("err_create_dir") + dir_target + "/" + get<0>(files.at(i)) + "/" + get<1>(files.at(i)));
						close_t = true;continue;
					}
				}
				continue;
			}
			filesystem::copy_file(filesystem::path(QString(dir_source.path() + "/" + get<0>(files.at(i)) + "/" + get<1>(files.at(i))).toStdString()),
									 filesystem::path(QString(dir_target + "/" + get<0>(files.at(i)) + "/" + get<1>(files.at(i))).toStdString()));
			if(!QFileInfo(dir_target + "/" + get<0>(files.at(i)) + "/" + get<1>(files.at(i))).isFile())
			{
				emit msg(tr("err_create_file") + dir_source.path() + "/" + get<0>(files.at(i)) + "/" + get<1>(files.at(i)) + " >> "
													+ dir_target + "/" + get<0>(files.at(i)) + "/" + get<1>(files.at(i)));
				close_t = true;continue;
			}
			size = size + get<2>(files.at(i));
			emit update_bar(size);
		}else{
			if( QFileInfo(dir_target).isDir() ){
				emit msg(tr("cleaning_files"));
				QDir(dir_target).removeRecursively();
			}
			break;
		}
	}
}
