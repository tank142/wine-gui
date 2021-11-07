#ifndef FILESHELL_H
#define FILESHELL_H

#include <QObject>
#include <QFile>
class fileShell : public QObject
{
	Q_OBJECT
public:
	explicit fileShell(QString f,QObject *parent = nullptr);
	void write();
	QFile *FILE;
	bool WINEESYNC = false;
	bool WINEFSYNC = false;
	bool VKBASALT = false;
	bool gamemoderun = false;
	QString DXVK_HUD;
	bool DXVK_HUD_enable = false;
	QString GALLIUM_HUD;
	bool GALLIUM_HUD_enable = false;
	QString MANGOHUD_CONFIG;
	bool MANGOHUD_CONFIG_enable = false;
	QString STRANGLE;
	bool STRANGLE_enable = false;
	QString EXEC;
	bool existsEXEC;
	QString EXE;
	QString WINE;
	QString WORKDIR;
	//QString WINEPREFIX;
private:
	bool check(QString);
	void line(QString);
signals:

};

#endif // FILESHELL_H
