
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
	downloader.setUrl("http://download.cdn.mozilla.net/pub/mozilla.org/firefox/releases/22.0/win32/en-US/Firefox%20Setup%2022.0.exe");
    downloader.setSavefileName("GooglePinyinInstaller.exe");
    downloader.ready(true);
	Sleep(1000);
	downloader.PauseTask();
	Sleep(20000);
	downloader.ResumeTask();
	Sleep(10000);	
	downloader.CancelTask();
	
	QObject::connect(CURLDownloadManager::getThis(),SIGNAL(DownloadFinish(int)),&app, SLOT(quit()));
    int ret;
    ret = app.exec();
    curl_global_cleanup();
    return ret;
}
