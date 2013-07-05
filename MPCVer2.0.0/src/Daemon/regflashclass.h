#ifndef REGFLASHCLASS_H
#define REGFLASHCLASS_H

#include <QFileInfo>
#include <QThread>
#include <QDir>
#include <QDateTime>

#include "./SOAServiceStub.h"
#include "../Common/SqliteDb.h"


class RegFlashClass : public QThread
{
	Q_OBJECT

public:
	RegFlashClass(QObject *parent);
    virtual void run();
	void UpdateInfo();
	~RegFlashClass();

private:
	CSQLiteDb m_SQLiteDb;
};

#endif // REGFLASHCLASS_H
