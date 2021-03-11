#ifndef PREFIXTABLE_H
#define PREFIXTABLE_H

#include <QObject>
#include <QTreeView>
#include <QStandardItemModel>
#include <QVector>
#include <tuple>
#include "main_target.h"
using namespace std;
class prefixTable : public QObject
{
	Q_OBJECT
public:
	explicit prefixTable(main_target*,QObject *parent = nullptr);
	~prefixTable();
	QStandardItemModel *model;
	QTreeView *treeView;
	main_target *target;
	void wait();
private:
	quint16 threadsRun = 0;
	void model_clear();
	void model_add_prefix(QStandardItem*,QList<QStandardItem*>*);
public slots:
	void model_update();
	void threadEnd();
signals:
	void readFinished();
};

#endif // PREFIXTABLE_H
