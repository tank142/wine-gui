#ifndef ICONEXTRACT_H
#define ICONEXTRACT_H
#include <QObject>

class iconExtract : public QObject
{
	Q_OBJECT
public:
	explicit iconExtract(QString,QString,QString,QObject *parent = nullptr);
	QString source;
	QString dir;
	QString name;
	void start();
private:
	void move(QString);
	void mv(QString,QString,QString);
signals:
	void exit();
};

#endif // ICONEXTRACT_H
