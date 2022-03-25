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
	virtual void start();
protected:
	void move(QString);
	void mv(QString,QString,QString);
	void workPng();
signals:
	void exit();
};

class iconExtractIco : iconExtract
{
	Q_OBJECT
public:
	explicit iconExtractIco(QString a,QString b,QString c,QObject *parent = nullptr):iconExtract(a,b,c,parent){};
	void start()override;
};

#endif // ICONEXTRACT_H
