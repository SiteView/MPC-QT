
#include <QApplication>


#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include <WinSock2.h >
#include <QFileInfo>
#include "curldownloadmanager.h"

int main(int argc, char *argv[])
{
  	QApplication app(argc, argv);
	app.setQuitOnLastWindowClosed(false);
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURLDownloadManager downloader(&app);
    downloader.start();
	downloader.setUrl("http://m.weiguanli.cn/setup/%E5%BE%AE%E7%AE%A1%E7%90%86-setup-v1.0.exe");
    downloader.setSavefileName("GooglePinyinInstaller.exe");
    downloader.ready(true);
	QObject::connect(CURLDownloadManager::getThis(),SIGNAL(DownloadFinish(int)),&app, SLOT(quit()));
    int ret;
    ret = app.exec();
    curl_global_cleanup();
    return ret;
}
