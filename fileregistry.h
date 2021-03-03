#ifndef FILEREGISTRY_H
#define FILEREGISTRY_H

#include <QObject>
#include <QFile>
#include <QVector>
#include <QTextStream>
class fileRegistry : public QObject
{
	Q_OBJECT
public:
	explicit fileRegistry(QString,QStringList,QObject *parent = nullptr);
	~fileRegistry();
	QMap<QString, QMap<QString,QString>*> registryBranchs;
	QString value(QString,QString);
	void setValue(QString,QString,QString);
	void remove(QString,QString);
	void write();
private:
	QFile *FILE;
signals:

};
#endif // FILEREGISTRY_H
