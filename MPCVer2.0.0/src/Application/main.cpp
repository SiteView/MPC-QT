
#include <QApplication>
#include <QFile>
#include <QDebug>

#include "mainwindow.h"
#include "../Common/SqliteDb.h"
#include "../Common/SytemLog.h"
#include "src/informdialog.h"

#include "src/SoftDownloadItem.h"
// 全局数据库对象
QSqlDatabase CSQLiteDb::m_db;


#include "curldownloadmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	//注册MsgHandler 可以输出标准日志文件
    curl_global_init(CURL_GLOBAL_DEFAULT);

//    SoftDownloadItem item;
//    CURLDownloadManager downloader(&item);
//    downloader.start();
//    downloader.setUrl("http://dl.google.com/pinyin/v2/GooglePinyinInstaller.exe");
//    downloader.setSavefileName("c:/GooglePinyinInstaller.exe");
//    downloader.ready(true);

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
    curl_global_cleanup();
	return ret;
	}


