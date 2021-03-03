#include "slot_del_storage.h"
#include <QHBoxLayout>
#include <QLayoutItem>
#include <QWidget>
#include <QComboBox>
#include <QToolButton>
#include <QPushButton>
#include <QGroupBox>
void del_storage::del(){
	a->~QComboBox();
	b->~QPushButton();
	c->~QToolButton();
	d->~QHBoxLayout();
	up->~QToolButton();
	down->~QToolButton();
	del_storage::~del_storage();
};
