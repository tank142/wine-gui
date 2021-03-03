#ifndef PREFIXADD_H
#define PREFIXADD_H

#include "shell.h"
#include "dirWidget.h"
#include "main_target.h"
#include <QWidget>
#include <QGroupBox>
#include <QString>
#include <QIcon>
#include <QHBoxLayout>
#include <QLabel>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QLineEdit>
#include <QProcess>
#include <QComboBox>
#include <QRadioButton>
#include <QPushButton>
#include <tuple>
using namespace std;
class prefixAdd : public QWidget
{
	Q_OBJECT
	public:
	explicit prefixAdd(QIcon,QStandardItemModel*,main_target*,QWidget *parent = nullptr);
		QLabel *text;
		QLabel *text_target;
		int short storage;
		int short storage_target;
		QString target_wineprefix;
		QComboBox *wine_ver;
		QRadioButton *move;
		QRadioButton *copy;
		QRadioButton *create;
		dirWidget *cpdir;
		QStandardItemModel *wine_vers;
		bool check_64bit(const char* elfFile);
	private:
		QVBoxLayout *main;
		QHBoxLayout *box_storage;
		QStandardItemModel *model;
		main_target *target;
		QPushButton *button;
		QLineEdit *edit;
		QComboBox *bit;
		void cp();
		void mv();
		void find_links();
	public slots:
		void create_check();
		void create_slot();
		void cancel_slot();
		void wine_arch();
		void update_slot(QModelIndex);
	signals:
		void model_update();
		void main_tool_widget_hide(bool);
};

#endif // PREFIXADD_H
