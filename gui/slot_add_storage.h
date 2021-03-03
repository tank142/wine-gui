#ifndef ADD_H
#define ADD_H
#include <QWidget>
#include <QVBoxLayout>
#include <QString>
#include <QComboBox>
#include <QPushButton>
#include <QObject>
class add_storage : public QWidget{
	Q_OBJECT
public:
	QVBoxLayout *vbox;
	QString directory;
	add_storage(QVBoxLayout *VBOX, QString D){vbox = VBOX; directory = D;}
	add_storage(QVBoxLayout *VBOX){vbox = VBOX;}
	QComboBox* icons(QString directory);
public slots:
	void add();
};
#endif // ADD_H
