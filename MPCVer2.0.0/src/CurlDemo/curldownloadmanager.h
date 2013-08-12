#ifndef CURLDOWNLOADMANAGER_H
#define CURLDOWNLOADMANAGER_H

#include <QThread>
#include <QUrl>
#include <QDebug>
#include <QProgressBar>
#include <QFileInfo>

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

struct FtpFile
{
    const char *filename;
    FILE *stream;
};

class CURLDownloadManager : public QThread
{
    Q_OBJECT
signals:
    void DownloadFinish(int finishCode);
    void Setvalue(int value);
    //使用以下方法关联到当前进度条 或其他进度接收方式
    //connect(CURLDownloadManager::getThis(),SIGNAL(Setvalue(int)),progressbar*,SLOT(setValue(int)));
private:
    static CURLDownloadManager *pthis;

public:
    CURLDownloadManager(QObject *parent);
    ~CURLDownloadManager();
    static	CURLDownloadManager * getThis()
    {
        return pthis;
    }
    void setUrl(QString urlstr)
    {
        m_urlStr = urlstr;
    }

    void setSavefileName(QString fileName)
    {
        m_fileName = fileName;
        strcpy(m_fileNameBuffer,m_fileName.toStdString().data());
    }
	bool isBusy()
	{
		return m_busy;
	}
    void ready( bool ready)
    {
        m_ready = ready;
    }
    virtual void run();
    /* 得到远程文件的大小, 要下载的文件大小 */
    bool getDownloadFileLenth(const char *url)
    {
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
        return true;
    }

    /* 得到本地文件大小的函数, 若不是续传则返回0, 否则返回指定路径地址的文件大小 */
long long  getLocalFileLenth(const char* localPath)
    {
        //return 0;
        QFileInfo info1(localPath);
        return info1.size();
    }
    static double g_totalSize ;
    static int progressvalue;
private:
    bool m_ready;
	bool m_busy;
    QString m_urlStr;
    QString m_fileName;
    char m_fileNameBuffer[255];

};
static int CurlDebugFunc(CURL *curl, curl_infotype type,
	char *buffer, size_t size, void *stream)
{
	if(type==CURLINFO_DATA_IN || type == CURLINFO_DATA_OUT){
		return 0;
	}
	qDebug("%s  Len %d",buffer,size);
	return 0;
}
static int my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
    struct FtpFile *out=(struct FtpFile *)stream;
    if(out && !out->stream)
    {
        out->stream=fopen(out->filename, "wb");// 不续传
   //   out->stream=fopen(out->filename, "ab");// 续传
        if(!out->stream)
        {
            qDebug("failure, can't open file to write");
            return -1;
        }
    }
    return fwrite(buffer, size, nmemb, out->stream);
}

static int progress_func(void* ptr, double rDlTotal, double rDlNow, double rUploadTotal, double rUploadNow)
{
    if(CURLDownloadManager::g_totalSize< 0.0001) // 不续传
    {
        qDebug("%f %f %f %f %f",100.0 * (rDlNow/rDlTotal),rDlTotal,rDlNow,rUploadTotal,rUploadNow);
        CURLDownloadManager::progressvalue  = 100.0*rDlNow/rDlTotal;
        emit CURLDownloadManager::getThis()->Setvalue(CURLDownloadManager::progressvalue);
        return 0;
    }
    if(!(rDlTotal < 0.0001))
        qDebug("%f %f %f %f %f",100.0 * ((rDlNow+(CURLDownloadManager::g_totalSize-rDlTotal))/CURLDownloadManager::g_totalSize),rDlTotal,rDlNow,rUploadTotal,rUploadNow);
    else
        qDebug("%f %f %f %f",rDlTotal,rDlNow,rUploadTotal,rUploadNow);
    return 0;
}
#endif // CURLDOWNLOADMANAGER_H
