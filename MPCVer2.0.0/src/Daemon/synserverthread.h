#ifndef SYNSERVERTHREAD_H
#define SYNSERVERTHREAD_H

#include "../Common/SqliteDb.h"
#include <QThread>

#include "gsoap/soapH.h"

class SynServerThread : public QThread
{
    Q_OBJECT

public:
    SynServerThread();
    ~SynServerThread();

protected:
    virtual void run();

private:
    int updateLocRecord(_MPC__npRequestResponse responseData);
    int syncDisplayName(); // return 0 stands for the displayName has sync, other is not.


    // Helper functions
    void versionStrToNumber(QString &versionStr, int *versionInt);
    int versionCompare(QString &locVersion, QString &serVersion);

    std::string convertToStdstring(QVariant variantdata);


private:
    CSQLiteDb m_SQLiteDb;
};

#endif // SYNSERVERTHREAD_H
