#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QThread>
#include <QProgressBar>

class downloadmanager : public QThread
{
    Q_OBJECT
public:
    explicit downloadmanager(QObject *parent = 0);
       virtual void run() ;

signals:
	  void sendValue(int value);
    
public slots:


};

#endif // DOWNLOADMANAGER_H
