
#include <QtCore/QCoreApplication>


#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>


double g_totalSize;

struct FtpFile {
	const char *filename;
	FILE *stream;
};

static int my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
	struct FtpFile *out=(struct FtpFile *)stream;
	if(out && !out->stream) {
		/* open file for writing */
		out->stream=fopen(out->filename, "ab");
		if(!out->stream)
			return -1; /* failure, can't open file to write */
	}
	return fwrite(buffer, size, nmemb, out->stream);
}

int progress_func(void* ptr, double rDlTotal, double rDlNow, double rUploadTotal, double rUploadNow)
{
	if(!(rDlTotal < 0.0001))
		printf("%f %f %f %f %f \n",100.0 * ((rDlNow+(g_totalSize-rDlTotal))/g_totalSize),rDlTotal,rDlNow,rUploadTotal,rUploadNow);
	else
		printf("%f %f %f %f \n",rDlTotal,rDlNow,rUploadTotal,rUploadNow);
	return 0;
}

/* 得到远程文件的大小, 要下载的文件大小 */  
bool getDownloadFileLenth(const char *url){  
	CURL *handle = curl_easy_init();  
	curl_easy_setopt(handle, CURLOPT_URL, url);  
	curl_easy_setopt(handle, CURLOPT_HEADER, 1);    //只需要header头   
	curl_easy_setopt(handle, CURLOPT_NOBODY, 1);    //不需要body 
	if (curl_easy_perform(handle) == CURLE_OK) {  
		curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &g_totalSize);  
	} else {  
		return false;  
	}  
	curl_easy_cleanup(handle);
	printf("%f",g_totalSize);
	return true;  
}  

/* 得到本地文件大小的函数, 若不是续传则返回0, 否则返回指定路径地址的文件大小 */  
long getLocalFileLenth(const char* localPath){  
	if (false){  
		return 0;  
	} 
	FILE * pFile;
	long lSize;
	size_t result;
	pFile = fopen ( localPath, "rb" );
	if (pFile==NULL) {return 0;}
	// 获得文件大小
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	fclose(pFile);
	return lSize;  
} 

int main(int argc, char *argv[])
{
	//QCoreApplication a(argc, argv);
	getDownloadFileLenth("http://m.weiguanli.cn/setup/%E5%BE%AE%E7%AE%A1%E7%90%86-setup-v1.0.exe");
	CURL *curl;
	CURLcode res;
	struct FtpFile ftpfile={
		"gsoap.zip", /* name to store the file as if succesful */
		NULL
	};

	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL,
			"http://m.weiguanli.cn/setup/%E5%BE%AE%E7%AE%A1%E7%90%86-setup-v1.0.exe");

		/* Define our callback to get called when there's data to be written */
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
		/* Set a pointer to our struct to pass to the callback */
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);

		/* Switch on full protocol/debug output */
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);

		//curl的进度条声明
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
		//回调进度条函数
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_func);

		//设置进度条名称
		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, "ok");


		curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, getLocalFileLenth(ftpfile.filename)); 

		res = curl_easy_perform(curl);

		/* always cleanup */
		curl_easy_cleanup(curl);

		if(CURLE_OK != res) {
			/* we failed */
			fprintf(stderr, "curl told us %d\n", res);
		}
	}
	else
	{
		fprintf(stderr, "create download object failed！", res);
	}

	if(ftpfile.stream)
		fclose(ftpfile.stream); /* close the local file */
	curl_global_cleanup();
	//return a.exec();
}
