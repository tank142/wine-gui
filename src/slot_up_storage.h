#ifndef SLOT_UP_STORAGE_H
#define SLOT_UP_STORAGE_H

#include <QWidget>
#include <QVBoxLayout>
class slot_up_storage
{
public:
	QVBoxLayout *up;
	QVBoxLayout *uph;
	slot_up_storage(QVBoxLayout *UP, QVBoxLayout *UPH){
		up = UP;
		uph = UPH;
	}
public slots:
	void up_storage();
};

#endif // SLOT_UP_STORAGE_H
