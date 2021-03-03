#ifndef FINDLINK_H
#define FINDLINK_H

#include <QObject>

class findLink : public QObject
{
	Q_OBJECT
public:
	explicit findLink(QString,QString,QObject *parent = nullptr);
	QString target;
	QString dir;
	QStringList links;
	QStringList linksTargets;
	bool ck();
	bool ckRc();
	void rm();
	void rmRc();
	void lnFindRc();
	void lnFind();
	void lnMove(QString);
private:
	bool ckrc(QString);
	void findLn(QString);
	void find(QString);
signals:

};

#endif // FINDLINK_H
