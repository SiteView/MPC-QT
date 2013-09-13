#ifndef CURLDOWNLOADMANAGER_H
#define CURLDOWNLOADMANAGER_H

#include <QThread>
#include <QUrl>
#include <QDebug>
#include <QProgressBar>
#include <QFileInfo>

#include <string.h>



#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

using namespace std;

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
    //ʹ�����·�����������ǰ������ ���������Ƚ��շ�ʽ
    //connect(CURLDownloadManager::getThis(),SIGNAL(Setvalue(int)),progressbar*,SLOT(setValue(int)));
private:
    static CURLDownloadManager *pthis;
    CURL *curl;

public:
    CURLDownloadManager(QObject *parent);
    ~CURLDownloadManager();

    /// �ļ�д��ص�����
    static int my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
    {
        struct FtpFile *out=(struct FtpFile *)stream;
        if(out && !out->stream)
        {
            out->stream=fopen(out->filename, "wb");// ������
            //   out->stream=fopen(out->filename, "ab");// ����
            if(!out->stream)
            {
                qDebug("failure, can't open file to write");
                return -1;
            }
        }
        return fwrite(buffer, size, nmemb, out->stream);
    }

    /// ���Ȼص�����
    static	int progress_func(void* ptr, double rDlTotal, double rDlNow, double rUploadTotal, double rUploadNow)
    {
        if(CURLDownloadManager::g_totalSize< 0.0001) // ������
        {
            //qDebug("%f %f %f %f %f",100.0 * (rDlNow/rDlTotal),rDlTotal,rDlNow,rUploadTotal,rUploadNow);
            CURLDownloadManager::progressvalue  = 100.0*rDlNow/rDlTotal;
            emit CURLDownloadManager::getThis()->Setvalue(CURLDownloadManager::progressvalue);
            if(CURLDownloadManager::getThis()->isCancel())
                return 1;
            else
                return 0;
        }
        // ���ô���
        //if(!(rDlTotal < 0.0001))
        //    qDebug("%f %f %f %f %f",100.0 * ((rDlNow+(CURLDownloadManager::g_totalSize-rDlTotal))/CURLDownloadManager::g_totalSize),rDlTotal,rDlNow,rUploadTotal,rUploadNow);
        //else
        //    qDebug("%f %f %f %f",rDlTotal,rDlNow,rUploadTotal,rUploadNow);
        //return 0;
    }
    static    CURLDownloadManager * getThis()
    {
        return pthis;
    }
    void setUrl(QString urlstr)
    {
        m_urlStr = urlstr;
        InitTask();
    }

    /// ��ʼ��curl
    int InitTask()
    {
        curl = curl_easy_init();
        m_haveRelease = false;
        return 1;
    }
    /// ��ͣ����
    int PauseTask()
    {
        curl_easy_pause(curl,CURLPAUSE_RECV);
        return 1;
    }

    /// ��������
    int ResumeTask()
    {
        curl_easy_pause(curl,CURLPAUSE_RECV_CONT);
        return 1;
    }

    /// �ͷ���Դ
    int CancelTask()
    {
        m_haveRelease = true;
        return 1;
    }
    /// �����ļ���
    void setSavefileName(QString fileName)
    {
        m_fileName = fileName;
        strcpy(m_fileNameBuffer,m_fileName.toStdString().data());
    }
    /// ȡ������
    bool isCancel()
    {
        return m_haveRelease;
    }
    /// ���������
    bool isBusy()
    {
        return m_busy;
    }
    /// �߳�������
    void ready( bool ready)
    {
        m_ready = ready;
    }
    virtual void run();
    /* �õ�Զ���ļ��Ĵ�С, Ҫ���ص��ļ���С */
    bool getDownloadFileLenth(const char *url)
    {
        CURL *handle = curl_easy_init();
        curl_easy_setopt(handle, CURLOPT_URL, url);
        curl_easy_setopt(handle, CURLOPT_HEADER, 1);    //ֻ��Ҫheaderͷ
        curl_easy_setopt(handle, CURLOPT_NOBODY, 1);    //����Ҫbody
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

    /* �õ������ļ���С�ĺ���, �����������򷵻�0, ���򷵻�ָ��·����ַ���ļ���С */
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
    bool m_haveRelease;
    QString m_urlStr;
    QString m_fileName;
    char m_fileNameBuffer[255];

};
/// ���ô���
static int CurlDebugFunc(CURL *curl, curl_infotype type,
                         char *buffer, size_t size, void *stream)
{
    if(type==CURLINFO_DATA_IN || type == CURLINFO_DATA_OUT)
    {
        return 0;
    }
    qDebug("%s  Len %d",buffer,size);
    return 0;
}

#endif // CURLDOWNLOADMANAGER_H
