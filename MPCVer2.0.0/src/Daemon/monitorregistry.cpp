#include "monitorregistry.h"

#include <QDebug>

MonitorRegistry::MonitorRegistry(QObject *parent) :
    QObject(parent)
{
}

void WINAPI MonitorRegistry::MonitorReg()
{
    while(TRUE)
    {
        DWORD  dwFilter = REG_NOTIFY_CHANGE_NAME |
                REG_NOTIFY_CHANGE_ATTRIBUTES |
                REG_NOTIFY_CHANGE_LAST_SET |
                REG_NOTIFY_CHANGE_SECURITY;

        HANDLE hEvent;
        HKEY   hMainKey;
        HKEY   hKey;
        LONG   lErrorCode;

        hMainKey = HKEY_LOCAL_MACHINE;

        LPCTSTR lpSubKey = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall");

        // Open a key.

        lErrorCode = RegOpenKeyEx(hMainKey, lpSubKey, 0, KEY_NOTIFY, &hKey);

        if (lErrorCode != ERROR_SUCCESS)
        {	
            _tprintf(TEXT("Error in RegOpenKeyEx (%d).\n"), lErrorCode);

            return;
        }

        // Create an event.

        hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

        if (hEvent == NULL)
        {
            _tprintf(TEXT("Error in CreateEvent (%d).\n"), GetLastError());

            return;
        }



        // Watch the registry key for a change of value.

        lErrorCode = RegNotifyChangeKeyValue(hKey,
                                             TRUE,
                                             dwFilter,
                                             hEvent,
                                             TRUE);

        if (lErrorCode != ERROR_SUCCESS)
        {
            _tprintf(TEXT("Error in RegNotifyChangeKeyValue (%d).\n"), lErrorCode);

            return;
        }

        // Wait for an event to occur.

        _tprintf(TEXT("Waiting for a change in the specified key...\n"));

        if (WaitForSingleObject(hEvent, INFINITE) == WAIT_FAILED)
        {
            _tprintf(TEXT("Error in WaitForSingleObject (%d).\n"), GetLastError());

            return;
        }
        else
        {
		    qDebug() << "Change has occurred";
            _tprintf(TEXT("\nChange has occurred.\n"));

            // start your function

            // download app must have litte time, so sleep(timeOut)?

            emit registryChanged();

        }

        // Close the key.

        lErrorCode = RegCloseKey(hKey);

        if (lErrorCode != ERROR_SUCCESS)
        {
            _tprintf(TEXT("Error in RegCloseKey (%d).\n"), GetLastError());

            return;
        }

        // Close the handle.

        if (!CloseHandle(hEvent))
        {
            _tprintf(TEXT("Error in CloseHandle.\n"));

            return;
        }
    }

    return;
}

