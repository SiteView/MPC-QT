#ifndef MONITORREGISTRY_H
#define MONITORREGISTRY_H

#include <QObject>

#include <windows.h>
#include <tchar.h>
#include <stdio.h>


class MonitorRegistry : public QObject
{
    Q_OBJECT
public:
    explicit MonitorRegistry(QObject *parent = 0);

signals:
    void registryChanged();

public slots:

public:
    void WINAPI MonitorReg();

};

#endif // MONITORREGISTRY_H
