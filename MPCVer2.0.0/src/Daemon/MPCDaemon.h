//#include <QCoreApplication>
//#include <QIcon>
//#include <QDebug>
//#include <QFile>
//#include "../../Common/SqliteDb.h"
//#include "../../Common/SytemLog.h"
//#include "regflashclass.h"
//#include "synserverthread.h"
//#include "./gsoap/PMPC.nsmap" // important, necessary
//
//
//// ȫ�����ݿ����
//QSqlDatabase CSQLiteDb::m_db;
//
//static void daemonFunction()
////int main(int argc, char *argv[])
//{
//    //ע��MsgHandler ���������׼��־�ļ�
//    qInstallMessageHandler(customMessageHandler);
//
//    qDebug() << "daemonFunction start";
//
//    if (!CSQLiteDb::ConnectionDB(QString( "./../localedb.db" ) ) )
//        //if (!CSQLiteDb::ConnectionDB(QString( "" ) ) )
//    {
//        QString err = QString("database open faile :%1").arg(CSQLiteDb::getDB()->lastError().text());
//        qCritical(err.toLocal8Bit().data());
//        CSQLiteDb::DisConnectionDB();
//    }
//
//    RegFlashClass  task;
//    task.start();
//
//    SynServerThread synServer;
//    synServer.start();
//
//    CSQLiteDb::DisConnectionDB();
//    task.terminate();
//}
