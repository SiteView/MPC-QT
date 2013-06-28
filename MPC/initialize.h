#ifndef INITIALIZE_H
#define INITIALIZE_H
#include "aes.h"
#include "informationanalyze.h"
#include <QObject>
#include "data.h"
class AES;
class InformationAnalyze;
class HttpHandle;
class InitializeInfo : public QObject
{
    Q_OBJECT
public:
    explicit InitializeInfo(QObject *parent = 0);
    void readTaskState(int taskInfoListId);
    void createUpdateFile();

    QList <TaskInformation *> GetTaskInformation();
    QList <TaskInformation *> GetNewTaskInformation();
    QList <TaskInformation *> GetUpdateTaskInformation();
    QList <TaskInformation *> GetNormalTaskInformation();
    void vUpdate();
private:
    InformationAnalyze * m_pInformationAnalyze;
    //QList <DownloadThread *> taskThreadList;
    NotDownload *g_notDownload;
    QList <TaskInformation *> g_updateTaskInfoList;
    QList <TaskInformation *> g_newTaskInfoList;
    QList <TaskInformation *> g_normalTaskInfoList;
    QList <TaskInformation *> g_taskInfoList;
    HttpHandle *m_pHttpHandle;
    AES *m_pAES;
    void createTaskInformation();
    bool downloadfinish;
    enum DownloadTaskState
    {
        noState = -1,
        //aaron
        doneState = 0,
        stopState,
        runningState,
        finishedState,
        pauseState,
        errorState,
        installingState
    };
signals:
    void sig_vUpdate();
    
public slots:
    void slt_vUpdate();

    
};

#endif // INITIALIZE_H
