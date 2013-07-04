
#include <QtCore/QCoreApplication>

#include "../Common/SqliteDb.h"
#include "../Common/SytemLog.h"

// ȫ�����ݿ����
QSqlDatabase CSQLiteDb::m_db;

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	//ע��MsgHandler ���������׼��־�ļ�
	qInstallMessageHandler(customMessageHandler); 

	if (!CSQLiteDb::ConnectionDB(QString( "./localedb.db" ) ) )
		//if (!CSQLiteDb::ConnectionDB(QString( "" ) ) )
	{
		QString err = QString("database open faile :%1").arg(CSQLiteDb::getDB()->lastError().text());
		qCritical(err.toLocal8Bit().data());
		CSQLiteDb::DisConnectionDB();
		return 0;
	}
	int ret = a.exec();
	CSQLiteDb::DisConnectionDB();
	return ret;
}
