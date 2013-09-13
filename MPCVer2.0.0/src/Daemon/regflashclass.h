#ifndef REGFLASHCLASS_H
#define REGFLASHCLASS_H

#include <QFileInfo>
#include <QThread>
#include <QDir>
#include <QStringList>
#include <QVariant>

#include "../Common/SqliteDb.h"

class RegFlashClass : public QThread
{
	Q_OBJECT

public:
    RegFlashClass(/*QObject *parent*/);
    virtual void run();
	void UpdateInfo(QVariant DisplayName);
	~RegFlashClass();

private:
	CSQLiteDb m_SQLiteDb;
    bool updateFieldValue(QSqlQuery sqlQuery, const QString &fieldName, const QVariant &value, const QVariant &var);

};

#endif // REGFLASHCLASS_H
