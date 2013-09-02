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

private:
    CSQLiteDb m_SQLiteDb;
};

#endif // SYNSERVERTHREAD_H
