#include "downloadmanager.h"

//QProgressBar* downloadmanager::bar = 0;

downloadmanager::downloadmanager(QObject *parent) :
    QThread(parent)
{
}
void  downloadmanager::run()
{
    double value = 0;
	forever
    {
        msleep(30);
		if(value >100)
			value = 0;
		else
			value  = value + qrand()%100*0.001;
		emit sendValue(value);
    }
}
