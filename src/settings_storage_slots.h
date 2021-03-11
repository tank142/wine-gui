#ifndef SETTINGS_SLOTS_H
#define SETTINGS_SLOTS_H

#include <QWidget>
#include <QHBoxLayout>
#include <QString>
#include <QComboBox>
#include <QPushButton>
#include <QToolButton>
#include <QVector>
#include <tuple>
#include "settings.h"
class settings_storage_slots : public QWidget
{
	Q_OBJECT
private:
	QHBoxLayout *hbox;
	QString directory;
	settings *s;
public:
	explicit settings_storage_slots(settings *S,QHBoxLayout *H){s = S;hbox = H;}
public slots:
	void set_dir();
	void move_up();
	void move_down();
	void del();
};

#endif // SETTINGS_SLOTS_H
