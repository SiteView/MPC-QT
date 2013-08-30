#include <QTime>
#include <iostream>
#include <QCoreApplication>

#include "synserverthread.h"

SynServerThread::SynServerThread()
    : QThread()
{
}

SynServerThread::~SynServerThread()
{

}

void SynServerThread::run()
{
    _MPC__npRequestRequest inputdata;
    _MPC__npRequestResponse outdata;

    // define a soap to control timeout
    struct soap mpcsoap;
    soap_init(&mpcsoap);
    //soap_set_namespaces(&addsoap, namespaces);
    //soap_set_mode(&addsoap, SOAP_C_UTFSTRING);
    mpcsoap.connect_timeout = 5;  // units are seconds
    mpcsoap.send_timeout = 5;
    mpcsoap.recv_timeout = 5;

    // control timeout
    //MPCSoapProxy proxy(soap);
    //char server[] = "http://192.168.9.2:8089/MPCSoap";
    //proxy.soap_endpoint = server;

    QSqlQuery query(*m_SQLiteDb.getDB());
    m_SQLiteDb.getDB()->transaction();

    // pull the row data to server database
    for (;;) {
        // synchronise LocalAppInfo and ServerAppInfo
        //int retTmp = syncDisplayName();
        //qDebug() << retTmp;

        // Randomly pick up a row from local database
        if (!query.exec("SELECT DisplayName, DisplayVersion, URLInfoAbout, Publisher FROM LocalAppInfor ORDER BY RANDOM() LIMIT 1")) {
            qDebug(query.lastError().text().toLocal8Bit().data());
        }

        while (query.next()) {
            inputdata.DisplayName = query.value(0).toString().toStdString();
            inputdata.ResetServerVersion = query.value(1).toString().toStdString();
            inputdata.URLInfoAbout = query.value(2).toString().toStdString();
            inputdata.Publisher = query.value(3).toString().toStdString();
        }

        int ret = soap_call_MPC__npRequest(&mpcsoap,"192.168.9.2:8089","",inputdata, outdata); // TODO /*220.168.30.10:8089*/

        if (ret == SOAP_OK) {
            updateLocRecord(outdata);
        } else {            
            soap_print_fault(&mpcsoap, stderr);
            soap_destroy(&mpcsoap);
            soap_end(&mpcsoap);
            soap_done(&mpcsoap);

            // new soap again
            struct soap addsoap;
            soap_init(&addsoap);
            addsoap.connect_timeout = 5;  // units are seconds
            addsoap.send_timeout = 5;
            addsoap.recv_timeout = 5;
        }
        query.finish();
        m_SQLiteDb.getDB()->commit();

        // sleep 10msec
        QTime t;
        t.start();
        while(t.elapsed() < 10000)
            QCoreApplication::processEvents();
    }

    // disconnect the database
    CSQLiteDb::DisConnectionDB();
}

