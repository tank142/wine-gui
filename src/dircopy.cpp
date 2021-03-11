#include "dircopy.h"
#include "findlink.h"
#include <QRegularExpression>
#include <QStandardPaths>
void dirCopy::run(){
	dirRecurse(dir_source.path());
	emit create_bar(sizeAll);
	copyTarget();
	emit exit();
	this->deleteLater();
}
void dirMove::run(){
	dirRecurse(dir_source.path());
	emit create_bar(sizeAll);
	copyTarget();
	emit closeBar();
	emit msg("Обработка ярлыков...");
	findLink ln(dir_source.path() + "/shortcuts",QDir::homePath() + "/.local/share/applications");
	ln.lnFindRc();
	ln.lnMove(dir_target + "/shortcuts");
	ln.dir = QDir::homePath() + "/.local/share/icons";
	ln.lnFindRc();
	ln.lnMove(dir_target + "/shortcuts");
	ln.dir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	ln.lnFind();
	ln.lnMove(dir_target + "/shortcuts");
	emit msg("Удаление оригинала...");
	QDir(dir_source.path()).removeRecursively();
	emit exit();
	this->deleteLater();
}
