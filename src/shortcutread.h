#ifndef SHORTCUTREAD_H
#define SHORTCUTREAD_H

#include <QObject>
#include "main_target.h"
class shortcutRead : public QObject
{
	Q_OBJECT
public:
	explicit shortcutRead(QString,QString,main_target*,QObject *parent = nullptr);

private:
	void fixWinePath(QString,QString);
	void fixWinePathDesktop(QString);
	QStringList list;
	QString winedir;
	main_target *target;
signals:

};

#endif // SHORTCUTREAD_H
