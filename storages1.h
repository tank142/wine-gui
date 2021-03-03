#ifndef STORAGES_H
#define STORAGES_H

#include <QDebug>
#include <QWidget>
#include <QComboBox>
#include <QVector>
#include <tuple>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QToolButton>
using namespace std;
class storages : public QWidget { Q_OBJECT
	public:
		QVBoxLayout *vbox;
		QString directory;
		QVector<tuple<QIcon, QString, QString>> *s;
		QVector<tuple<QHBoxLayout *,QComboBox *,QPushButton *,QToolButton *,QToolButton *,QToolButton *>> s_ui;
		storages(QVector<tuple<QIcon, QString, QString>> *S){
			s = S;
			//
			qDebug() << "Вектор:" << s->length();
		}
		storages(QVBoxLayout *VBOX, QString D){vbox = VBOX; directory = D;}
		storages(QVBoxLayout *VBOX){vbox = VBOX;}
		QComboBox* icons(QString directory);
	public slots:
		void add();
	//signals:

};

#endif // STORAGES_H
