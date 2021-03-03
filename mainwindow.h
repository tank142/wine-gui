#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QVector>
#include <tuple>
#include <QMainWindow>
#include <QTabWidget>
#include <QToolButton>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QComboBox>
#include <QSettings>
#include <QTreeView>
#include <QStorageInfo>
#include <QLineEdit>
#include "main_target.h"
#include "prefixtab.h"
#include "prefixtable.h"
using namespace std;
//################################################################
class mainwindow : public QWidget {
	Q_OBJECT
public:
		prefixTab *tab;
		QPushButton *button;
		QPushButton *button2;
		QToolButton *button3;
		prefixTable *table;
		QHBoxLayout *HBOX1;
		main_target *target;
		explicit mainwindow(main_target*,QWidget *parent = nullptr);
		void model_clear();
		QVBoxLayout *VBOX;
public slots:
		void main_tool_widget_hide(bool hidden = true);
		void model_add_prefix(QStandardItem*,QList<QStandardItem*>*);
		void modelWineVer(QString);
		void settings_slot();
private slots:
		void button_slot();
		void button2_slot();
		void settingsEnabled();
		void storage_edit();
		void storage_slot(QModelIndex);
		void storage_slot2(QModelIndex);
private:
		bool edit_create;
		QLineEdit *edit;
		int short edit_target;
		QString edit_target_wineprefix;
		QTabWidget *tabWidget;
		//void dir_model_update(QStandardItemModel*,QString);
};
#endif // MAINWINDOW_H
