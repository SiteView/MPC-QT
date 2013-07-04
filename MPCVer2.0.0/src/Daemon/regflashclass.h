#ifndef REGFLASHCLASS_H
#define REGFLASHCLASS_H

#include <QThread>
#include "./SOAServiceStub.h"


class RegFlashClass : public QThread
{
	Q_OBJECT

public:
	RegFlashClass(QObject *parent);
    virtual void run();
	~RegFlashClass();

private:
	
};

#endif // REGFLASHCLASS_H
