
#include <QtCore/QCoreApplication>
#include <QDebug>
#include <pthread.h>

#include "PMPC.nsmap"
#include "SqliteDb.h"
#include "SytemLog.h"

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
        qDebug("%f",g_totalSize);
        return g_totalSize;
    }

QSqlDatabase CSQLiteDb::m_db;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qInstallMessageHandler(customMessageHandler);

  curl_global_init(CURL_GLOBAL_DEFAULT);


    struct soap soap;
    int m, s; // master and slave sockets
         if (!CSQLiteDb::ConnectionDB(QString( "" ) ) )
        {
            QString err = QString("database open faile :%1").arg(CSQLiteDb::getDB()->lastError().text());
            qCritical(err.toLocal8Bit().data());
            CSQLiteDb::DisConnectionDB();
            return 0;
        }
    soap_init(&soap);
    m = soap_bind(&soap, 0,8089, 200);
    if (m < 0)
        soap_print_fault(&soap, stderr);
    else
    {
        qDebug("Socket connection successful: master socket = %d\n", m);
        for (int i = 1;; i++)
        {
            s = soap_accept(&soap);
            if (s < 0)
			{
                soap_print_fault(&soap, stderr);
				break;
			}
           // qDebug("%d: accepted connection from IP=%d.%d.%d.%d socket=%d", i,(soap.ip>>24)&0xFF, (soap.ip>>16)&0xFF, (soap.ip>>8)&0xFF, soap.ip&0xFF, s);
            if (soap_serve(&soap) != SOAP_OK) // process RPC request
                soap_print_fault(&soap, stderr); // print error
    		soap_destroy(&soap); // clean up class instances
   		soap_end(&soap); // clean up everything and close socket
        }
    }
    soap_done(&soap); // close master socket and detach context
    int rec;
    rec = a.exec();
    	CSQLiteDb::DisConnectionDB();
    curl_global_cleanup();

    return rec;
}
