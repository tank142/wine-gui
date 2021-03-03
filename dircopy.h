#ifndef DIRCOPY_H
#define DIRCOPY_H
#include "dirwork.h"
class dirCopy : public dirWork
{
	Q_OBJECT
public:
	explicit dirCopy(QString d,QString t){dir_source.setPath(d);dir_target = t;};
	void run()override;
};
class dirMove : public dirWork
{
	Q_OBJECT
public:
	explicit dirMove(QString d,QString t){dir_source.setPath(d);dir_target = t;};
	void run()override;
};
#endif // DIRCOPY_H
