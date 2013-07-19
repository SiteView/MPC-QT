#ifndef REGFLASHCLASS_H
#define REGFLASHCLASS_H

#include <QFileInfo>
#include <QThread>
#include <QDir>
#include <QDateTime>
#include <QStringList>

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
    // add by shu-yuan
    bool updateFieldValue(QSqlQuery sqlQuery, const QString &fieldName, const QVariant &value, const QVariant &var);
    // add end: shu-yuan

};

#endif // REGFLASHCLASS_H
