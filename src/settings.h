#ifndef SETTINGS_H
#define SETTINGS_H
#include <QDebug>
#include <QWidget>
#include <QDialog>
#include <QString>
#include <QDir>
#include <QVector>
#include <QThread>
#include <QVBoxLayout>
#include <QToolButton>
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>
#include <QCheckBox>
#include <QStandardItemModel>
#include <tuple>
#include "main_target.h"
using namespace std;
class settings : public QWidget {
	Q_OBJECT
	public:
		void check();
		void del();
		QString directory;
		QVBoxLayout *storage_vbox2;
		QComboBox* icons(QString);
		QVector<tuple<QHBoxLayout*,QComboBox *,QPushButton *,QToolButton *,QToolButton *,QToolButton *>> *s_ui;
		explicit settings(QStandardItemModel*, main_target*, QWidget *parent = nullptr);
		~settings();
	public slots:
		void ok_s();
		void resizeHeight();
	private slots:
		void storage_add();
		void winever_s();
		void dxvk_b_s();
		void nine_b_s();

	private:
		void setdir(QPushButton *);
		void save_conf(QVector<QString>*);
		QString find_name(QString);
		QStandardItemModel *model;
		main_target *target;
		QToolButton *ok;
		QPushButton *winever;
		QCheckBox *dark_theme;
		QPushButton *dxvk_b;
		QPushButton *nine_b;
		QPushButton *winetricks_b;
		void storages_update();
		QString find_storage_name(QStringList*,QStringList*,QString);
		void winetricks_b_s();
signals:
		void model_update();
};
#endif // SETTINGS_H
