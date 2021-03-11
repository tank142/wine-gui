#include "dirWidget.h"
dirWidget::dirWidget(dirWork *w, QWidget *parent) : QWidget(parent)
{
	work = w;
	vbox = new QVBoxLayout(this);
	hbox_bar = new QHBoxLayout(this);
	status = new QStatusBar(this);
	bar = new QProgressBar(this);bar->setHidden(true);
	close = new QToolButton(this);close->setText("Отмена");
	vbox->addWidget(status);
	vbox->addLayout(hbox_bar);
	hbox_bar->addWidget(bar);
	hbox_bar->addWidget(close);
	setLayout(vbox);
	status->showMessage("Поиск файлов...");
	connect( work, &dirWork::msg , this , &dirWidget::updateStatus );
	connect( work, &dirWork::create_bar , this , &dirWidget::createBar );
	connect( work, &dirWork::update_bar , this , &dirWidget::updateBar );
	connect(close, &QToolButton::clicked, this , &dirWidget::stop );
	connect( work, &dirWork::exit , this , &dirWidget::exit );
	connect( work, &dirWork::closeBar , this , &dirWidget::closeBar );
}

QString dirWidget::fileSize(quint64 nSize)
{
	qint64 i = 0;
	for (; nSize > 1023 && 2 > i; ++i) {nSize /= 1024;}
	if(nSize > 1023){
		float fSize;
		for (fSize = nSize; fSize > 1023 && 4 > i; ++i) {fSize /= 1024;}
		return QString().setNum(fSize,'f',3) + "BKMGT"[i];
	}
	return QString().setNum(nSize) + "BKM"[i];
}

void dirWidget::createBar(quint64 max){
	bar->setHidden(false);
	maxSize_text = fileSize(max);
	pctSize = max / 100;
}
void dirWidget::updateBar(quint64 cur){
	status->showMessage(fileSize(cur) + " / " + maxSize_text);
	bar->setValue( cur / pctSize );
}
void dirWidget::closeBar(){
	status->clearMessage();
	bar->deleteLater();
	close->deleteLater();
}
void dirWidget::updateStatus(QString text){
	status->showMessage(text);
}
void dirWidget::stop(){
	work->close_t = true;
}
void dirWidget::exit(){
	emit main_tool_widget_hide();
	this->deleteLater();
}
