
#include <QtCore/QCoreApplication>

#include "../Common/SqliteDb.h"
#include "../Common/SytemLog.h"
#include "regflashclass.h"

// 全局数据库对象
QSqlDatabase CSQLiteDb::m_db;


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
	int ret = a.exec();

	CSQLiteDb::DisConnectionDB();
	task.terminate();
	
	return ret;
}
