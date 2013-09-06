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
    //soap_set_mode(&mpcsoap, SOAP_C_UTFSTRING);

    // control timeout
    mpcsoap.connect_timeout = 5;  // units are seconds
    mpcsoap.send_timeout = 5;
    mpcsoap.recv_timeout = 5;

    // pull the row data to server database
    for (;;) {
        // Randomly pick up a row from local database
        QSqlQuery query(*m_SQLiteDb.getDB());
        m_SQLiteDb.getDB()->transaction();

        if (!query.exec("SELECT DisplayName, DisplayVersion, URLInfoAbout, Publisher FROM LocalAppInfor ORDER BY RANDOM() LIMIT 1")) {
            qDebug(query.lastError().text().toLocal8Bit().data());
        }
        while (query.next()) {
            inputdata.DisplayName = query.value(0).toString().toStdString();
            inputdata.ResetServerVersion = query.value(1).toString().toStdString();
            inputdata.URLInfoAbout = query.value(2).toString().toStdString();
            inputdata.Publisher = query.value(3).toString().toStdString();
        }

        query.finish();
        m_SQLiteDb.getDB()->commit();


        int ret = soap_call_MPC__npRequest(&mpcsoap,"192.168.9.2:8089","",inputdata, outdata); // TODO /*220.168.30.10:8089  192.168.9.2:8089*/

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

        msleep(10000);
    }
}

int SynServerThread::updateLocRecord(_MPC__npRequestResponse responseData)
{
    QVariant serverAppID = QVariant((QString::fromStdString(responseData.AppID)).toLongLong());

    QSqlQuery updateLocQuery(*m_SQLiteDb.getDB());
    m_SQLiteDb.getDB()->transaction();

    // check AppID from local database
    updateLocQuery.prepare("SELECT AppID FROM ServerAppInfo WHERE AppID = ?");
    updateLocQuery.addBindValue(serverAppID);
    if (!updateLocQuery.exec()) {
        qDebug(updateLocQuery.lastError().text().toLocal8Bit().data());
        return 0;
    }

    bool bHaveNext = updateLocQuery.next();
    // the response AppID exist in local database
    if (bHaveNext) {
        updateLocQuery.prepare("UPDATE ServerAppInfo SET AppName = ?, DisplayName = ?, Detailtext = ?, Mark = ?, Type = ?, ServerVersion = ?, ResetServerVersion = ?, Size = ?, OrderNumber = ?, AllDownload= ?, FewDownload = ?, DownloadURL = ?, OS = ? WHERE AppID = ?");
    }
    // the response AppId not exist in local database
    else {
        updateLocQuery.prepare("INSERT INTO ServerAppInfo (AppID, AppName, DisplayName, Detailtext, Mark, Type, ServerVersion, ResetServerVersion, Size, OrderNumber, AllDownload, FewDownload, DownloadURL, OS)"
                               "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        updateLocQuery.addBindValue(QVariant((QString::fromStdString(responseData.AppID)).toLongLong()));
    }
    updateLocQuery.addBindValue(QVariant(QString::fromStdString(responseData.AppName)));
    updateLocQuery.addBindValue(QVariant(QString::fromStdString(responseData.DisplayName)));
    updateLocQuery.addBindValue(QVariant(QString::fromStdString(responseData.Detailtext)));
    updateLocQuery.addBindValue(QVariant(QString::fromStdString(responseData.Mark)));
    updateLocQuery.addBindValue(QVariant((QString::fromStdString(responseData.Type)).toInt()));
    updateLocQuery.addBindValue(QVariant(QString::fromStdString(responseData.ServerVersion)));
    updateLocQuery.addBindValue(QVariant(QString::fromStdString(responseData.ResetServerVersion)));
    updateLocQuery.addBindValue(QVariant((QString::fromStdString(responseData.Size)).toLongLong()));
    updateLocQuery.addBindValue(QVariant((QString::fromStdString(responseData.OrderNumber)).toLongLong()));
    updateLocQuery.addBindValue(QVariant((QString::fromStdString(responseData.AllDownload)).toInt()));
    updateLocQuery.addBindValue(QVariant((QString::fromStdString(responseData.FewDownload)).toInt()));
    updateLocQuery.addBindValue(QVariant(QString::fromStdString(responseData.DownloadURL)));
    updateLocQuery.addBindValue(QVariant((QString::fromStdString(responseData.OS)).toInt()));

    if (bHaveNext) {
        updateLocQuery.addBindValue(serverAppID);
    }

    if (!updateLocQuery.exec()) {
        qDebug(updateLocQuery.lastError().text().toLocal8Bit().data());
        return 0;
    }

    updateLocQuery.finish();
    m_SQLiteDb.getDB()->commit();

    return 1;
}

//CP_ACP,CP_UTF8
wchar_t* MulityByteToWideChar(UINT CodePage, char *str)
{
    DWORD dwNum = MultiByteToWideChar(CodePage, 0, str, -1, 0, 0);
    wchar_t *pwText = new wchar_t[dwNum];
    MultiByteToWideChar(CodePage, 0, str, -1, pwText, dwNum);
    return pwText;
}

char* WideCharToMulityByte(UINT CodePage, wchar_t *str)
{
    int len = WideCharToMultiByte(CodePage, 0, str, -1, 0, 0, 0, 0);
    char* output = new char[len + 2];
    WideCharToMultiByte(CodePage, 0, str, -1, output, len + 1, 0, 0);
    return output;
}


