
#include <QtCore/QCoreApplication>


#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include <WinSock2.h >
#include <QFileInfo>
#include "curldownloadmanager.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	curl_global_init(CURL_GLOBAL_DEFAULT);
	CURLDownloadManager downloader(&a);
	downloader.start();
	downloader.setUrl("http://dl.google.com/pinyin/v2/GooglePinyinInstaller.exe");
	downloader.setSavefileName("c:/GooglePinyinInstaller.exe");
	downloader.ready(true);

	int ret;	
	ret = a.exec();
	curl_global_cleanup();
	return ret;
}
