#ifndef SLOT_ICON_H
#define SLOT_ICON_H

#include <QWidget>
#include <QComboBox>
#include <QString>

class slot_icon : public QWidget
{
	Q_OBJECT
private:
	QComboBox *b;
public:
	slot_icon(QComboBox *B){b = B;}
public slots:
	void icon();
};

#endif // SLOT_ICON_H
