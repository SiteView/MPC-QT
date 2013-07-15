#include "mainwindow.h"
#include <QApplication>
#include "QFile"
#include "QDebug"

#include "../Common/SqliteDb.h"
#include "../Common/SytemLog.h"
#include "src/informdialog.h"
// 全局数据库对象
QSqlDatabase CSQLiteDb::m_db;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
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

    QFile qssFile(":/styles/Default.qss");
    if(qssFile.open(QFile::ReadOnly))
    {
        qDebug()<<"qss is ok ";
    }
    qApp->setStyleSheet(qssFile.readAll());
    qssFile.close();

	MainWindow w;
    w.show();

	int ret = a.exec();
	CSQLiteDb::DisConnectionDB();
	return ret;
	}


