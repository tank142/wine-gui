#include "slot_icon.h"
#include <QComboBox>
#include <QString>
#include <QTextStream>
#include <QIcon>
#include <QDebug>
using namespace Qt;
void slot_icon::icon()
{
	qDebug() << b->itemText(b->currentIndex());
}
