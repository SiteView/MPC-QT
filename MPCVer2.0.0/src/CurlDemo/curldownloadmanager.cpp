#include "curldownloadmanager.h"


double CURLDownloadManager::g_totalSize = 0;
QProgressBar* CURLDownloadManager::bar = 0;

CURLDownloadManager::CURLDownloadManager(QObject *parent)
	: QThread(parent)
{
	m_ready = false;
}

CURLDownloadManager::~CURLDownloadManager()
{
	
}
void CURLDownloadManager::run()
{
   forever
   {
	  if(!m_ready)
	  {
		  msleep(200);
		  continue;
	  }
	  m_ready =!m_ready;
	  //getDownloadFileLenth(m_urlStr.toStdString().data());
	  CURL *curl;
	  CURLcode res;
	  struct FtpFile ftpfile={
		  m_fileNameBuffer, /* name to store the file as if succesful */
		  NULL
	  };
	  //char localFileLenthStr[255] ={0}; 
	  //unsigned long filesize = g_totalSize;
	  //sprintf(localFileLenthStr, "%ld", filesize);  

	  //CURLM *multi_handle = NULL;

	  //multi_handle = curl_multi_init();

	  curl = curl_easy_init();

	  if(curl) {
		  curl_easy_setopt(curl, CURLOPT_URL, m_urlStr.toStdString().data());
		  /* Define our callback to get called when there's data to be written */
		  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
		  /* Set a pointer to our struct to pass to the callback */
		  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);

		  /* Switch on full protocol/debug output */
		  curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);


		  //curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);

		  //curl的进度条声明
		  curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);

		  //回调进度条函数
		  curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_func);

		  //curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, localFileLenthStr);

		  //curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, 0); 
		  //curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, getLocalFileLenth(ftpfile.filename)); 

		  res = curl_easy_perform(curl);

		  curl_easy_cleanup(curl);

		  if(CURLE_OK != res) {
			  qDebug("curl download error %d", res);
		  }
	  }
	  else
	  {
		  qDebug( "create download object failed！", res);
	  }

	  //curl_multi_add_handle(multi_handle, curl);
	  //int running_handle_count;
	  //while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multi_handle, &running_handle_count))
	  //{
	  //	qDebug("%d",running_handle_count) ;
	  //}
	  //while (running_handle_count)
	  //{
	  //	timeval tv;
	  //	tv.tv_sec = 3;
	  //	tv.tv_usec = 0;

	  //	int max_fd;
	  //	fd_set fd_read;
	  //	fd_set fd_write;
	  //	fd_set fd_except;

	  //	FD_ZERO(&fd_read);
	  //	FD_ZERO(&fd_write);
	  //	FD_ZERO(&fd_except);
	  //	CURLMcode resm ;
	  //	resm = curl_multi_fdset(multi_handle, &fd_read, &fd_write, &fd_except, &max_fd);
	  //	int return_code = select(max_fd + 1, &fd_read, &fd_write, &fd_except, &tv);
	  //	if (-1 == return_code)
	  //	{
	  //		qDebug("select error.");
	  //		break;
	  //	}
	  //	else
	  //	{
	  //		while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multi_handle, &running_handle_count))
	  //		{
	  //			qDebug("%d",running_handle_count) ;
	  //		}
	  //	}
	  //}

	  if(ftpfile.stream)
		  fclose(ftpfile.stream); /* close the local file */
	  
	  //curl_easy_cleanup(curl);
	  //curl_multi_cleanup(multi_handle);
}
}




