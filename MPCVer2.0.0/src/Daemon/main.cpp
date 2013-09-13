#include <QApplication>
#include <QIcon>
#include "../Common/SqliteDb.h"
#include "../Common/SytemLog.h"
#include "./winservice/svc_win32.h"
#include "regmonitor.h"
#include "gsoap/PMPC.nsmap"  // important, only once

// 全局数据库对象
QSqlDatabase CSQLiteDb::m_db;

int main(int argc, char *argv[])
{   
    if(argc == 2)
    {
        ::ak_main(argc,argv);
        return 0;
    }

    QApplication a(argc, argv);

    //注册MsgHandler 可以输出标准日志文件
    qInstallMessageHandler(customMessageHandler);

    if (!CSQLiteDb::ConnectionDB(QString(qApp->applicationDirPath() +  "/localedb.db" ) ) )
    {
        QString err = QString("database open fail :%1").arg(CSQLiteDb::getDB()->lastError().text());
        qCritical(err.toLocal8Bit().data());
        CSQLiteDb::DisConnectionDB();
        return 0;
    }

    ::ak_main(argc,argv);

    int ret = 0;
    ret = a.exec();

    CSQLiteDb::DisConnectionDB();

    return ret;
}

