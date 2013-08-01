#include <QApplication>
#include <QtGlobal>

#include "allapplicationdialog.h"
#include "start.h"

#include "../SytemLog.h"
#include "../SqliteDb.h"

// 全局数据库对象
QSqlDatabase CSQLiteDb::m_db;


int main(int argc, char ** argv)
{
    QApplication app( argc, argv );

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

    AllApplicationDialog *hello = new AllApplicationDialog();
    hello->show();
    return app.exec();
}

