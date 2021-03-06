#ifndef SHELLOUTPUT_H
#define SHELLOUTPUT_H

#include "shell.h"
#include <QTextEdit>
#include <QWidget>
#include <QVBoxLayout>
class shellOutputErrCk : public QWidget
{
	Q_OBJECT
public:
	explicit shellOutputErrCk(QString,QStringList,QWidget *parent = nullptr);
	shell *exec;
	void start();
private:
	QVBoxLayout *vbox;
private slots:
	void output(QProcess*);
signals:

};
class shellOutputDebugging : public QWidget
{
	Q_OBJECT
public:
	explicit shellOutputDebugging(QString,QStringList,QWidget *parent = nullptr);
	shell *exec;
	void start();
public slots:
	void exit(int);
private:
	QTextEdit *text;
	QTextCursor *cursor;
private slots:
	void addText();
signals:

};

#endif // SHELLOUTPUT_H
