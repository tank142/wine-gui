#ifndef SLOT_SET_DIR_H
#define SLOT_SET_DIR_H

#include <QWidget>
#include <QObject>
#include <QPushButton>
class slot_set_dir : public QWidget { Q_OBJECT
	private:
		QPushButton *b;
	public:
		slot_set_dir(QPushButton *B){
			b = B;
		}
	public slots:
		void set_dir();
};

#endif // SLOT_SET_DIR_H
