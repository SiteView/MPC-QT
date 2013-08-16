
#include <QtCore/QCoreApplication>
#include <QIcon>

#include "../Common/SqliteDb.h"
#include "../Common/SytemLog.h"
#include "regflashclass.h"

// 全局数据库对象
QSqlDatabase CSQLiteDb::m_db;

#include "./PMPC.nsmap"



int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);


	//注册MsgHandler 可以输出标准日志文件
	qInstallMessageHandler(customMessageHandler); 

	if (!CSQLiteDb::ConnectionDB(QString( "./localedb.db" ) ) )
		//if (!CSQLiteDb::ConnectionDB(QString( "" ) ) )
	{
		QString err = QString("database open faile :%1").arg(CSQLiteDb::getDB()->lastError().text());
		qCritical(err.toLocal8Bit().data());
		CSQLiteDb::DisConnectionDB();
		return 0;
	}


	RegFlashClass  task(0);
	task.start();
	int ret = 0;
	//struct soap add_soap;
	//add_soap.connect_timeout = 5; //单位是秒
	//add_soap.send_timeout = 5;
	//add_soap.recv_timeout = 5;
	//soap_init(&add_soap);
	//_MPC__npRequestRequest inputdata;
	//_MPC__npRequestResponse outdata;
	//inputdata.DisplayName = std::string("shfsdhf");
	//inputdata.DisplayVersion = std::string("shfsdhf");
	//inputdata.Publisher = std::string("shfsdhf");
	//inputdata.URLInfoAbout = std::string("shfsdhf");

	//qDebug() <<"start";
	//for(long i = 0;i<100000; i++)
	//{
	//	ret = soap_call_MPC__npRequest(&add_soap,"192.168.9.64:8089","",inputdata,outdata);
	//	//Sleep(1);
	//}
	//qDebug() <<"end";
	ret = a.exec();

	CSQLiteDb::DisConnectionDB();
//	task.terminate();
	
	return ret;
}
