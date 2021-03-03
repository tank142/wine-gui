#ifndef DEL_H
#define DEL_H
/*while(d->count())
	delete d->takeAt(0);
delete d;
s->update();*/
#include <QObject>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QToolButton>

class del_storage : public QWidget {
	Q_OBJECT
public:
	QHBoxLayout *d;QComboBox *a;QPushButton *b;QToolButton *c;QToolButton *up;QToolButton *down;
	del_storage(QHBoxLayout *D, QComboBox *A, QPushButton *B, QToolButton *C, QToolButton *UP, QToolButton *DOWN)
	{d = D; a = A; b = B; c = C; up = UP; down = DOWN;}

public slots:
	void del();
};

#endif // DEL_H