int SynServerThread::updateLocRecord(_MPC__npRequestResponse responseData)
{
    QSqlQuery query(*m_SQLiteDb.getDB());
    m_SQLiteDb.getDB()->transaction();

    QVariant serverAppID = QVariant((QString::fromStdString(responseData.AppID)).toLongLong());
    QString serverVersion = QString::fromStdString(responseData.ResetServerVersion);
    QString displayName = QString::fromStdString(responseData.DisplayName);

    qDebug() << serverAppID << serverVersion << displayName;

    if (serverAppID == 0)
        return 0;
    // check AppID from local database
    query.prepare("SELECT AppID, ResetServerVersion FROM ServerAppInfo WHERE AppID = ?");
    query.addBindValue(serverAppID);
    if (!query.exec()) {
        qDebug(query.lastError().text().toLocal8Bit().data());
    }

    // the response AppID exist in local database
    if(query.next()) {
        QString localVersion = query.value(1).toString();
        bool bHaveNewVersion = versionCompare(localVersion, serverVersion);

        // have new version, update version
        if (bHaveNewVersion) {
            query.prepare("UPDATE ServerAppInfo SET ServerVersion = ?, ResetServerVersion = ? WHERE AppID = ?");
            query.addBindValue(QVariant(QString::fromStdString(responseData.ServerVersion)));
            query.addBindValue(QVariant(QString::fromStdString(responseData.ResetServerVersion)));
            query.addBindValue(serverAppID);
            if (!query.exec()) {
                qDebug(query.lastError().text().toLocal8Bit().data());
            }
        }

        // update except version
        query.prepare("UPDATE ServerAppInfo SET AppName = ?, DisplayName = ?, Detailtext = ?, Mark = ?, Type = ?, Size = ?, OrderNumber = ?, AllDownload= ?, FewDownload = ?, DownloadURL = ?, OS = ? WHERE AppID = ?");
        query.addBindValue(QVariant(QString::fromStdString(responseData.AppName)));
        query.addBindValue(QVariant(QString::fromStdString(responseData.DisplayName)));
        query.addBindValue(QVariant(QString::fromStdString(responseData.Detailtext)));
        query.addBindValue(QVariant((QString::fromStdString(responseData.Mark)).toDouble()));
        query.addBindValue(QVariant((QString::fromStdString(responseData.Type)).toInt()));
        query.addBindValue(QVariant((QString::fromStdString(responseData.Size)).toLongLong()));
        query.addBindValue(QVariant((QString::fromStdString(responseData.OrderNumber)).toLongLong()));
        query.addBindValue(QVariant((QString::fromStdString(responseData.AllDownload)).toInt()));
        query.addBindValue(QVariant((QString::fromStdString(responseData.FewDownload)).toInt()));
        query.addBindValue(QVariant(QString::fromStdString(responseData.DownloadURL)));
        query.addBindValue(QVariant((QString::fromStdString(responseData.OS)).toInt()));
        query.addBindValue(serverAppID);
        if (!query.exec()) {
            qDebug(query.lastError().text().toLocal8Bit().data());
        }
    }

    // the response AppId not exist in local database
    else {
        QString serDisplayName = QString::fromStdString(responseData.DisplayName);
        query.prepare("SELECT DisplayName FROM ServerAppInfo WHERE DisplayName = ?");
        query.addBindValue(serDisplayName);
        if (!query.exec()) {
            qDebug(query.lastError().text().toLocal8Bit().data());
        }

        if (query.next()) {
            query.prepare("UPDATE ServerAppInfo SET AppID = ? WHERE DisplayName = ?");
            query.addBindValue(QVariant((QString::fromStdString(responseData.AppID)).toLongLong()));
            query.addBindValue(QVariant(QString::fromStdString(responseData.DisplayName)));
        }
        else {
            // TODO: if server data is NULL, inset is not precise?
            query.prepare("INSERT INTO ServerAppInfo (AppID, AppName, DisplayName, Detailtext, Mark, Type, ServerVersion, ResetServerVersion, Size, OrderNumber, AllDownload, FewDownload, DownloadURL, OS)"
                          "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(QVariant((QString::fromStdString(responseData.AppID)).toLongLong()));
            query.addBindValue(QVariant(QString::fromStdString(responseData.AppName)));
            query.addBindValue(QVariant(QString::fromStdString(responseData.DisplayName)));
            query.addBindValue(QVariant(QString::fromStdString(responseData.Detailtext)));
            query.addBindValue(QVariant(QString::fromStdString(responseData.Mark)));
            query.addBindValue(QVariant((QString::fromStdString(responseData.Type)).toInt()));
            query.addBindValue(QVariant(QString::fromStdString(responseData.ServerVersion)));
            query.addBindValue(QVariant(QString::fromStdString(responseData.ResetServerVersion)));
            query.addBindValue(QVariant((QString::fromStdString(responseData.Size)).toLongLong()));
            query.addBindValue(QVariant((QString::fromStdString(responseData.OrderNumber)).toLongLong()));
            query.addBindValue(QVariant((QString::fromStdString(responseData.AllDownload)).toInt()));
            query.addBindValue(QVariant((QString::fromStdString(responseData.FewDownload)).toInt()));
            query.addBindValue(QVariant(QString::fromStdString(responseData.DownloadURL)));
            query.addBindValue(QVariant((QString::fromStdString(responseData.OS)).toInt()));
        }
        if (!query.exec()) {
            qDebug(query.lastError().text().toLocal8Bit().data());
        }
    }

    return 1;
}

int SynServerThread::syncDisplayName()
{
    QSqlQuery query(*m_SQLiteDb.getDB());
    m_SQLiteDb.getDB()->transaction();

    QVariant displayNameLocal;
    QVariant displayVersionLocal;

    // Randomly get a displayName from localAppInfo
    if (!query.exec("SELECT DisplayName, DisplayVersion FROM LocalAppInfor ORDER BY RANDOM() LIMIT 1")) {
        qDebug(query.lastError().text().toLocal8Bit().data());
    }
    while (query.next()) {
        displayNameLocal = query.value(0);
        displayVersionLocal = query.value(1);
    }

    // check displayNameLocal whether exist in serverAppInfo or not.
    query.prepare("SELECT DisplayName FROM ServerAppInfo WHERE DisplayName = ?");
    query.addBindValue(displayNameLocal);
    if (!query.exec()) {
        qDebug(query.lastError().text().toLocal8Bit().data());
    }
    if (!query.next() && !displayNameLocal.isNull()) {  // displayNameLocal can't be Null
        query.prepare("INSERT INTO ServerAppInfo (DisplayName, ResetServerVersion)"
                      "VALUES (?, ?)");
        query.addBindValue(displayNameLocal);
        query.addBindValue(displayVersionLocal);
        if (!query.exec()) {
            qDebug(query.lastError().text().toLocal8Bit().data());
        }

        query.finish();
        m_SQLiteDb.getDB()->commit();

        return 1;
    }

    return 0;
}

// Helper functions
void SynServerThread::versionStrToNumber(QString &versionStr, int *versionInt)
{
    QStringList strList;
    strList = versionStr.split(".");
    int i = 0;
    foreach(QString str, strList) {
        versionInt[i] = str.toInt();
        i++;
    }
}

int SynServerThread::versionCompare(QString &versionStr1, QString &versionStr2)
{
    int versionInt1[4] = {0};
    int versionInt2[4] = {0};

    versionStrToNumber(versionStr1, versionInt1);
    versionStrToNumber(versionStr2, versionInt2);

    for (int i = 0; i < 4; ++i) {
        if (versionInt1[i] < versionInt2[i])
            return 1;
    }
    return 0;
}

std::string SynServerThread::convertToStdstring(QVariant variantdata)
{
    return variantdata.toString().toStdString();
}
