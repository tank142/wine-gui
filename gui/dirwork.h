#ifndef DIRWORK_H
#define DIRWORK_H
#include <QThread>
#include <QDir>
#include <QVector>
#include <tuple>
using namespace std;
class dirWork : public QThread
{
	Q_OBJECT
public:
	bool close_t = false;
	quint64 sizeAll = 0;
	void dirRecurse(QString);
	void copyTarget();
	QString fileSize(qint64 nSize);
	QVector<tuple<QString,QString,quint64>> files;
	QDir dir_source;
	QString dir_target;
signals:
	QString msg(QString);
	qint64 create_bar(qint64);
	qint64 update_bar(qint64);
	void closeBar();
	void exit();
};

#endif // DIRWORK_H
