#ifndef TABSETTINGSREGKEY_H
#define TABSETTINGSREGKEY_H
#include "fileregistry.h"
#include <QObject>
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>
class tabSettingsRegkey : public QHBoxLayout
{
	Q_OBJECT
public:
	explicit tabSettingsRegkey(fileRegistry*,int,QString,QString,QStringList,QWidget *parent = nullptr);
	~tabSettingsRegkey();
	void reset();
	void setValue();
	QLabel *key;
	QComboBox *value;
private:
	fileRegistry *reg;
	QString branch;
};

#endif // TABSETTINGSREGKEY_H
