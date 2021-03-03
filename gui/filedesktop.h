#ifndef FILEDESKTOP_H
#define FILEDESKTOP_H

#include <QObject>
#include <QFile>
#include <QTextStream>
class fileDesktop : public QObject
{
	Q_OBJECT
public:
	explicit fileDesktop(QString,QObject *parent = nullptr);
	QFile *FILE;
	QString Name;
	QString Comment;
	QString Exec;
	QString Path;
	QString Icon;
	QString Categories;
	QString StartupWMClass;
	void write();
private:
	void line(QString);
	bool check_type(QStringList);
signals:

};

#endif // FILEDESKTOP_H
