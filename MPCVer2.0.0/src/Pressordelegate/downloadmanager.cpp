#include "downloadmanager.h"

//QProgressBar* downloadmanager::bar = 0;

downloadmanager::downloadmanager(QObject *parent) :
    QThread(parent)
{
}
void  downloadmanager::run()
{
    int value = 0;
	forever
    {
        msleep(200);
		if(value >100)
			value = 0;
		else
			value++;
		emit sendValue(value);
    }
}
