#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <QtCore/QCoreApplication>
#include <QString>
#include <QDebug>

// Service Name
#define SVCNAME TEXT("NetMonitor")

SERVICE_STATUS          gSvcStatus;
SERVICE_STATUS_HANDLE   gSvcStatusHandle;

VOID SvcInstall(void);
VOID SvcUnInstall(void);
VOID WINAPI SvcCtrlHandler(DWORD);
VOID WINAPI SvcMain(int, char**);

VOID ReportSvcStatus(int, int, int);
VOID SvcInit(int, char**);

int main(int argc, char *argv[])
{

    if(argc == 2)
    {
        if((strcmp(argv[1], "--install") == 0)||(strcmp(argv[1], "-i") == 0))
        {
            SvcInstall();
            return 0;
        }
        else if((strcmp(argv[1], "--uninstall") == 0)||(strcmp(argv[1], "-u") == 0))
        {
            SvcUnInstall();
            return 0;
        }
    }

    SERVICE_TABLE_ENTRY DispatchTable[] =
    {
        { SVCNAME, (LPSERVICE_MAIN_FUNCTION)SvcMain },
        { NULL, NULL }
    };

    if (!StartServiceCtrlDispatcher(DispatchTable))
    {
        // God Error.
    }
    return 0;
}

VOID SvcInstall()
{
    SC_HANDLE schSCManager;
    SC_HANDLE schService;
    TCHAR szPath[MAX_PATH];

    if(!GetModuleFileName(NULL, szPath, MAX_PATH))
    {
        // TODO: LOG
        printf("Cannot install service (%d)\n", (int)GetLastError());
        return;
    }

    // Open the local default service control manager database
    schSCManager = OpenSCManager(
                NULL,                    // local computer
                NULL,                    // ServicesActive database
                SC_MANAGER_ALL_ACCESS);  // full access rights

    if (NULL == schSCManager)
    {
        printf("OpenSCManager failed (%d)\n", (int)GetLastError());
        return;
    }

    // Install the service into SCM by calling CreateService
    schService = CreateService(
                schSCManager,              // SCM database
                SVCNAME,                   // name of service
                SVCNAME,                   // service name to display
                SERVICE_ALL_ACCESS,        // desired access
                SERVICE_WIN32_OWN_PROCESS, // service type
                SERVICE_AUTO_START,        // start type
                SERVICE_ERROR_NORMAL,      // error control type
                szPath,                    // path to service's binary
                NULL,                      // no load ordering group
                NULL,                      // no tag identifier
                NULL,                      // no dependencies
                NULL,                      // LocalSystem account
                NULL);                     // no password

    if (schService == NULL)
    {
        printf("CreateService failed (%d)\n", (int)GetLastError());
        CloseServiceHandle(schSCManager);
        return;
    }
    else printf("Service installed successfully\n");

    // Centralized cleanup for all allocated resources.
    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
}

VOID SvcUnInstall()
{
    SC_HANDLE schSCManager;
    SC_HANDLE schService;

    // Open the local default service control manager database
    schSCManager = OpenSCManager(
                NULL,                    // local computer
                NULL,                    // ServicesActive database
                SC_MANAGER_ALL_ACCESS);  // full access rights

    if (NULL == schSCManager)
    {
        printf("OpenSCManager failed (%d)\n", (int)GetLastError());
        return;
    }

    // Open the service with delete, stop, and query status permissions
    schService = OpenService(schSCManager, SVCNAME, SC_MANAGER_ALL_ACCESS);
    if(schService == NULL)
    {
        printf("OpenService failed (%d)\n", (int)GetLastError());
        CloseServiceHandle(schSCManager);
        return;
    }

    // Try to stop the service
    // TODO:

    // Now remove the service by calling DeleteService.
    if(!DeleteService(schService))
    {
        printf("DeleteService failed (%d)\n", (int)GetLastError());
        return;
    }
    else
        printf("Service uninstall successfully\n");

    // Centralized cleanup for all allocated resources.
    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
}

VOID WINAPI SvcMain(int dwArgc, char** lpszArgv)
{
    gSvcStatusHandle = RegisterServiceCtrlHandler(
                SVCNAME,
                SvcCtrlHandler);

    if(!gSvcStatusHandle)
    {
        return;
    }

    gSvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    gSvcStatus.dwServiceSpecificExitCode = 0;

    ReportSvcStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

    SvcInit(dwArgc, lpszArgv);
}

VOID SvcInit(int argc, char** argv)
{
    QCoreApplication coreApp(argc, argv);
    wchar_t fpath[260];
    GetModuleFileName(NULL, fpath, 260);

    memset(fpath, 0, sizeof(fpath));
    QString appFilePath = QString((const QChar*)fpath);
    appFilePath.remove(appFilePath.lastIndexOf("\\"), appFilePath.size()-appFilePath.lastIndexOf("/"));
    QCoreApplication::addLibraryPath(appFilePath+"/plugins");

    serverConfig = new ServerConfig(&coreApp);
    serverConfig->Load(appFilePath+"\\serverConfig");
    if(serverConfig->GetServerAddress() != "0.0.0.0")
    {
        ReportSvcStatus(SERVICE_RUNNING, NO_ERROR, 0);
        delete serverConfig;
        ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);
        return;
    }

    netMonitorServer = new NetMonitorServer(serverConfig, &coreApp);
    netMonitorServer->start();

    ReportSvcStatus(SERVICE_RUNNING, NO_ERROR, 0);

    // TO_DO: Perform work until service stops.

    coreApp.exec();
    delete netMonitorServer;
    delete serverConfig;
    ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);
}

VOID ReportSvcStatus(int dwCurrentState,
                     int dwWin32ExitCode,
                     int dwWaitHint)
{
    static int dwCheckPoint = 1;

    // Fill in the SERVICE_STATUS structure of the service.
    gSvcStatus.dwCurrentState = dwCurrentState;
    gSvcStatus.dwWin32ExitCode = dwWin32ExitCode;
    gSvcStatus.dwWaitHint = dwWaitHint;

    if (dwCurrentState == SERVICE_START_PENDING)
        gSvcStatus.dwControlsAccepted = 0;
    else gSvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

    if ((dwCurrentState == SERVICE_RUNNING) ||
            (dwCurrentState == SERVICE_STOPPED))
        gSvcStatus.dwCheckPoint = 0;
    else gSvcStatus.dwCheckPoint = dwCheckPoint++;

    // Report the status of the service to the SCM.
    SetServiceStatus(gSvcStatusHandle, &gSvcStatus);
}

VOID WINAPI SvcCtrlHandler(DWORD dwCtrl)
{
    switch(dwCtrl)
    {
    case SERVICE_CONTROL_STOP:   // SCM STOP SIGNAL
        ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);
        //netMonitorServer->stop();
        QCoreApplication::quit();
        ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);
        return;
    case SERVICE_CONTROL_INTERROGATE:
        break;
    default:
        break;
    }
    //ReportSvcStatus(gSvcStatus.dwCurrentState, NO_ERROR, 0);
}

