#ifndef MAIN_TARGET_H
#define MAIN_TARGET_H
#include <QObject>
#include <tuple>
using namespace std;
class main_target : public QObject {
	Q_OBJECT
	public:
	explicit main_target(QObject *parent = nullptr){setParent(parent);};
		int short storage;
		QString prefix;
		QString prefix_path;
		QString prefix_wine;
		QStringList model_storages;
		QString arch;
		QString home;
		QString ICON = "/usr/share/wine-gui/icons/";
		QString CONF;
		QString DXVK;
		QString NINE;
		QString WINE;
		QString WINE_VER;
		QVector<tuple<QString,QString,QString>> storages;
		bool esync;
		char sync;
};
#endif // MAIN_TARGET_H
