#include "slot_set_dir.h"
#include <QFileDialog>
void slot_set_dir::set_dir(){
	QString d=(QFileDialog::getExistingDirectory(this,
				QString::fromUtf8("Открыть папку"),
				QDir::homePath() + "/.local",
				QFileDialog::ShowDirsOnly));
	if(d != "" ){b->setText(d);}
}
