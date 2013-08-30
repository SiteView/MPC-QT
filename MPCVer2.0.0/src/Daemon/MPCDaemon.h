//#include <QCoreApplication>
//#include <QIcon>
//#include <QDebug>
//#include <QFile>
//#include "../../Common/SqliteDb.h"
//#include "../../Common/SytemLog.h"
//#include "regflashclass.h"
//#include "synserverthread.h"
//#include "./code/gsoap/PMPC.nsmap" // important, necessary


//// 全局数据库对象
//QSqlDatabase CSQLiteDb::m_db;

////static void daemonFunction()
//int main(int argc, char *argv[])
//{
////    int dwArgc;
////    char **lpszArgv;
////    QCoreApplication coreApp(dwArgc, lpszArgv);
////    wchar_t fpath[260];
////    GetModuleFileName(NULL, fpath, 260);

////    memset(fpath, 0, sizeof(fpath));

//	//注册MsgHandler 可以输出标准日志文件
//    qInstallMessageHandler(customMessageHandler);

//    qDebug() << "daemonFunction start";

//    if (!CSQLiteDb::ConnectionDB(QString( "./../localedb.db" ) ) )
//        //if (!CSQLiteDb::ConnectionDB(QString( "" ) ) )
//    {
//        QString err = QString("database open faile :%1").arg(CSQLiteDb::getDB()->lastError().text());
//        qCritical(err.toLocal8Bit().data());
//        CSQLiteDb::DisConnectionDB();
//    }

//    RegFlashClass  task;
//    task.start();

////    coreApp.exec();

//    //SynServerThread synServer;
//    //synServer.start();

//    CSQLiteDb::DisConnectionDB();
//    task.terminate();

////    delete task;
////    delete synServer;

//}
