#ifndef STORAGEREAD_H
#define STORAGEREAD_H

#include <QThread>
#include <QStandardItem>
#include <QList>
class storageRead : public QThread
{
	Q_OBJECT
public:
	explicit storageRead(QStandardItem*,QString);
	void run()override;
private:
	QStandardItem *branch;
	QString directory;
signals:
	void model_add_prefix(QStandardItem*,QList<QStandardItem*>*);
};

#endif // STORAGEREAD_H
