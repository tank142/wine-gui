#ifndef SHELL_H
#define SHELL_H

#include <main_target.h>
#include <QThread>
#include <QObject>
#include <QProcess>
#include <QProcessEnvironment>

class shell : public QThread
{
	Q_OBJECT
public:
	QProcess *proc;
	QProcessEnvironment *env;
	QString exec;
	QStringList opt;
	shell(QString,QStringList);
	~shell();
	void run()override;
	void envSetup(main_target*);
private:
signals:
	void exit(QProcess*);
};

#endif // SHELL_H
