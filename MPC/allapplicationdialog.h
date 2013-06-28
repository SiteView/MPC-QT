#ifndef ALLAPPLICATIONDIALOG_H
#define ALLAPPLICATIONDIALOG_H
#include <QMainWindow>
#include <QTableWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QSystemTrayIcon>
#include <QDockWidget>
#include <QClipboard>
#include <QSplitter>
#include <QTreeWidget>
#include <QListWidget>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QCloseEvent>
#include <QMutex>
#include "data.h"
class HttpHandle;
class InformationAnalyze;
class AES;
#include "downloadthread.h"

#include <QWidget>

class AllApplicationDialog : public QWidget
{
    Q_OBJECT
public:
    //explicit AllApplicationDialog(QWidget *parent = 0);
    void setTableInformation();
    void createUpdateFile();
    void createTaskInformationOld();
signals:

public slots:
    void slt_vUpdate();
    //void showThreadMessages(int taskInfoListId, int taskThreadListId, int messagesType, QString strResponseHeader);
    void getFileSize(int taskInfoListId, qint64 fileSize);
    void getFileDownloadFinished(int taskInfoListId, int taskThreadListId);
    void slt_vSetupFinished(int iTaskId);
public slots:

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void OnBtnDownloadClicked();
    void OnBtnUninstallClicked();
    void OnBtnPauseClicked();
    void runTask(int taskInfoListId = -1);
    void runAllTask();
    void showTaskMessages();
    //void newTask(QString urlString = "");

    void threadsDetailTableWidgetClear();
    void stopTask(int m_taskInfoListid = -1);
    void stopAllTask();
    void pauseTask(int m_taskInfoListid = -1);
    void deleteTask();
    void redownloadTask();
    void modifyTask();

    void preferencesTriggered();

    void deleteTaskDownloadThread();
    void quitTriggered();
#if defined(DEBUG)
    void testTriggered();
#endif
    void quitApplication();
    void copyTaskThreadsDetail();
    void switchLanguage(QAction *action);
    void showApplicationNormal();
    void acceptClipboard();
    //void monitorClipboardTriggered();
private:
    void createActions();
    void createTrayIcon();
    void createMenus();
    void createLanguageMenu();
    void createToolBars();
    //void createSpeedWidgetDock();
    void createTaskTableWidget();
    void createTaskGraphPainterWidget();

    void createThreadNumberTreeWidget();
    void createTaskInformation();
    void createSmallWindow();

    void readSettings();
    //void saveAllTask();
    void createTaskTableWidgetContextMenu();
    void createTaskThreadsDetailTableWidgetContextMenu();

    void deleteTaskAndTableWidget(int taskInfoListId);
    int getCurrentTaskInfoListId();
    int getCurrentTaskThreadListId();
    void showTaskState(int taskInfoListId);
    bool newNotDownload(int taskInfoListId);
    void startFinishedThread(int taskInfoListId, int taskThreadListId);
    void saveTaskState(int taskInfoListId);
    void readTaskState(int taskInfoListId);
    void connectTaskThread(int taskInfoListId, int taskThreadListId);
    void showGraphWidget(bool refreshAll = false, int taskInfoListId = -1, int newThreadReceivedListId = -1);
    void showSmallWindow();
    void threadNewReceivedListAppend(int taskInfoListId, int taskThreadListId, int notDownloadListId);
    int getRunningTasksCount();
    void startTaskThread(int taskInfoListId, int taskThreadListId, int notDownloadListId, bool connectTaskThreadBool=false);
    void showActionState();
    void checkNotDownloadList(int taskInfoListId);
    void checkTaskThread(int taskInfoListId);
    int getProxyTitleToId(QString title);
    void restartTask(int taskInfoListId);

