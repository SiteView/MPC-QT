#include "soapH.h"
#include "SqliteDb.h"

CSQLiteDb m_SQLiteDb;

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>


/* 得到远程文件的大小, 要下载的文件大小 */
double getDownloadFileLenth(const char *url)
{
    double g_totalSize ;
    CURL *handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_HEADER, 1);    //只需要header头
    curl_easy_setopt(handle, CURLOPT_NOBODY, 1);    //不需要body
    if (curl_easy_perform(handle) == CURLE_OK)
    {
        curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &g_totalSize);
    }
    else
    {
        return false;
    }
    curl_easy_cleanup(handle);
    return g_totalSize;
}


void versionStrToNumber(QString &versionStr, int *versionInt)
{
    QStringList strList;
    strList = versionStr.split(".");
    int i = 0;
    foreach(QString str, strList)
    {
        versionInt[i] = str.trimmed().toInt();
        i++;
    }
}

int versionCompare(QString &versionStr1, QString &versionStr2)
{
    int versionInt1[4] = {0};
    int versionInt2[4] = {0};

    versionStrToNumber(versionStr1, versionInt1);
    versionStrToNumber(versionStr2, versionInt2);

    for (int i = 0; i < 4; ++i)
    {
        if (versionInt1[i] < versionInt2[i])
            return 1;
    }
    return 0;
}

long long generateUUID()
{
    QSqlQuery query(*m_SQLiteDb.getDB());
    m_SQLiteDb.getDB()->transaction();
    query.prepare("SELECT AppID FROM ServerAppInfo ORDER BY AppID DESC");
    if (!query.exec())
    {
        qDebug(query.lastError().text().toLocal8Bit().data());
    }
    while (query.first())
    {
        return query.value(0).toLongLong() + 1;
    }
    return 100000; // TODO
}


int SOAP_FMAC6 MPC__npRequest(struct soap*, struct _MPC__npRequestRequest r_input, struct _MPC__npRequestResponse &r_output)

{
    fprintf(stderr, "start");
    QSqlQuery query(*m_SQLiteDb.getDB());
    QString locVersion = QString::fromStdString(r_input.ResetServerVersion);
    QString locDisplayName = QString::fromStdString(r_input.DisplayName).trimmed();
    // --update server begin
    // check input AppID from server database
    query.prepare("SELECT AppID,ResetServerVerSion ,Size , Downloadurl FROM ServerAppInfo WHERE DisplayName = ?");
    query.addBindValue(locDisplayName);
    if (!query.exec())
    {
        qDebug(query.lastError().text().toLocal8Bit().data());
    }
    // the input App exist in server database
    if(query.next())
    {
        QString serVersion = query.value(1).toString();
        bool bHaveNewVersion = versionCompare(serVersion, locVersion);
        QString URL = query.value(3).toString();
        qlonglong Checksize = query.value(2).toLongLong();
        m_SQLiteDb.getDB()->transaction();
        // have new version, update
        if (bHaveNewVersion&&serVersion.compare("")!=0)
        {
            qDebug() << "Server ver" << serVersion << locVersion;
	    query.prepare("UPDATE ServerAppInfo SET  ResetServerVersion = ? WHERE DisplayName = ?");
            query.addBindValue(QVariant((QString::fromStdString(r_input.ResetServerVersion))));
            query.addBindValue(locDisplayName);
            if (!query.exec())
            {
                qDebug(query.lastError().text().toLocal8Bit().data());
            }
        }
        if(Checksize == 0 && URL.size()>5)
        {
            qlonglong downsize = qlonglong(getDownloadFileLenth(URL.toStdString().c_str()));
            query.prepare("UPDATE ServerAppInfo SET  Size = ? WHERE DisplayName = ?");
            query.addBindValue(QVariant(downsize));
            query.addBindValue(locDisplayName);
            if (!query.exec())
            {
                qDebug(query.lastError().text().toLocal8Bit().data());
            }
        }
		m_SQLiteDb.getDB()->commit();
    }
    else
    {
        // generate a new AppID
	if(locDisplayName.size() >1)
	{
        	m_SQLiteDb.getDB()->transaction();
       	 	query.prepare("INSERT INTO ServerAppInfo ( DisplayName, ResetServerVersion) VALUES(:DisplayName, :ResetServerVersion)");
        	query.bindValue(":DisplayName", QVariant(locDisplayName));
        	query.bindValue(":ResetServerVersion", QVariant(QString::fromStdString(r_input.ResetServerVersion)));
        	if (!query.exec())
        	{
            		qDebug(query.lastError().text().toLocal8Bit().data());
        	}
 
        	m_SQLiteDb.getDB()->commit();
	}

    }
    // --update server finish

    // randomly get a row for output
    query.prepare("SELECT AppID, AppName, DisplayName, Detailtext, Mark, Type, ServerVersion, ResetServerVersion, Size, OrderNumber, AllDownload, FewDownload, DownloadURL, OS FROM ServerAppInfo where Flag = 1 ORDER BY RAND() LIMIT 1");
    if (!query.exec())
    {
        qDebug(query.lastError().text().toLocal8Bit().data());
        CSQLiteDb::DisConnectionDB();
        CSQLiteDb::ConnectionDB(QString( "" ) );
    }
    while (query.next())
    {
        r_output.AppID = 		query.value(0).toString().toStdString();;
        r_output.AppName = 		QString::fromLocal8Bit(query.value(1).toByteArray()).toStdString();
        r_output.DisplayName = 	QString::fromLocal8Bit(query.value(2).toByteArray()).toStdString();
        r_output.Detailtext = 	QString::fromLocal8Bit(query.value(3).toByteArray()).toStdString();
        r_output.Mark = 		QString::fromLocal8Bit(query.value(4).toByteArray()).toStdString();
        r_output.Type = 		QString::fromLocal8Bit(query.value(5).toByteArray()).toStdString();
        r_output.ServerVersion = QString::fromLocal8Bit(query.value(6).toByteArray()).toStdString();
        r_output.ResetServerVersion = QString::fromLocal8Bit(query.value(7).toByteArray()).toStdString();
        r_output.Size = 		QString::fromLocal8Bit(query.value(8).toByteArray()).toStdString();
        r_output.OrderNumber = 	QString::fromLocal8Bit(query.value(9).toByteArray()).toStdString();
        r_output.AllDownload = 	QString::fromLocal8Bit(query.value(10).toByteArray()).toStdString();
        r_output.FewDownload = 	QString::fromLocal8Bit(query.value(11).toByteArray()).toStdString();
        r_output.DownloadURL = 	QString::fromLocal8Bit(query.value(12).toByteArray()).toStdString();
        r_output.OS = 			QString::fromLocal8Bit(query.value(13).toByteArray()).toStdString();
#if 0
        qDebug() << query.value(0)  << r_output.AppID.c_str() ;
        qDebug() << query.value(1) ;
        qDebug() << query.value(2) ;
        qDebug() << query.value(3) ;
        qDebug() << query.value(4) ;
        qDebug() << query.value(5) ;
        qDebug() << query.value(6) ;
        qDebug() << query.value(7) ;
        qDebug() << query.value(8) ;
        qDebug() << query.value(9) ;
        qDebug() << query.value(10) ;
#endif
    }
    query.finish();
    return SOAP_OK;
}


int SOAP_FMAC6 MPC__flashReguest(struct soap*, long start_input, struct _MPC__npRequestResponse *pReq, long &count)
{
    return SOAP_OK;
}
int SOAP_FMAC6 MPC__typeRequest(struct soap*, long type, long order, struct _MPC__npRequestResponse &r_output)
{
    return SOAP_OK;
}
