#include <QStringList>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QByteArray>
#include <QStandardItem>
#include <QDir>
#include <QSettings>
#include <QComboBox>
#include <QIcon>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QEventLoop>
#include <QObject>
#include <QApplication>
#include <QCoreApplication>
#include <QtWidgets>
#include <QObject>
QString wine_ver(QString d){
	if(QFile::exists(d + "/proton") || QFile::exists(d + "/dist")){
		return QFileInfo(d).fileName();}
	if(QFile::exists(d + "/bin/wine")){
		return QString(QProcess::startDetached(d + "/bin/wine",QStringList() << "--version"));
	}
	return "Error";
}
QString wine_ver(){
	if(QFile::exists("/bin/wine")){
		QProcess *exec = new QProcess();
		exec->start("/bin/wine",QStringList() << "--version");
		exec->waitForFinished();
		QString output(exec->readAllStandardOutput());
		exec->~QProcess();
		output.remove("\n");
		return output;
	}
	return "Error";
}