    //QMenuBar *menuBar;
      QMenu *trayIconMenu;
    //QMenu *fileMenu;
    //QMenu *taskMenu;
    //QMenu *viewMenu;
    //QMenu *languageSubMenu;
    //QMenu *optionMenu;
    //QMenu *helpMenu;
    //QToolBar *taskToolBar;
    //QActionGroup *languageActionGroup;
    //QAction *quitAct;
    //QAction *addTaskAct;
    //QAction *runTaskAct;
    //QAction *runAllTaskAct;
    //QAction *stopTaskAct;
    //QAction *stopAllTaskAct;
    //QAction *deleteTaskAct;
    //QAction *redownloadTaskAct;
    //QAction *modifyTaskAct;
    //QAction *viewDropWindowAct;
    //QAction *viewSpeedWidgetDockAct;
    //QAction *viewTaskToolBarAct;
    //QAction *preferencesTaskAct;
    //QAction *monitorClipboardAct;
    //QAction *aboutAct;
    //QAction *aboutQtAct;
    //QAction *restoreAct;
    //QAction *hideAct;
    //QAction *copyTaskThreadsDetailAct;
    //QAction *separatorAct;
    //QAction *separatorAct2;
    //QAction *separatorAct3;
    //QAction *testAct;
      QTableWidget *threadsDetailTableWidget;
    //QClipboard *clipboard;
    //QDockWidget *speedWidgetDock;
    //SmallWindow *smallWindow;
    //SpeedGraphWidget *speedGraphWidget;
    QSystemTrayIcon *trayIcon;
    QTableWidget *m_tableWidget;
    QSplitter *taskInfoSplitter;
    //QSplitter *infoDetailSplitter;
    QHBoxLayout *taskGraphDetailLayout;
    QWidget *threadGraphDetailWidget;
    QScrollArea *taskGraphWidget;
    QTreeWidget *threadNumberTreeWidget;
    QTreeWidgetItem *threadInfomationTreeWidgetItem;
    QTreeWidgetItem *threadNumberTreeWidgetItem;
    QTimer *timer;
    QTimer *deleteTaskTimer;
    QTimer *quitApplicationTimer;
    QMutex showThreadMessagesMutex;
    QMutex getNewReceiveDataMutex;
    int retryMax;
    int smallWindowTaskInfoListId;
    int smallWindowShowSeconds;
    HttpHandle *m_pHttpHandle;
    InformationAnalyze *m_pInformationAnalyze;
    AES *m_pAES;
    QList <int> deleteTaskList;
    QString m_sCurrentPath;
    struct _TaskPreferences
    {
        int maxRunningTasksInt;
        int splitInt;
        qint64 minimumSplitSizeInt64;

        bool useLastPathBool;
        bool useDefaultPathBool;
        QString saveToString;

        int connectTimeOutInt;
        int maxRetryNumberInt;
        int retryDelayInt;

        qint64 maxSpeedGraph;
        int maxHeightGraph;
        int pixelWidthGraph;

        qint64 blockSizeGraph;

        bool monitorClipboardBool;
        QString monitorTypesString;
    };
    typedef struct _TaskPreferences TaskPreferences;
    TaskPreferences taskPreferences;
    DownloadThread *downloadThread;
    QList <DownloadThread *> taskThreadList;

    //aaron 0402
    /*struct _NotDownload
    {
        int taskThreadListId;
        qint64 startPosition;
        qint64 endPosition;
    };
    typedef struct _NotDownload NotDownload;*/

    NotDownload *notDownload;

    //aaron 0402
    /*struct _TaskInfomation
    {
        int taskInfoListId;
        int controlState;
        int state;
        QUrl url;
        QStringList urlsStringList;
        bool noProxyBool;
        bool singleProxyBool;
        int singleProxyId;
        bool mutipleProxyBool;
        QList <int> mutipleProxyIdsList;
        QString referrer;
        QString saveTo;
        QString rename;
        QString comment;
        QString user;
        QString password;
        int splite;
        int retry;
        qint64 size;
        qint64 completed;
        qint64 lastCompleted;
        qint64 startPosition;
        qint64 speed;
        qint64 elapsedTime;
        QList <qint64> speedPointList;
        QList <NotDownload *> notDownloadList;
        QList <DownloadThread *> taskThreadList;
        QString applicationName;
        QString silentParam;
        QString currentVersion;
        QString updateVersion;
    };
    typedef struct _TaskInfomation TaskInfomation;*/

    QList <TaskInformation *> m_taskInfoList;
    enum ThreadMessagesType
    {
        uploadMessages = 0,
        downloadMessages,
        infomationMessages,
        errorMessages
    };
    enum tableWidgetHeader
    {
        stateHeader = 0,
        fileNameHeader,
        sizeHeader,
        //compeletedHeader,
        progressHeader,
        speedHeader,
        elapsedTimeHeader,
        timeLeftHeader,
        retryHeader,
        DownloadHeader,
        PauseHeader,
        //commentHeader,
        //0408 aaron uninstall
        UninstallHeader
        //tr("Progress") << tr("Speed")<< tr("Elapsed time")<< tr("Time left") << tr("Retry")  << tr("") << tr("") << tr("Comment");
    };
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

    enum STATUS
    {
        TOTAL = 0,
        NEW,
        NEEDUPDATE,
        NORMAL
    };

    int m_iVersionStatus;
    QVector<QString> versionStatus;
public:
    explicit AllApplicationDialog(QList <TaskInformation *> taskInfoList, QWidget *parent = 0);
    void setTaskInformation(QList <TaskInformation *> &taskInfoList);
};





#endif
