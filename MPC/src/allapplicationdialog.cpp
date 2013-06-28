#include "allapplicationdialog.h"

#include "httphandle.h"
#include "informationanalyze.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QDateTime>
#include <QMessageBox>
#include <QThread>
#include <QDialog>
#include <QDebug>
#include <QPushButton>
#include "AllApplicationDialog.h"
#include "httpdownload.h"
#include "tools.h"

#define SERVERINI "d:/server.ini"
#define LOCALINI "d:/local.ini"
#define UPDATEINI "d:/update.ini"
#define SERVER "http://127.0.0.1/gshy/server.ini"

#ifdef WIN32
#include <windows.h>
#endif

#include <QProgressBar>
#define UPDATEFILE "D:/update.ini"

AllApplicationDialog::AllApplicationDialog(QWidget *parent) :
    QWidget(parent)
{
    QString language = tr("English");	//Translate English into your own language
    createUpdateFile();

    createTaskTableWidget();
    createThreadNumberTreeWidget();

    //aaron 0326
    //createTaskInformation();
    m_tableWidget->show();

    deleteTaskTimer = 0;

    quitApplicationTimer = 0;

    readSettings();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTaskMessages()) );
    timer->start(1000);
}

void AllApplicationDialog::createUpdateFile()
{
    //下载SERVERINI文件
    m_pHttpHandle = new HttpHandle(QUrl(SERVER), "D:/");
    m_pHttpHandle->vDownloadFile();

    m_pInformationAnalyze = new InformationAnalyze(SERVERINI, LOCALINI, UPDATEINI);
    //m_pInformationAnalyze->vInitLocalInfomation();

    //在下载安装完毕后更新显示
    connect(m_pHttpHandle, SIGNAL(sig_vSetupfinish()), this, SLOT(slt_vUpdate()));
}

void AllApplicationDialog::slt_vUpdate()
{
    //qDebug() << "update";
    m_pInformationAnalyze->vRefresh();
    createTaskInformation();
}
void AllApplicationDialog::createActions()
{

}
void AllApplicationDialog::createTrayIcon()
{


}
void AllApplicationDialog::createMenus()
{

}
void AllApplicationDialog::createLanguageMenu()
{

}
void AllApplicationDialog::createToolBars()
{

}

void AllApplicationDialog::createSmallWindow()
{

}
/*void AllApplicationDialog::createSpeedWidgetDock()
{

}*/

void AllApplicationDialog::createTaskTableWidget()
{
    m_tableWidget = new QTableWidget(this);
    m_tableWidget->setColumnCount(11);
    m_tableWidget->setRowCount(0);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWidget->resize(800,600);
    QStringList tableHeaders;
    tableHeaders << tr("State") << tr("File name") << tr("Size") << tr("Progress") << tr("Speed")<< tr("Elapsed time")<< tr("Time left") << tr("Retry")  << tr("") << tr("") << tr("Comment");
    m_tableWidget->setHorizontalHeaderLabels(tableHeaders);
    m_tableWidget->verticalHeader()->hide();
    m_tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    m_tableWidget->horizontalHeader()->setHighlightSections(false);
    m_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_tableWidget->setFocusPolicy(Qt::NoFocus);
    QList <int> columnWidths;
    columnWidths<<40<<80<<80<<80<<70<<70<<70<<70<<70<<50<<50;
    for (int i=0;i<columnWidths.size();i++)
    {
        m_tableWidget->setColumnWidth(i,columnWidths.at(i));
    }

    //aaron
    //connect(m_tableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(taskTableWidgetSelectionChanged()) );
    //connect(m_tableWidget, SIGNAL(cellDoubleClicked(int , int)), this, SLOT(taskTableWidgetDoubleClicked(int , int)) );
}
void AllApplicationDialog::createThreadNumberTreeWidget()
{

}

void AllApplicationDialog::setTaskInformation(QList <TaskInfomation *> &taskInfoList)
{
    m_taskInfoList = taskInfoList;
}

void AllApplicationDialog::setTableInformation()
{
    //qDebug() <<  "set table information";
    for(int taskInfoListId = 0; taskInfoListId < m_taskInfoList.size(); taskInfoListId++)
    {
        //qDebug() << m_taskInfoList.at(taskInfoListId)->size;

        int row = m_tableWidget->rowCount();
        m_tableWidget->insertRow(row);
        if (m_taskInfoList.at(taskInfoListId)->state == runningState)
            m_taskInfoList.at(taskInfoListId)->state = stopState;
        showTaskState(taskInfoListId);
        //aaron
        m_tableWidget->setItem(taskInfoListId, fileNameHeader, new QTableWidgetItem(m_taskInfoList.at(taskInfoListId)->applicationName));
        m_tableWidget->setItem(taskInfoListId, sizeHeader, new QTableWidgetItem(QString("%1").arg(m_taskInfoList.at(taskInfoListId)->size)));
        //m_tableWidget->setItem(taskInfoListId, compeletedHeader, new QTableWidgetItem(QString("%1").arg(taskInfoList.at(taskInfoListId)->completed)));
        float percent = 0;
        if (m_taskInfoList.at(taskInfoListId)->size != 0)
            percent=float(m_taskInfoList.at(taskInfoListId)->completed)/float(m_taskInfoList.at(taskInfoListId)->size)*100;
        //m_tableWidget->setItem(taskInfoListId, progressHeader, new QTableWidgetItem(QString("%1").arg(percent,0,'f',2)));
        //m_tableWidget->setItem(taskInfoListId, urlHeader, new QTableWidgetItem(taskInfoList.at(taskInfoListId)->url.toString()));

        m_tableWidget->setItem(taskInfoListId, commentHeader, new QTableWidgetItem(m_taskInfoList.at(taskInfoListId)->comment));



        //aaron
        if((percent > 0) && (percent < 100))
        {
            QProgressBar *l_pProgress = new QProgressBar(this);
            m_tableWidget->setCellWidget(taskInfoListId, progressHeader, l_pProgress);
            l_pProgress->setRange(0,100);
            l_pProgress->setValue(percent);
        }
        //else
        //{
        //    l_pProgress->hide();
        //}


        QPushButton *l_pbDownload = new QPushButton("Download");
        m_tableWidget->setCellWidget(taskInfoListId, DownloadHeader, l_pbDownload);
        connect(l_pbDownload, SIGNAL(clicked()),this,SLOT(OnBtnDownloadClicked()));

        QPushButton *l_pbPause = new QPushButton("Pause");
        m_tableWidget->setCellWidget(taskInfoListId, PauseHeader, l_pbPause);
        connect(l_pbPause, SIGNAL(clicked()),this,SLOT(OnBtnPauseClicked()));
    }

    //aaron 0326
    //timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(showTaskMessages()) );
    //timer->start(1000);
}
void AllApplicationDialog::createTaskInformation()
{
    m_taskInfoList.clear();
    int taskInfoListId = 0;

    //aaron
    QSettings l_RegSettings("MPC","task");

    //settings.beginGroup(QString("task%1").arg(taskInfoListId));
    l_RegSettings.beginGroup(QString("task"));

    //QString urlString = l_RegSettings.value("url").toString();
    QString referrerString  = l_RegSettings.value("referrer").toString();
    QString saveToString = l_RegSettings.value("saveTo").toString();
    //QString renameString = l_RegSettings.value("rename").toString();

    //aaron 以下配置暂未使用
    int splitInt = l_RegSettings.value("split").toInt();
    QString userString = l_RegSettings.value("user").toString();
    QString passwordString = l_RegSettings.value("password").toString();
    QString commentString = l_RegSettings.value("comment").toString();
    bool noProxyBool = l_RegSettings.value("noProxy", true).toBool();
    bool singleProxyBool = l_RegSettings.value("singleProxy", false).toBool();
    int singleProxyId = l_RegSettings.value("singleProxyId", -1).toInt();
    bool mutipleProxyBool = l_RegSettings.value("mutipleProxy", false).toBool();
    QList <QVariant> mutipleProxyIdsVariant = l_RegSettings.value("mutipleProxyIds").toList();;
    QList <QVariant> urlsListVariant = l_RegSettings.value("urls").toList();
    l_RegSettings.endGroup();

    //UPDATEFILE需要做到设置里
    QSettings l_psetUpdate(UPDATEFILE, QSettings::IniFormat) ;
    QString l_sUpdateSoftware = l_psetUpdate.value("/LIST/Software").toString();
    QStringList l_slSoftwareList = l_sUpdateSoftware.split("|");

    //qDebug() << l_sUpdateSoftware;

    for (int i = 0; i < l_slSoftwareList.size(); ++i)
    {

        QString l_sSoftwareName = l_slSoftwareList.at(i);
        QString urlString = l_psetUpdate.value(l_sSoftwareName+"/url").toString();

        QFileInfo fileInfo(QUrl(urlString).path());
        QString fileName = fileInfo.fileName();

        if (urlString.isEmpty()) break;

        TaskInfomation *taskInfo;

        taskInfo = new TaskInfomation;
        taskInfo->applicationName = l_sSoftwareName;
        taskInfo->taskInfoListId = taskInfoListId;
        taskInfo->controlState = noState;
        taskInfo->state = stopState;
        taskInfo->url.setUrl(urlString);
        taskInfo->referrer = referrerString;
        taskInfo->saveTo = saveToString;
        taskInfo->rename = fileName;
        taskInfo->comment = commentString;
        taskInfo->splite = splitInt;
        taskInfo->retry = 0;
        taskInfo->size = 0;
        taskInfo->completed = 0;
        taskInfo->lastCompleted = 0;
        taskInfo->speed = 0;
        taskInfo->elapsedTime = 0;
        taskInfo->noProxyBool = noProxyBool;
        taskInfo->singleProxyBool = singleProxyBool;
        taskInfo->singleProxyId = singleProxyId;
        taskInfo->mutipleProxyBool = mutipleProxyBool;
        foreach( QVariant mutipleProxyId, mutipleProxyIdsVariant )
            taskInfo->mutipleProxyIdsList<<mutipleProxyId.toInt();
        foreach( QVariant url, urlsListVariant )
            taskInfo->urlsStringList<<url.toString();

        m_taskInfoList.append(taskInfo);

        readTaskState(taskInfoListId);

        qint64 taskInfoListIdNotCompleted = 0;
        for (int notDownloadListId=0;notDownloadListId<m_taskInfoList.at(taskInfoListId)->notDownloadList.size();notDownloadListId++)
        {
            taskInfoListIdNotCompleted += m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->endPosition - m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->startPosition;
        }
        m_taskInfoList.at(taskInfoListId)->completed = m_taskInfoList.at(taskInfoListId)->size - taskInfoListIdNotCompleted;
        m_taskInfoList.at(taskInfoListId)->lastCompleted = m_taskInfoList.at(taskInfoListId)->completed;
        //qDebug() << m_taskInfoList.at(taskInfoListId)->size << "what's wrong?";
        taskInfoListId++;
    }

    setTableInformation();
}
void AllApplicationDialog::createTaskInformationOld()
{
    m_taskInfoList.clear();
    int taskInfoListId = 0;

    //aaron
    QSettings l_RegSettings("MPC","task");

    //settings.beginGroup(QString("task%1").arg(taskInfoListId));
    l_RegSettings.beginGroup(QString("task"));

    //QString urlString = l_RegSettings.value("url").toString();
    QString referrerString  = l_RegSettings.value("referrer").toString();
    QString saveToString = l_RegSettings.value("saveTo").toString();
    //QString renameString = l_RegSettings.value("rename").toString();

    //aaron 以下配置暂未使用
    int splitInt = l_RegSettings.value("split").toInt();
    QString userString = l_RegSettings.value("user").toString();
    QString passwordString = l_RegSettings.value("password").toString();
    QString commentString = l_RegSettings.value("comment").toString();
    bool noProxyBool = l_RegSettings.value("noProxy", true).toBool();
    bool singleProxyBool = l_RegSettings.value("singleProxy", false).toBool();
    int singleProxyId = l_RegSettings.value("singleProxyId", -1).toInt();
    bool mutipleProxyBool = l_RegSettings.value("mutipleProxy", false).toBool();
    QList <QVariant> mutipleProxyIdsVariant = l_RegSettings.value("mutipleProxyIds").toList();;
    QList <QVariant> urlsListVariant = l_RegSettings.value("urls").toList();
    l_RegSettings.endGroup();

    //UPDATEFILE需要做到设置里
    QSettings l_psetUpdate(UPDATEFILE, QSettings::IniFormat) ;
    QString l_sUpdateSoftware = l_psetUpdate.value("/LIST/Software").toString();
    QStringList l_slSoftwareList = l_sUpdateSoftware.split("|");

    //qDebug() << l_sUpdateSoftware;

    for (int i = 0; i < l_slSoftwareList.size(); ++i)
    {

        QString l_sSoftwareName = l_slSoftwareList.at(i);
        QString urlString = l_psetUpdate.value(l_sSoftwareName+"/url").toString();

        QFileInfo fileInfo(QUrl(urlString).path());
        QString fileName = fileInfo.fileName();

        if (urlString.isEmpty()) break;

        TaskInfomation *taskInfo;
        taskInfo = new TaskInfomation;
        taskInfo->taskInfoListId = taskInfoListId;
        taskInfo->controlState = noState;
        taskInfo->state = stopState;
        taskInfo->url.setUrl(urlString);
        taskInfo->referrer = referrerString;
        taskInfo->saveTo = saveToString;
        taskInfo->rename = fileName;
        taskInfo->comment = commentString;
        taskInfo->splite = splitInt;
        taskInfo->retry = 0;
        taskInfo->size = 0;
        taskInfo->completed = 0;
        taskInfo->lastCompleted = 0;
        taskInfo->speed = 0;
        taskInfo->elapsedTime = 0;
        taskInfo->noProxyBool = noProxyBool;
        taskInfo->singleProxyBool = singleProxyBool;
        taskInfo->singleProxyId = singleProxyId;
        taskInfo->mutipleProxyBool = mutipleProxyBool;
        foreach( QVariant mutipleProxyId, mutipleProxyIdsVariant )
            taskInfo->mutipleProxyIdsList<<mutipleProxyId.toInt();
        foreach( QVariant url, urlsListVariant )
            taskInfo->urlsStringList<<url.toString();

        m_taskInfoList.append(taskInfo);

        readTaskState(taskInfoListId);

        qint64 taskInfoListIdNotCompleted = 0;
        for (int notDownloadListId=0;notDownloadListId<m_taskInfoList.at(taskInfoListId)->notDownloadList.size();notDownloadListId++)
        {
            taskInfoListIdNotCompleted += m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->endPosition - m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->startPosition;
        }
        m_taskInfoList.at(taskInfoListId)->completed = m_taskInfoList.at(taskInfoListId)->size - taskInfoListIdNotCompleted;
        m_taskInfoList.at(taskInfoListId)->lastCompleted = m_taskInfoList.at(taskInfoListId)->completed;

        int row = m_tableWidget->rowCount();
        m_tableWidget->insertRow(row);
        if (m_taskInfoList.at(taskInfoListId)->state == runningState)
            m_taskInfoList.at(taskInfoListId)->state = stopState;
        showTaskState(taskInfoListId);
        //aaron
        m_tableWidget->setItem(taskInfoListId, fileNameHeader, new QTableWidgetItem(m_taskInfoList.at(taskInfoListId)->applicationName));
        m_tableWidget->setItem(taskInfoListId, sizeHeader, new QTableWidgetItem(QString("%1").arg(m_taskInfoList.at(taskInfoListId)->size)));
        //m_tableWidget->setItem(taskInfoListId, compeletedHeader, new QTableWidgetItem(QString("%1").arg(taskInfoList.at(taskInfoListId)->completed)));
        float percent = 0;
        if (m_taskInfoList.at(taskInfoListId)->size != 0)
            percent=float(m_taskInfoList.at(taskInfoListId)->completed)/float(m_taskInfoList.at(taskInfoListId)->size)*100;
        //m_tableWidget->setItem(taskInfoListId, progressHeader, new QTableWidgetItem(QString("%1").arg(percent,0,'f',2)));
        //m_tableWidget->setItem(taskInfoListId, urlHeader, new QTableWidgetItem(taskInfoList.at(taskInfoListId)->url.toString()));

        m_tableWidget->setItem(taskInfoListId, commentHeader, new QTableWidgetItem(m_taskInfoList.at(taskInfoListId)->comment));

        //aaron
        /*if((percent > 0) && (percent < 100))
        {
            QProgressBar *l_pProgress = new QProgressBar(this);
            l_pProgress->setRange(0,100);

            m_tableWidget->setCellWidget(taskInfoListId, compeletedHeader, l_pProgress);
            l_pProgress->setValue(percent);

        }*/


        QPushButton *l_pbDownload = new QPushButton("Download");
        m_tableWidget->setCellWidget(taskInfoListId, retryHeader, l_pbDownload);
        connect(l_pbDownload, SIGNAL(clicked()),this,SLOT(OnBtnDownloadClicked()));

        QPushButton *l_pbPause = new QPushButton("Pause");
        m_tableWidget->setCellWidget(taskInfoListId, PauseHeader, l_pbPause);
        connect(l_pbPause, SIGNAL(clicked()),this,SLOT(OnBtnPauseClicked()));

        taskInfoListId++;
    }
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTaskMessages()) );
    timer->start(1000);

    //qDebug() << "549";
}


void AllApplicationDialog::quitTriggered()
{
    bool runningTaskBool = false;
    for (int taskInfoListId=0;taskInfoListId<m_taskInfoList.size();taskInfoListId++)
    {
        if (m_taskInfoList.at(taskInfoListId)->state == runningState) runningTaskBool = true;
    }

    if (runningTaskBool)
    {
        //		QMessageBox::information(this, tr("Infomation"), tr("There are tasks are running.Close Window Will stop all running task.Quit?"));
        if (QMessageBox::question(this, tr("Question"),tr("There are tasks are running.Close Window Will stop all running task.Quit?"),
                                  QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Cancel)
            == QMessageBox::Cancel)
            {
            return;
        }
        else
        {
            stopAllTask();
            if (quitApplicationTimer == 0)
            {
                quitApplicationTimer = new QTimer(this);
                connect(quitApplicationTimer, SIGNAL(timeout()), this, SLOT(quitApplication()));
                quitApplicationTimer->start(50);
                return;
            }
        }
    }
    for (int taskInfoListId=0;taskInfoListId<m_taskInfoList.size();taskInfoListId++)
    {
        if (m_taskInfoList.at(taskInfoListId)->state == pauseState) m_taskInfoList.at(taskInfoListId)->state=stopState;
        saveTaskState(taskInfoListId);
    }
    saveSettings();
    qApp->quit();
}
#if defined(DEBUG)
void AllApplicationDialog::testTriggered()
{
    int taskInfoListId = getCurrentTaskInfoListId();
    if (taskInfoListId == -1) return;

    QFile file("out.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << "notDownloadList: " << "\n";
    for (int notDownloadListId=0;notDownloadListId<taskInfoList.at(taskInfoListId)->notDownloadList.size();notDownloadListId++)
    {
        out << "notDownloadListId: " << notDownloadListId
                << ",taskThreadListId:" << taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->taskThreadListId
                << ",startPosition:" << taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->startPosition
                << ",endPosition" <<taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->endPosition
                << "\n";
    }
    out << "\ntaskThreadList: " << "\n";
    for (int taskThreadListId=0;taskThreadListId<taskInfoList.at(taskInfoListId)->taskThreadList.size();taskThreadListId++)
    {
        out << "taskThreadListId: " << taskThreadListId
                << ",startPosition:" << taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.startPosition
                << ",completed:" << taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.completed
                << ",threadStopped" << taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadStopped
                << ",httpRequestFinished" <<taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.httpRequestFinished
                << "\n";
    }
    file.close();
}
#endif
void AllApplicationDialog::quitApplication()
{
    //qDebug() << "759";
    bool runningTaskBool = false;
    for (int taskInfoListId=0;taskInfoListId<m_taskInfoList.size();taskInfoListId++)
    {
        if (m_taskInfoList.at(taskInfoListId)->state == runningState) runningTaskBool = true;
    }

    if (!runningTaskBool)
    {
        quitApplicationTimer->stop();
        delete quitApplicationTimer;
        quitApplicationTimer = 0;

        for (int taskInfoListId=0;taskInfoListId<m_taskInfoList.size();taskInfoListId++)
        {
            if (m_taskInfoList.at(taskInfoListId)->state == pauseState) m_taskInfoList.at(taskInfoListId)->state=stopState;
            saveTaskState(taskInfoListId);
        }
        saveSettings();
        qApp->quit();
    }
}
void AllApplicationDialog::runTask(int taskInfoListId)
{
    //qDebug() << "758 is ok";
    if (taskInfoListId == -1) taskInfoListId = getCurrentTaskInfoListId();
    if (taskInfoListId == -1) return;

    //qDebug() << m_taskInfoList << "run task";
    if (m_taskInfoList.at(taskInfoListId)->state == finishedState)
    {
        //qDebug() << "finishedstate";
          return;
    }


    if (getRunningTasksCount() >= taskPreferences.maxRunningTasksInt)
    {
        qDebug() << taskPreferences.maxRunningTasksInt << "maximum task account";
        m_taskInfoList.at(taskInfoListId)->state = pauseState;
        return;
    }

    //	threadsDetailTableWidgetClear();

    m_taskInfoList.at(taskInfoListId)->state = runningState;
    m_taskInfoList.at(taskInfoListId)->retry = 0;
    m_taskInfoList.at(taskInfoListId)->elapsedTime = 0;
    //resume task
    if (!m_taskInfoList.at(taskInfoListId)->notDownloadList.isEmpty() && m_taskInfoList.at(taskInfoListId)->size>0)
    {
        int threadsCount = 0;
        if (m_taskInfoList.at(taskInfoListId)->notDownloadList.size() > m_taskInfoList.at(taskInfoListId)->splite)
            threadsCount = m_taskInfoList.at(taskInfoListId)->splite;
        else
            threadsCount = m_taskInfoList.at(taskInfoListId)->notDownloadList.size();
        int taskThreadListId = 0;
        for (int notDownloadListId=0;notDownloadListId<threadsCount;notDownloadListId++)
        {
            taskThreadListId = notDownloadListId;
            if (m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->taskThreadListId == -1)
            {
                downloadThread = new DownloadThread();
                m_taskInfoList.at(taskInfoListId)->taskThreadList.append(downloadThread);
                m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.newReceivedList.clear();
                threadNewReceivedListAppend(taskInfoListId, taskThreadListId, notDownloadListId);
                startTaskThread(taskInfoListId, taskThreadListId, notDownloadListId, true);
            }

        }
        taskThreadListId++;
        while (taskThreadListId < m_taskInfoList.at(taskInfoListId)->splite)
        {
            if (newNotDownload(taskInfoListId) == false) break;

            downloadThread = new DownloadThread();
            m_taskInfoList.at(taskInfoListId)->taskThreadList.append(downloadThread);
#if defined(DEBUG)
            qDebug()<<qPrintable(QString("start new thread:%1").arg(taskThreadListId));
#endif
            m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.newReceivedList.clear();
            threadNewReceivedListAppend(taskInfoListId, taskThreadListId, m_taskInfoList.at(taskInfoListId)->notDownloadList.size()-1);

            startTaskThread(taskInfoListId, taskThreadListId, m_taskInfoList.at(taskInfoListId)->notDownloadList.size()-1, true);
            taskThreadListId++;
        }
        //aaron
        //taskTableWidgetSelectionChanged();

        return;
    }

    //start new download
    QString taskFileName = QString("%1/%2").arg(m_taskInfoList.at(taskInfoListId)->saveTo).arg(m_taskInfoList.at(taskInfoListId)->rename);
    int n = taskFileName.lastIndexOf("?");
    taskFileName = taskFileName.left(n);
    if (QFile::exists(taskFileName))
    {
        if (QMessageBox::question(this, tr("Infomation"),
                                  tr("There already exists a file called %1 in "
                                     "the current directory. Overwrite?").arg(taskFileName),
                                  QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Cancel)
            == QMessageBox::Cancel)
            {
            m_taskInfoList.at(taskInfoListId)->state = stopState;
            return;
        }
        QFile::remove(taskFileName);
    }
    m_taskInfoList.at(taskInfoListId)->elapsedTime = 0;

    newNotDownload(taskInfoListId);
    m_taskInfoList.at(taskInfoListId)->notDownloadList.at(m_taskInfoList.at(taskInfoListId)->notDownloadList.size()-1)->taskThreadListId = 0;
    downloadThread = new DownloadThread();
    m_taskInfoList.at(taskInfoListId)->taskThreadList.append(downloadThread);
    m_taskInfoList.at(taskInfoListId)->taskThreadList.at(0)->threadTaskInfomation.newReceivedList.clear();
    threadNewReceivedListAppend(taskInfoListId, 0, m_taskInfoList.at(taskInfoListId)->notDownloadList.size()-1);
    startTaskThread(taskInfoListId, 0, 0, true);
    //taskTableWidgetSelectionChanged();
}
void AllApplicationDialog::runAllTask()
{
    for (int taskInfoListId=0;taskInfoListId<m_taskInfoList.size();taskInfoListId++)
    {
        if (m_taskInfoList.at(taskInfoListId)->state==stopState || m_taskInfoList.at(taskInfoListId)->state==errorState)
            runTask(taskInfoListId);
    }
}
void AllApplicationDialog::showTaskMessages()
{
    //qDebug() << "863";
    qint64 totalSpeed = 0;
    for (int taskInfoListId=0;taskInfoListId<m_taskInfoList.size();taskInfoListId++)
    {
        //if(taskInfoList.at(taskInfoListId)->state == runningState)
        if(true)
        {
            checkNotDownloadList(taskInfoListId);

            //get file completed bytes
            qint64 taskInfoListIdNotCompleted = 0;
            for (int notDownloadListId=0;notDownloadListId<m_taskInfoList.at(taskInfoListId)->notDownloadList.size();notDownloadListId++)
            {
                taskInfoListIdNotCompleted += m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->endPosition - m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->startPosition;
            }

            m_taskInfoList.at(taskInfoListId)->completed = m_taskInfoList.at(taskInfoListId)->size - taskInfoListIdNotCompleted;

            showTaskState(taskInfoListId);
            saveTaskState(taskInfoListId);
            showActionState();
            checkTaskThread(taskInfoListId);

            //get elapsed time
            if (m_taskInfoList.at(taskInfoListId)->state == runningState)
            {    
                m_taskInfoList.at(taskInfoListId)->elapsedTime++;
            }

            //get speed
            qint64 speed = 0;
            if (m_taskInfoList.at(taskInfoListId)->state == runningState)
                speed = m_taskInfoList.at(taskInfoListId)->completed - m_taskInfoList.at(taskInfoListId)->lastCompleted;
            if (speed < 0) speed = 0;

            //aaron
            if(speed == 0 && m_taskInfoList.at(taskInfoListId)->state == runningState)
            {
                //qDebug() << "speed is 0";
                //restartTask(taskInfoListId);
            }


            m_taskInfoList.at(taskInfoListId)->speed = m_taskInfoList.at(taskInfoListId)->completed - m_taskInfoList.at(taskInfoListId)->lastCompleted;
            m_taskInfoList.at(taskInfoListId)->lastCompleted = m_taskInfoList.at(taskInfoListId)->completed;
            totalSpeed += speed;

            Tools tools;
            m_tableWidget->setItem(taskInfoListId, sizeHeader, new QTableWidgetItem(tools.get_size(m_taskInfoList.at(taskInfoListId)->size)));
            //m_tableWidget->setItem(taskInfoListId, compeletedHeader, new QTableWidgetItem(tools.get_size(taskInfoList.at(taskInfoListId)->completed)));




            float percent = 0;
            if (m_taskInfoList.at(taskInfoListId)->size != 0)
                percent=float(m_taskInfoList.at(taskInfoListId)->completed)/float(m_taskInfoList.at(taskInfoListId)->size)*100;


            //aaron
            //qDebug() << "size:" << taskInfoList.at(taskInfoListId)->size;
            //qDebug() << "complete:" << taskInfoList.at(taskInfoListId)->completed;


            QWidget * l_pWidget = m_tableWidget->cellWidget(taskInfoListId,progressHeader);
            QProgressBar * l_pProgress;
            if(l_pWidget != NULL)
            {
               l_pProgress = (QProgressBar*)l_pWidget;
            }

             if((percent > 0) && (percent < 100))
             {
                 if(l_pWidget == NULL)
                 {
                    qDebug() << "new progresBar";
                    l_pProgress = new QProgressBar(this);
                    m_tableWidget->setCellWidget(taskInfoListId, progressHeader, l_pProgress);
                    l_pProgress->setRange(0,100);
                 }

                 l_pProgress->setValue(percent);
             }
             else
             {
                 if(l_pWidget != NULL)
                 {
                     l_pProgress->hide();
                 }

             }


            m_taskInfoList.at(taskInfoListId)->speedPointList<<m_taskInfoList.at(taskInfoListId)->speed;
            while (m_taskInfoList.at(taskInfoListId)->speedPointList.size() > 50)
                m_taskInfoList.at(taskInfoListId)->speedPointList.removeAt(0);

            //m_tableWidget->setItem(taskInfoListId, progressHeader, new QTableWidgetItem(QString("%1").arg(percent,0,'f',2)));
            if (m_taskInfoList.at(taskInfoListId)->state == runningState)
            {
                m_tableWidget->setItem(taskInfoListId, speedHeader, new QTableWidgetItem(tools.get_size(speed)));
                m_tableWidget->setItem(taskInfoListId, elapsedTimeHeader, new QTableWidgetItem(tools.getTime(m_taskInfoList.at(taskInfoListId)->elapsedTime)));
                m_tableWidget->setItem(taskInfoListId, timeLeftHeader, new QTableWidgetItem(tools.getTimeLeft(m_taskInfoList.at(taskInfoListId)->size, m_taskInfoList.at(taskInfoListId)->completed, speed)));
            }
            else
            {
                m_tableWidget->setItem(taskInfoListId, speedHeader, new QTableWidgetItem(""));
                m_tableWidget->setItem(taskInfoListId, timeLeftHeader, new QTableWidgetItem(""));
            }
        }


        //aaron
        m_tableWidget->setItem(taskInfoListId, retryHeader, new QTableWidgetItem(QString("%1").arg(m_taskInfoList.at(taskInfoListId)->retry)));
        //m_tableWidget->setItem(taskInfoListId, compeletedHeader, new QTableWidgetItem(QString("%1").arg(m_taskInfoList.at(taskInfoListId)->retry)));
        //qDebug() << taskInfoList.at(taskInfoListId)->retry;
    }

    //speedGraphWidget->addSpeedPoint(totalSpeed);
    //showSmallWindow();
    //showGraphWidget(false);
}

void AllApplicationDialog::checkTaskThread(int taskInfoListId)
{
    //start pause state task
    if (m_taskInfoList.at(taskInfoListId)->state==pauseState && getRunningTasksCount()<taskPreferences.maxRunningTasksInt)
    {
        runTask(taskInfoListId);
    }


    //aaron
    if (m_taskInfoList.at(taskInfoListId)->state == errorState && getRunningTasksCount()<taskPreferences.maxRunningTasksInt)
    {
        runTask(taskInfoListId);
    }

    //start finished and error threads.
    for (int taskThreadListId=0;taskThreadListId<m_taskInfoList.at(taskInfoListId)->taskThreadList.size();taskThreadListId++)
    {
        if (m_taskInfoList.at(taskInfoListId)->controlState != noState) break;
        if (m_taskInfoList.at(taskInfoListId)->state==runningState && m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.httpRequestFinished==true && m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadStopped==true && m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->isRunning()==false)
        {
            startFinishedThread(taskInfoListId, taskThreadListId);
        }

        if (m_taskInfoList.at(taskInfoListId)->state==runningState && m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadStopped==true && m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->isRunning()==false && m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadError==true && m_taskInfoList.at(taskInfoListId)->retry<taskPreferences.maxRetryNumberInt)
        {
#if defined(DEBUG)
            qDebug()<<"start error thread";
#endif
            //qDebug() << taskPreferences.maxRetryNumberInt << ":" << m_taskInfoList.at(taskInfoListId)->retry;
            //qDebug("retry++");
            m_taskInfoList.at(taskInfoListId)->retry++;
            m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.startPosition += m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.completed;
            m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.completed = 0;
            m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadStopped = false;
            m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->start();
        }



        /*		if (taskInfoList.at(taskInfoListId)->state==runningState && taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadStopped==true && taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->isRunning()==false && taskInfoList.at(taskInfoListId)->retry<taskPreferences.maxRetryNumberInt)
                                {
                                        if (taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.httpRequestFinished==false && taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadError==false)
                                        {
                                                for (int notDownloadListId=0;notDownloadListId<taskInfoList.at(taskInfoListId)->notDownloadList.size();notDownloadListId++)
                                                {
                                                        if (taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->taskThreadListId == taskThreadListId)
                                                                if (taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->startPosition < taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->endPosition)
                                                                {
                #if defined(DEBUG)
                                                                        qDebug()<<"start non-normal stopped thread";
                #endif
                                                                        taskInfoList.at(taskInfoListId)->retry++;
                                                                        taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.startPosition = taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->startPosition;
                                                                        taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.completed = 0;
                                                                        taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadStopped = false;
                                                                        taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->start();
                                                                }
                                                }
                                        }
                                }
                */

        if (m_taskInfoList.at(taskInfoListId)->state==runningState && m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadStopped==true && m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->isRunning()==false && m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadError==true && m_taskInfoList.at(taskInfoListId)->retry==taskPreferences.maxRetryNumberInt)
        {
#if defined(DEBUG)
            qDebug()<<"errorState";
#endif
            m_taskInfoList.at(taskInfoListId)->controlState = errorState;
            deleteTaskList.append(taskInfoListId);

            //taskTableWidgetSelectionChanged();

            if (deleteTaskTimer == 0)
            {
                deleteTaskTimer = new QTimer(this);
                connect(deleteTaskTimer, SIGNAL(timeout()), this, SLOT(deleteTaskDownloadThread()));
                deleteTaskTimer->start(500);
            }
        }
    }
}
void AllApplicationDialog::checkNotDownloadList(int taskInfoListId)
{
    //qDebug() << "1079";
    for (int notDownloadListId=0;notDownloadListId<m_taskInfoList.at(taskInfoListId)->notDownloadList.size();notDownloadListId++)
    {
        int taskThreadListId = m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->taskThreadListId;

        if (taskThreadListId != -1)
        {
            m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->startPosition = m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.startPosition + m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.completed;
            if (m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->startPosition >m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->endPosition)
            {
#if defined(DEBUG)
                qDebug()<<"if(startPosition>endPosition) startPosition=endPosition="<<taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->endPosition;
#endif
                m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->startPosition = m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->endPosition;
            }
            qint64 startPosition;
            startPosition = m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->startPosition;
            if (m_taskInfoList.at(taskInfoListId)->taskThreadList.size()>0)
            {
                if (taskThreadListId < m_taskInfoList.at(taskInfoListId)->taskThreadList.size())
                    m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.newReceivedList.append(startPosition);
                while (m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.newReceivedList.size() > 3)
                    m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.newReceivedList.removeAt(0);
            }
            if (startPosition>=m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->endPosition && m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->endPosition!=0)
            {

                qint64 size = m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.newReceivedList.size();
                if (size > 0)
                {
                    m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.newReceivedList.removeAt(size-1);
                    qint64 endPosition = m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->endPosition;
                    m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.newReceivedList.append(endPosition);
                }
                //showGraphWidget(false, taskInfoListId, taskThreadListId);
                m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.newReceivedList.clear();
                m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.httpRequestFinished = true;

                NotDownload *notDownload=m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId);
                m_taskInfoList.at(taskInfoListId)->notDownloadList.removeAt(notDownloadListId);
                delete notDownload;
            }
        }
    }
}

void AllApplicationDialog::getFileSize(int taskInfoListId, qint64 fileSize)
{
    //qDebug() << "1187";
    m_taskInfoList.at(taskInfoListId)->size = fileSize;
    m_taskInfoList.at(taskInfoListId)->notDownloadList.at(0)->endPosition = m_taskInfoList.at(taskInfoListId)->size;

    //start new download threads
    while (m_taskInfoList.at(taskInfoListId)->size>0 && m_taskInfoList.at(taskInfoListId)->state==runningState && m_taskInfoList.at(taskInfoListId)->taskThreadList.size() < m_taskInfoList.at(taskInfoListId)->splite && newNotDownload(taskInfoListId)==true)
    {
        int taskThreadListId = m_taskInfoList.at(taskInfoListId)->taskThreadList.size();
        m_taskInfoList.at(taskInfoListId)->notDownloadList.at(m_taskInfoList.at(taskInfoListId)->notDownloadList.size()-1)->taskThreadListId = taskThreadListId;

        downloadThread = new DownloadThread();
        m_taskInfoList.at(taskInfoListId)->taskThreadList.append(downloadThread);
        m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.newReceivedList.clear();
        threadNewReceivedListAppend(taskInfoListId, taskThreadListId, m_taskInfoList.at(taskInfoListId)->notDownloadList.size()-1);
        startTaskThread(taskInfoListId, taskThreadListId, taskThreadListId, true);
        //taskTableWidgetSelectionChanged();
    }
    //showGraphWidget(true);
}
void AllApplicationDialog::getFileDownloadFinished(int taskInfoListId, int taskThreadListId)
{
    m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.httpRequestFinished = true;
    NotDownload *notDownload=m_taskInfoList.at(taskInfoListId)->notDownloadList.at(0);
    m_taskInfoList.at(taskInfoListId)->notDownloadList.removeAt(0);
    delete notDownload;
#if defined(DEBUG)
    qDebug()<<qPrintable(QString("notDownloadList.size%1").arg(taskInfoList.at(taskInfoListId)->notDownloadList.size()));
#endif
    m_taskInfoList.at(taskInfoListId)->state = finishedState;
    if (m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadError)
    {
        m_taskInfoList.at(taskInfoListId)->controlState = errorState;
        deleteTaskList.append(taskInfoListId);

        //taskTableWidgetSelectionChanged();


        if (deleteTaskTimer == 0)
        {
            deleteTaskTimer = new QTimer(this);
            connect(deleteTaskTimer, SIGNAL(timeout()), this, SLOT(deleteTaskDownloadThread()));
           deleteTaskTimer->start(500);
        }

    }

    showTaskState(taskInfoListId);
}


void AllApplicationDialog::pauseTask(int taskInfoListId)
{
    if(-1 == taskInfoListId)
    {
        taskInfoListId = getCurrentTaskInfoListId();
    }

    if (taskInfoListId == -1) return;

    if(-1 == taskInfoListId)
    {
        taskInfoListId = getCurrentTaskInfoListId();
    }

    if (taskInfoListId == -1) return;
    for (int taskThreadListId=0;taskThreadListId<m_taskInfoList.at(taskInfoListId)->taskThreadList.size();taskThreadListId++)
    {
        m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.httpRequestAborted = true;
        m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->setThreadStopped();
        DownloadThread *downloadThread = m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId);
        downloadThread->terminate();
        downloadThread->wait();
        delete downloadThread;
    }


    m_taskInfoList.at(taskInfoListId)->controlState = stopState;

    deleteTaskList.append(taskInfoListId);

    if (deleteTaskTimer == 0)
    {

        deleteTaskTimer = new QTimer(this);

        connect(deleteTaskTimer, SIGNAL(timeout()), this, SLOT(deleteTaskDownloadThread()));

        deleteTaskTimer->start(500);
    }

}

void AllApplicationDialog::restartTask(int taskInfoListId)
{
    //stopTask(taskInfoListId);

    //runTask(taskInfoListId);

    qDebug() << "restart";

}
void AllApplicationDialog::stopTask(int taskInfoListId)
{

    if(-1 == taskInfoListId)
    {
        taskInfoListId = getCurrentTaskInfoListId();
    }

    if (taskInfoListId == -1) return;
    for (int taskThreadListId=0;taskThreadListId<m_taskInfoList.at(taskInfoListId)->taskThreadList.size();taskThreadListId++)
    {
        m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.httpRequestAborted = true;
        m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->setThreadStopped();
    }

    m_taskInfoList.at(taskInfoListId)->controlState = stopState;

    deleteTaskList.append(taskInfoListId);

#if defined(DEBUG)
    qDebug("stopstate********************");
    for (int j=0;j<taskInfoList.at(taskInfoListId)->notDownloadList.size();j++)
    {
        qint64 startPosition = taskInfoList.at(taskInfoListId)->notDownloadList.at(j)->startPosition;
        qint64 endPosition = taskInfoList.at(taskInfoListId)->notDownloadList.at(j)->endPosition;
        int listId = taskInfoList.at(taskInfoListId)->notDownloadList.at(j)->taskThreadListId;
        qDebug()<<qPrintable(QString("%1:%2---%3").arg(listId).arg(startPosition).arg(endPosition));
    }
#endif

    //aaron 0322 for restarttask
    //deleteTaskDownloadThread();


    if (deleteTaskTimer == 0)
    {
        deleteTaskTimer = new QTimer(this);

        connect(deleteTaskTimer, SIGNAL(timeout()), this, SLOT(deleteTaskDownloadThread()));

        deleteTaskTimer->start(50);
    }

}
void AllApplicationDialog::stopAllTask()
{
    for (int taskInfoListId=0;taskInfoListId<m_taskInfoList.size();taskInfoListId++)
    {
        if ( m_taskInfoList.at(taskInfoListId)->state == pauseState)
            m_taskInfoList.at(taskInfoListId)->state = stopState;
        if ( m_taskInfoList.at(taskInfoListId)->state == runningState)
        {
            for (int taskThreadListId=0;taskThreadListId<m_taskInfoList.at(taskInfoListId)->taskThreadList.size();taskThreadListId++)
            {
                m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.httpRequestAborted = true;
                m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->setThreadStopped();
            }
            m_taskInfoList.at(taskInfoListId)->controlState = stopState;
            deleteTaskList.append(taskInfoListId);
        }
    }

    if (deleteTaskTimer == 0)
    {
        deleteTaskTimer = new QTimer(this);
        connect(deleteTaskTimer, SIGNAL(timeout()), this, SLOT(deleteTaskDownloadThread()));
        deleteTaskTimer->start(500);
    }
}
void AllApplicationDialog::deleteTask()
{
    /*
    int taskInfoListId = getCurrentTaskInfoListId();
    if (taskInfoListId == -1) return;
    DeleteTaskDialog *deleteTaskDialog = new DeleteTaskDialog(this);

    if ( deleteTaskDialog->exec() == QDialog::Accepted )
    {
        bool deleteUncompletedBool = deleteTaskDialog->deleteUncompletedCheckBox->isChecked();
        bool deleteDownloadedBool = deleteTaskDialog->deleteDownloadedCheckBox->isChecked();

        if (deleteUncompletedBool==true && m_taskInfoList.at(taskInfoListId)->state!=finishedState)
        {
            QString fileName = QString("%1/%2").arg(m_taskInfoList.at(taskInfoListId)->saveTo).arg(m_taskInfoList.at(taskInfoListId)->rename);
            QFile *file = new QFile(fileName);
            file->remove();
            delete file;
            file = 0;
            deleteTaskAndTableWidget(taskInfoListId);
            return;
        }
        if (deleteDownloadedBool==true && m_taskInfoList.at(taskInfoListId)->state==finishedState)
        {
            QString fileName = QString("%1/%2").arg(m_taskInfoList.at(taskInfoListId)->saveTo).arg(m_taskInfoList.at(taskInfoListId)->rename);
            QFile *file = new QFile(fileName);
            file->remove();
            delete file;
            file = 0;
            deleteTaskAndTableWidget(taskInfoListId);
            return;
        }
        if (deleteUncompletedBool==true && deleteDownloadedBool==true)
        {
            QString fileName = QString("%1/%2").arg(m_taskInfoList.at(taskInfoListId)->saveTo).arg(m_taskInfoList.at(taskInfoListId)->rename);
            QFile *file = new QFile(fileName);
            file->remove();
            delete file;
            file = 0;
            deleteTaskAndTableWidget(taskInfoListId);
            return;
        }
        deleteTaskAndTableWidget(taskInfoListId);
    }
    */
}
void AllApplicationDialog::deleteTaskAndTableWidget(int taskInfoListId)
{
    QString taskFileName = QString("%1/%2.task").arg(m_taskInfoList.at(taskInfoListId)->saveTo).arg(m_taskInfoList.at(taskInfoListId)->rename);
    QFile *file = new QFile(taskFileName);
    file->remove();
    delete file;
    file = 0;
    int row = m_tableWidget->currentRow();
    m_tableWidget->removeRow(row);
    TaskInfomation *tempTaskInfo=m_taskInfoList.at(taskInfoListId);
    m_taskInfoList.removeAt(taskInfoListId);
    for (int notDownloadListId=0;notDownloadListId<tempTaskInfo->notDownloadList.size();notDownloadListId++)
    {
        NotDownload *tempNotDownload = tempTaskInfo->notDownloadList.at(0);
        tempTaskInfo->notDownloadList.removeAt(0);
        delete tempNotDownload;
    }
    delete tempTaskInfo;
    for (int i=taskInfoListId;i<m_taskInfoList.size();i++)
    {
        for (int taskThreadListId=0;taskThreadListId<m_taskInfoList.at(i)->taskThreadList.size();taskThreadListId++)
        {
            m_taskInfoList.at(i)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.taskInfoListId = i;
        }
    }
    saveAllTask();
}
void AllApplicationDialog::redownloadTask()
{
    int taskInfoListId = getCurrentTaskInfoListId();
    if (taskInfoListId == -1) return;

    m_taskInfoList.at(taskInfoListId)->size = 0;
    m_taskInfoList.at(taskInfoListId)->state = stopState;
    while (!m_taskInfoList.at(taskInfoListId)->notDownloadList.isEmpty())
    {
        NotDownload *notDownload=m_taskInfoList.at(taskInfoListId)->notDownloadList.at(0);
        m_taskInfoList.at(taskInfoListId)->notDownloadList.removeAt(0);
        delete notDownload;
    }

    QString taskName = QString("%1/%2.task").arg(m_taskInfoList.at(taskInfoListId)->saveTo).arg(m_taskInfoList.at(taskInfoListId)->rename);
    QSettings settings(taskName, QSettings::IniFormat);

    settings.beginGroup("file");
    settings.setValue("size", m_taskInfoList.at(taskInfoListId)->size);
    settings.setValue("state", m_taskInfoList.at(taskInfoListId)->state);
    settings.endGroup();

    settings.beginGroup("notDownloadsList");
    settings.remove("");
    settings.endGroup();

    runTask();
}
void AllApplicationDialog::modifyTask()
{
    /*
    int taskInfoListId = getCurrentTaskInfoListId();
    if (taskInfoListId == -1) return;
    if (m_taskInfoList.at(taskInfoListId)->state==runningState || m_taskInfoList.at(taskInfoListId)->state==pauseState || m_taskInfoList.at(taskInfoListId)->controlState!=noState)
    {
        QMessageBox::information(this, tr("Information"), tr("Please stop task."));
        return;
    }
    ModifyTaskDialog *modifyTaskDialog = new ModifyTaskDialog(taskInfoListId, this);
    modifyTaskDialog->setWindowTitle(tr("Modify task"));

    if ( modifyTaskDialog->exec() == QDialog::Accepted )
    {
        m_taskInfoList.at(taskInfoListId)->taskInfoListId = m_taskInfoList.size();
        m_taskInfoList.at(taskInfoListId)->url.setUrl(modifyTaskDialog->urlLineEdit->text());
        m_taskInfoList.at(taskInfoListId)->referrer = modifyTaskDialog->referrerLineEdit->text();
        m_taskInfoList.at(taskInfoListId)->saveTo = modifyTaskDialog->saveToLineEdit->text();
        m_taskInfoList.at(taskInfoListId)->rename = modifyTaskDialog->renameLineEdit->text();
        m_taskInfoList.at(taskInfoListId)->splite = modifyTaskDialog->splitSpinBox->value();
        m_taskInfoList.at(taskInfoListId)->user = modifyTaskDialog->userLineEdit->text();
        m_taskInfoList.at(taskInfoListId)->password = modifyTaskDialog->passwordLineEdit->text();

        bool startImmediately;
        if ( modifyTaskDialog->immediatelyRadioButton->isChecked() )
            startImmediately = true;
        else
            startImmediately = false;

        m_taskInfoList.at(taskInfoListId)->comment = modifyTaskDialog->commentTextEdit->toPlainText();

        m_taskInfoList.at(taskInfoListId)->noProxyBool = modifyTaskDialog->noProxyRadioButton->isChecked();
        m_taskInfoList.at(taskInfoListId)->singleProxyBool = modifyTaskDialog->singleProxyRadioButton->isChecked();
        QString singleProxyString = modifyTaskDialog->singleProxyComboBox->currentText();
        m_taskInfoList.at(taskInfoListId)->singleProxyId = getProxyTitleToId(singleProxyString);
        m_taskInfoList.at(taskInfoListId)->mutipleProxyBool = modifyTaskDialog->mutipleProxyRadioButton->isChecked();
        QList <int> mutipleProxyIdsList;
        for (int row=0;row<modifyTaskDialog->mutipleProxyTableWidget->rowCount();row++)
        {
            QComboBox *mutipleProxyComboBox = dynamic_cast<QComboBox*>(modifyTaskDialog->mutipleProxyTableWidget->cellWidget(row, 1));
            QString mutipleProxyString = mutipleProxyComboBox->currentText();
            mutipleProxyIdsList<<getProxyTitleToId(mutipleProxyString);
        }
        m_taskInfoList.at(taskInfoListId)->mutipleProxyIdsList = mutipleProxyIdsList;

        QStringList urlsStringList;
        for (int row=0;row<modifyTaskDialog->ulrsTableWidget->rowCount();row++)
        {
            QTableWidgetItem *item = modifyTaskDialog->ulrsTableWidget->item(row,1);
            QString urlString = item->text();
            urlsStringList<<urlString;
        }
        m_taskInfoList.at(taskInfoListId)->urlsStringList = urlsStringList;

        qint64 taskInfoListIdNotCompleted = 0;
        for (int notDownloadListId=0;notDownloadListId<m_taskInfoList.at(taskInfoListId)->notDownloadList.size();notDownloadListId++)
        {
            taskInfoListIdNotCompleted += m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->endPosition - m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->startPosition;
        }
        m_taskInfoList.at(taskInfoListId)->completed = m_taskInfoList.at(taskInfoListId)->size - taskInfoListIdNotCompleted;

        m_tableWidget->setItem(taskInfoListId, fileNameHeader, new QTableWidgetItem(m_taskInfoList.at(taskInfoListId)->rename));
        m_tableWidget->setItem(taskInfoListId, sizeHeader, new QTableWidgetItem(QString("%1").arg(m_taskInfoList.at(taskInfoListId)->size)));
        m_tableWidget->setItem(taskInfoListId, compeletedHeader, new QTableWidgetItem(QString("%1").arg(m_taskInfoList.at(taskInfoListId)->completed)));
        m_tableWidget->setItem(taskInfoListId, urlHeader, new QTableWidgetItem(m_taskInfoList.at(taskInfoListId)->url.toString()));

        QSettings settings("MPC","task");
        settings.beginGroup(QString("task%1").arg(taskInfoListId));
        settings.setValue("url", m_taskInfoList.at(taskInfoListId)->url.toString());
        settings.setValue("referrer", m_taskInfoList.at(taskInfoListId)->referrer);
        settings.setValue("saveTo", m_taskInfoList.at(taskInfoListId)->saveTo);
        settings.setValue("rename", m_taskInfoList.at(taskInfoListId)->rename);

        settings.setValue("split", m_taskInfoList.at(taskInfoListId)->splite);
        settings.setValue("user", m_taskInfoList.at(taskInfoListId)->user);
        settings.setValue("password", m_taskInfoList.at(taskInfoListId)->password);

        settings.setValue("comment", m_taskInfoList.at(taskInfoListId)->comment);

        settings.setValue("noProxy", m_taskInfoList.at(taskInfoListId)->noProxyBool);
        settings.setValue("singleProxy", m_taskInfoList.at(taskInfoListId)->singleProxyBool);
        settings.setValue("singleProxyId", m_taskInfoList.at(taskInfoListId)->singleProxyId);
        settings.setValue("mutipleProxy", m_taskInfoList.at(taskInfoListId)->mutipleProxyBool);
        QList <QVariant> mutipleProxyIdsListVariant;
        foreach( int mutipleProxyId, m_taskInfoList.at(taskInfoListId)->mutipleProxyIdsList )
           mutipleProxyIdsListVariant << mutipleProxyId;
        settings.setValue("mutipleProxyIds", mutipleProxyIdsListVariant);

        QList <QVariant> urlsListVariant;
        foreach( QString url, urlsStringList )
            urlsListVariant << url;
        settings.setValue("urls", urlsListVariant);
        settings.endGroup();

        QSettings settings2;
        settings2.beginGroup("savePath");
        settings2.setValue("lastPath", m_taskInfoList.at(taskInfoListId)->saveTo);
        settings2.endGroup();

        if (startImmediately) runTask(taskInfoListId);
    }
    */
}





//aaron

void AllApplicationDialog::OnBtnDownloadClicked()
{
    //qDebug() << "1580 is ok";
    //获取按钮所在行
    QPushButton * l_ppbSenderObj=qobject_cast<QPushButton *>(sender());
    if(l_ppbSenderObj == 0)
    {
         return;
    }
    QModelIndex l_miModelIndex = m_tableWidget->indexAt(QPoint(l_ppbSenderObj->frameGeometry().x(),l_ppbSenderObj->frameGeometry().y()));
    int l_iRow = l_miModelIndex.row();

    //qDebug() << l_iRow;
    runTask(l_iRow);

}

void AllApplicationDialog::OnBtnPauseClicked()
{
    //qDebug() << "Button pause clicked";
    //获取按钮所在行
    QPushButton * l_ppbSenderObj=qobject_cast<QPushButton *>(sender());
    if(l_ppbSenderObj == 0)
    {
         return;
    }

    QModelIndex l_miModelIndex = m_tableWidget->indexAt(QPoint(l_ppbSenderObj->frameGeometry().x(),l_ppbSenderObj->frameGeometry().y()));
    int l_iRow = l_miModelIndex.row();

    //qDebug() << l_iRow << "stop";
    stopTask(l_iRow);
    //pauseTask(l_iRow);
}
void AllApplicationDialog::preferencesTriggered()
{

    /*
    showApplicationNormal();

    PreferencesDialog *preferencesDialog = new PreferencesDialog(this);

    if ( preferencesDialog->exec() == QDialog::Accepted )
    {
        taskPreferences.maxRunningTasksInt = preferencesDialog->maxRunningTasksSpinBox->value();
        taskPreferences.splitInt = preferencesDialog->splitSpinBox->value();
        taskPreferences.minimumSplitSizeInt64 = preferencesDialog->minimumSplitSizeSpinBox->value();

        taskPreferences.useLastPathBool = preferencesDialog->useLastPathRadioButton->isChecked();

        taskPreferences.useDefaultPathBool = preferencesDialog->useDefaultPathRadioButton->isChecked();
        taskPreferences.saveToString = preferencesDialog->saveToLineEdit->text();

        taskPreferences.connectTimeOutInt = preferencesDialog->connectTimeOutSpinBox->value();
        if (preferencesDialog->alwaysRetryCheckBox->isChecked())
            taskPreferences.maxRetryNumberInt = -1;
        else
            taskPreferences.maxRetryNumberInt = preferencesDialog->maxRetryNumberSpinBox->value();
        taskPreferences.retryDelayInt = preferencesDialog->retryDelaySpinBox->value();

        taskPreferences.maxSpeedGraph = preferencesDialog->maxSpeedSpinBox->value();
        taskPreferences.maxHeightGraph = preferencesDialog->maxHeightSpinBox->value();
        taskPreferences.pixelWidthGraph = preferencesDialog->pixelWidthSpinBox->value();

        taskPreferences.blockSizeGraph = preferencesDialog->blockSizeSpinBox->value();

        //taskPreferences.monitorClipboardBool = preferencesDialog->monitorClipboardCheckBox->isChecked();
        //monitorClipboardAct->setChecked(taskPreferences.monitorClipboardBool);
        //taskPreferences.monitorTypesString = preferencesDialog->monitorTypesTextEdit->toPlainText();

        QList <QStringList> proxysList;
        QStringList proxyStringList;
        for (int row=0;row<preferencesDialog->proxyTableWidget->rowCount();row++)
        {
            proxyStringList.clear();
            proxyStringList << preferencesDialog->proxyTableWidget->item(row, 0)->text();
            proxyStringList << preferencesDialog->proxyTableWidget->item(row, 1)->text();
            proxyStringList << preferencesDialog->proxyTableWidget->item(row, 2)->text();
            proxyStringList << preferencesDialog->proxyTableWidget->item(row, 3)->text();
            proxyStringList << preferencesDialog->proxyTableWidget->item(row, 4)->text();
            proxyStringList << preferencesDialog->proxyTableWidget->item(row, 5)->text();
            proxysList << proxyStringList;
        }

        QSettings settings;
        settings.beginGroup("task");
        settings.setValue("maxRunningTasks", taskPreferences.maxRunningTasksInt);
        settings.setValue("split", taskPreferences.splitInt);
        settings.setValue("minimumSplitSize", taskPreferences.minimumSplitSizeInt64);
        settings.endGroup();

        settings.beginGroup("savePath");
        settings.setValue("useLastPath", taskPreferences.useLastPathBool);
        settings.setValue("useDefaultPath", taskPreferences.useDefaultPathBool);
        settings.setValue("defaultPath", taskPreferences.saveToString);
        settings.endGroup();

        settings.beginGroup("connection");
        settings.setValue("connectTimeOut", taskPreferences.connectTimeOutInt);
        settings.setValue("maxRetryNumber", taskPreferences.maxRetryNumberInt);
        settings.setValue("retryDelay", taskPreferences.retryDelayInt);
        settings.endGroup();

        settings.beginGroup("speedGraph");
        settings.setValue("maxSpeed", taskPreferences.maxSpeedGraph);
        settings.setValue("maxHeight", taskPreferences.maxHeightGraph);
        settings.setValue("pixelWidth", taskPreferences.pixelWidthGraph);
        settings.endGroup();

        settings.beginGroup("blocksGraph");
        settings.setValue("blockSize", taskPreferences.blockSizeGraph);
        settings.endGroup();

        settings.beginGroup("monitor");
        settings.setValue("monitorClipboard", taskPreferences.monitorClipboardBool);
        settings.setValue("monitorTypes", taskPreferences.monitorTypesString);
        settings.endGroup();

        settings.beginGroup("proxys");
        settings.remove("");
        for (int n=0;n<proxysList.size();n++)
        {
            proxyStringList = proxysList.at(n);
            QList <QVariant> proxyListVariant;
            foreach( QString proxyString, proxyStringList )
                proxyListVariant << proxyString;
            settings.setValue(QString("proxy%1").arg(n), proxyListVariant);
        }
        settings.endGroup();
    }
    delete preferencesDialog;
    preferencesDialog = 0;

    //speedGraphWidget->setData(taskPreferences.maxSpeedGraph*1024, taskPreferences.maxHeightGraph, taskPreferences.pixelWidthGraph);
    //smallWindow->setData(taskPreferences.maxSpeedGraph*1024, taskPreferences.pixelWidthGraph);
    taskGraphPainterWidget->setBlockSizeData( taskPreferences.blockSizeGraph * 1024);
    */
}

/*
void AllApplicationDialog::taskTableWidgetDoubleClicked(int row, int column)
{
    int taskInfoListId = getCurrentTaskInfoListId();
    if (taskInfoListId == -1) return;

    if (taskInfoList.at(taskInfoListId)->state == finishedState)
    {
        QString taskName = QString("%1/%2").arg(taskInfoList.at(taskInfoListId)->saveTo).arg(taskInfoList.at(taskInfoListId)->rename);
        QProcess *myProcess = new QProcess(0);
        myProcess->start(taskName);
    }
}*/

void AllApplicationDialog::closeEvent(QCloseEvent *event)
{
    //if (trayIcon->isVisible())
    //{
     //   hide();
     //   event->ignore();
    //}

    quitTriggered();
}
void AllApplicationDialog::changeEvent(QEvent *event)
{
    //if (event->type() == QEvent::WindowStateChange)
   // {
     //   if (isMinimized())
     //   {
     //       event->ignore();
     //       hide();
     //       return;
    //    }
    //}
}
#ifdef WIN32
bool AllApplicationDialog::winEvent( MSG *m, long *result )
{
    switch ( m->message )
    {
    //case WM_SYSCOMMAND:
    //    {
    //        if ( m->wParam == SC_MINIMIZE )
    //        {
                //			ShowWindow(winId(), SW_HIDE);
    //            hide();
    //            return true;
     //       }
    //    }
    //    break;
    default:
        break;
    }
    return QWidget::winEvent(m, result);
}
#endif
void AllApplicationDialog::saveSettings()
{

}
void AllApplicationDialog::readSettings()
{

    QSettings settings;
    settings.beginGroup("AllApplicationDialog");
    //QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    //QSize size = settings.value("size", QSize(640, 480)).toSize();
    //QByteArray state = settings.value("state", QByteArray()).toByteArray();
    //resize(size);
    //move(pos);
    //restoreGeometry(settings.value("AllApplicationDialogGeometry").toByteArray());
    //restoreState(settings.value("AllApplicationDialogState").toByteArray());
    //m_tableWidget->restoreGeometry(settings.value("tableWidgetState").toByteArray());
    //QList <int> infoDetailSplitterList;
    //QList <QVariant> infoDetailSplitterListVariant;
    //QList <QVariant> infoDetailSplitterListVariantDefault;
    //infoDetailSplitterListVariantDefault<<130<<600;
    //infoDetailSplitterListVariant = settings.value("infoDetailSplitterSizes",infoDetailSplitterListVariantDefault).toList();
    //foreach( QVariant i, infoDetailSplitterListVariant )
        //infoDetailSplitterList << i.toInt();
    //infoDetailSplitter->setSizes(infoDetailSplitterList);
    bool showSpeedWidgetDockBool = settings.value("showSpeedWidgetDock", true).toBool();
    if (showSpeedWidgetDockBool)
    {
        //speedWidgetDock->setVisible(true);
        //viewSpeedWidgetDockAct->setChecked(true);
    }
    else
    {
        //speedWidgetDock->setVisible(false);
        //viewSpeedWidgetDockAct->setChecked(false);
    }
    bool showTaskToolBarBool = settings.value("showTaskToolBar", true).toBool();
    if (showTaskToolBarBool)
    {
        //taskToolBar->setVisible(true);
        //viewTaskToolBarAct->setChecked(true);
    }
    else
    {
        //taskToolBar->setVisible(false);
        //viewTaskToolBarAct->setChecked(false);
    }
    settings.endGroup();


    //settings.beginGroup("SmallWindow");
    //pos = settings.value("pos", QPoint(0, 0)).toPoint();
    //smallWindow->move(pos);
    //bool showSmallWindowBool = settings.value("show", true).toBool();
    //if (showSmallWindowBool)
    //{
    //    smallWindow->setVisible(true);
    //    viewDropWindowAct->setChecked(true);
    //}
    //else
    //{
     //   smallWindow->setVisible(false);
     //   viewDropWindowAct->setChecked(false);
    //}
    //settings.endGroup();

    settings.beginGroup("task");
    taskPreferences.maxRunningTasksInt = settings.value("maxRunningTasks", 5).toInt();
    taskPreferences.splitInt = settings.value("split", 5).toInt();
    taskPreferences.minimumSplitSizeInt64 = settings.value("minimumSplitSize", 300).toInt();
    settings.endGroup();

    settings.beginGroup("savePath");
    taskPreferences.useLastPathBool = settings.value("useLastPath", true).toBool();
    taskPreferences.useDefaultPathBool = settings.value("useDefaultPath", false).toBool();
    taskPreferences.saveToString = settings.value("defaultPath").toString();
    settings.endGroup();

    settings.beginGroup("connection");
    taskPreferences.connectTimeOutInt = settings.value("connectTimeOut", 30).toInt();
    taskPreferences.maxRetryNumberInt = settings.value("maxRetryNumber", 99).toInt();
    taskPreferences.retryDelayInt = settings.value("retryDelay", 5).toInt();
    settings.endGroup();

    settings.beginGroup("speedGraph");
    taskPreferences.maxSpeedGraph = settings.value("maxSpeed", 250).toLongLong();
    taskPreferences.maxHeightGraph = settings.value("maxHeight", 60).toInt();
    taskPreferences.pixelWidthGraph = settings.value("pixelWidth", 2).toInt();
    settings.endGroup();

    settings.beginGroup("blocksGraph");
    taskPreferences.blockSizeGraph = settings.value("blockSize", 50).toLongLong();
    settings.endGroup();

    //settings.beginGroup("monitor");
    //taskPreferences.monitorClipboardBool = settings.value("monitorClipboard", true).toBool();
    //monitorClipboardAct->setChecked(taskPreferences.monitorClipboardBool);
    //QString defaultMonitorTypes = ".zip;.rar;.gz;.z;.bz2;.tar;.arj;.lzh;.exe;.bin;.mp3;.mp4";
    //taskPreferences.monitorTypesString = settings.value("monitorTypes", defaultMonitorTypes).toString();
    //settings.endGroup();

    //speedGraphWidget->setData(taskPreferences.maxSpeedGraph*1024, taskPreferences.maxHeightGraph, taskPreferences.pixelWidthGraph);
    //qDebug() << "2038";
    //smallWindow->setData(taskPreferences.maxSpeedGraph*1024, taskPreferences.pixelWidthGraph);
}
int AllApplicationDialog::getCurrentTaskInfoListId()
{
    int row = m_tableWidget->currentRow();
    QTableWidgetItem *currentTableWidgetItem;

    //aaron
    //currentTableWidgetItem = m_tableWidget->item(row, urlHeader);
    currentTableWidgetItem = m_tableWidget->item(row, fileNameHeader);
    int taskInfoListId=-1;
    for (int i=0;i<m_taskInfoList.size();i++)
    {
        QTableWidgetItem *tableWidgetItem;

        //aaron
        //tableWidgetItem = m_tableWidget->item(i, urlHeader);
        tableWidgetItem = m_tableWidget->item(i, fileNameHeader);

        if (currentTableWidgetItem == tableWidgetItem)
        {
            taskInfoListId = i;
            break;
        }
    }
    return taskInfoListId;
}
int AllApplicationDialog::getCurrentTaskThreadListId()
{
    QList<QTreeWidgetItem *> selectedItems;
    selectedItems = threadNumberTreeWidget->selectedItems();
    if (selectedItems.isEmpty()) return -1;
    QTreeWidgetItem *currentTreeWidgetItem;
    currentTreeWidgetItem = selectedItems.at(0);

    QString taskThreadListIdString = currentTreeWidgetItem->text(0);
    if (taskThreadListIdString == "Infomation") return -1;

    if (taskThreadListIdString.size()<6) return -1;
    taskThreadListIdString = taskThreadListIdString.mid(6);
    int taskThreadListId = taskThreadListIdString.toInt();

    return taskThreadListId;
}
void AllApplicationDialog::showTaskState(int taskInfoListId)
{


    //QTableWidgetItem *stateTableWidgetItem;
    //stateTableWidgetItem = m_tableWidget->item(taskInfoListId, stateHeader);
    //m_tableWidget->takeItem(taskInfoListId, stateHeader);
    //delete stateTableWidgetItem;

    //set finished state
    if (m_taskInfoList.at(taskInfoListId)->state==runningState && m_taskInfoList.at(taskInfoListId)->controlState==noState && m_taskInfoList.at(taskInfoListId)->completed==m_taskInfoList.at(taskInfoListId)->size && m_taskInfoList.at(taskInfoListId)->completed>0)
    {
#if defined(DEBUG)
        qDebug()<<qPrintable(QString("set finished taskid%1 controlState=finishedState").arg(taskInfoListId));
#endif
        //showSmallWindow();
        m_taskInfoList.at(taskInfoListId)->controlState = finishedState;
        deleteTaskList.append(taskInfoListId);

#if defined(DEBUG)
        qDebug("set finished state********************");
        for (int j=0;j<taskInfoList.at(taskInfoListId)->notDownloadList.size();j++)
        {
            qint64 startPosition = taskInfoList.at(taskInfoListId)->notDownloadList.at(j)->startPosition;
            qint64 endPosition = taskInfoList.at(taskInfoListId)->notDownloadList.at(j)->endPosition;
            int listId = taskInfoList.at(taskInfoListId)->notDownloadList.at(j)->taskThreadListId;
            qDebug()<<qPrintable(QString("%1:%2---%3").arg(listId).arg(startPosition).arg(endPosition));
        }
#endif


        if (deleteTaskTimer == 0)
        {
#if defined(DEBUG)
            qDebug()<<"start deleteTaskTimer";
#endif
            deleteTaskTimer = new QTimer(this);
            connect(deleteTaskTimer, SIGNAL(timeout()), this, SLOT(deleteTaskDownloadThread()));
            deleteTaskTimer->start(500);
        }
    }

    QPushButton *l_pDownloadButton = new QPushButton("Download");
    m_tableWidget->setCellWidget(taskInfoListId, DownloadHeader, l_pDownloadButton);
    connect(l_pDownloadButton, SIGNAL(clicked()),this,SLOT(OnBtnDownloadClicked()));

    QPushButton *l_pPauseButton = new QPushButton("Pause");
    m_tableWidget->setCellWidget(taskInfoListId, PauseHeader, l_pPauseButton);
    connect(l_pPauseButton, SIGNAL(clicked()),this,SLOT(OnBtnPauseClicked()));

    //QWidget * l_pDownloadWidget = m_tableWidget->cellWidget(taskInfoListId,compeletedHeader);
    //QPushButton *l_pDownloadButton = (QPushButton*)l_pDownloadWidget;
    //QWidget * l_pPauseWidget = m_tableWidget->cellWidget(taskInfoListId,urlHeader);
    //QPushButton *l_pPauseButton = (QPushButton*)l_pPauseWidget;

    switch (m_taskInfoList.at(taskInfoListId)->state)
    {
    case stopState:
        l_pDownloadButton->setEnabled(true);
        l_pPauseButton->setEnabled(false);
        m_tableWidget->setItem(taskInfoListId, stateHeader, new QTableWidgetItem(QIcon(":/images/stopstate.png"),""));
        break;
    case runningState:
        l_pDownloadButton->setEnabled(false);
         l_pPauseButton->setEnabled(true);
        m_tableWidget->setItem(taskInfoListId, stateHeader, new QTableWidgetItem(QIcon(":/images/runningstate.png"),""));
        break;
    case finishedState:
        l_pDownloadButton->setEnabled(false);
         l_pPauseButton->setEnabled(false);
        m_tableWidget->setItem(taskInfoListId, stateHeader, new QTableWidgetItem(QIcon(":/images/finishedstate.png"),""));
        break;
    case pauseState:
        l_pDownloadButton->setEnabled(true);
         l_pPauseButton->setEnabled(false);
        m_tableWidget->setItem(taskInfoListId, stateHeader, new QTableWidgetItem(QIcon(":/images/pausestate.png"),""));
        break;
    case errorState:
        l_pDownloadButton->setEnabled(true);
         l_pPauseButton->setEnabled(false);
        m_tableWidget->setItem(taskInfoListId, stateHeader, new QTableWidgetItem(QIcon(":/images/errorstate.png"),""));
        break;
    }

}
void AllApplicationDialog::deleteTaskDownloadThread()
{
    if (!deleteTaskList.isEmpty())
    {

        int taskStoppedCount = 0;
        int taskInfoListId = deleteTaskList.at(0);

        for (int taskThreadListId=0;taskThreadListId<m_taskInfoList.at(taskInfoListId)->taskThreadList.size();taskThreadListId++)
        {
            //qDebug() << m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadStopped << "thread is stoped?\n";
            //qDebug() << m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->isRunning() << "thread is running?\n";
            if (m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadStopped==true && m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->isRunning()==false)
            {
                taskStoppedCount ++;
            }
        }

        //qDebug() << "taskStoppedCount" << taskStoppedCount;
        //qDebug() << "taskThreadList.size()" << m_taskInfoList.at(taskInfoListId)->taskThreadList.size();

        //if (taskStoppedCount==taskInfoList.at(taskInfoListId)->taskThreadList.size())
        if(true)
        {
           //showGraphWidget(false);

            while (!m_taskInfoList.at(taskInfoListId)->taskThreadList.isEmpty())
            {

                //disconnect(taskInfoList.at(taskInfoListId)->taskThreadList.at(0), SIGNAL(returnThreadMessages(int,int,int, QString)), this, SLOT(showThreadMessages(int,int,int, QString)));
                disconnect(m_taskInfoList.at(taskInfoListId)->taskThreadList.at(0), SIGNAL(fileDownloadFinished(int,int)), this, SLOT(getFileDownloadFinished(int,int)));
                disconnect(m_taskInfoList.at(taskInfoListId)->taskThreadList.at(0), SIGNAL(returnFileSize(int,qint64)), this, SLOT(getFileSize(int,qint64)));


                DownloadThread *downloadThread = m_taskInfoList.at(taskInfoListId)->taskThreadList.at(0);

                //qDebug() << downloadThread->isRunning();
                //aaron
                if(downloadThread->isRunning())
                {
                    downloadThread->terminate();
                    downloadThread->wait();
                }

                m_taskInfoList.at(taskInfoListId)->taskThreadList.removeAt(0);
                delete downloadThread;

#if defined(DEBUG)
                qDebug()<<qPrintable(QString("threadCount:%1").arg(taskInfoList.at(taskInfoListId)->taskThreadList.size()));
#endif
            }
            for (int i=0;i<m_taskInfoList.at(taskInfoListId)->notDownloadList.size();i++)
            {
                m_taskInfoList.at(taskInfoListId)->notDownloadList.at(i)->taskThreadListId = -1;
            }


            //aaron
            //taskTableWidgetSelectionChanged();
            deleteTaskList.removeAt(0);


#if defined(DEBUG)
            qDebug()<<qPrintable(QString("set taskid%1 state=%2").arg(taskInfoListId).arg(taskInfoList.at(taskInfoListId)->controlState));
#endif
            m_taskInfoList.at(taskInfoListId)->state = m_taskInfoList.at(taskInfoListId)->controlState;
#if defined(DEBUG)
            qDebug()<<"set controlState = noState";
#endif
            m_taskInfoList.at(taskInfoListId)->controlState = noState;
            saveTaskState(taskInfoListId);

        }
    }
    else
    {
        deleteTaskTimer->stop();
        delete deleteTaskTimer;
        deleteTaskTimer = 0;
    }
}
/*
void AllApplicationDialog::showThreadMessages(int taskInfoListId, int taskThreadListId, int messagesType, QString strResponseHeader)
{
    showThreadMessagesMutex.lock();
    int currentTaskInfoListId = getCurrentTaskInfoListId();

    int currentTaskThreadListId = getCurrentTaskThreadListId();

    QDateTime dateTime = QDateTime::currentDateTime();
    QString str = dateTime.toString("yyyy-MM-dd hh:mm:ss");
    QStringList list = strResponseHeader.split("\r\n");
    for (int i=0;i<list.size();i++)
    {
        QString str = dateTime.toString("yyyy-MM-dd hh:mm:ss ");
        if (list.at(i).isEmpty())
        {
            continue;
        }

        QTableWidgetItem *threadStateDetailItem;
        QIcon stateIcon;
        switch (messagesType)
        {
                case uploadMessages:
            stateIcon.addFile(":/images/uploadmessages.png");
            break;
                case downloadMessages:
            stateIcon.addFile(":/images/downloadmessages.png");
            break;
                case infomationMessages:
            stateIcon.addFile(":/images/infomationmessages.png");
            break;
                case errorMessages:
            stateIcon.addFile(":/images/errormessages.png");
            break;
                default:
            stateIcon.addFile(":/images/errormessages.png");
            break;
        }
        taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.stateDetailList.append(stateIcon);
        taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.timeDetailList.append(str);
        taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.messageDetailList.append(list.at(i));

        if (currentTaskInfoListId==taskInfoListId && currentTaskThreadListId==taskThreadListId)
        {
            int row = threadsDetailTableWidget->rowCount();
            threadsDetailTableWidget->insertRow(row);
            threadStateDetailItem = new QTableWidgetItem(stateIcon,str);
            threadsDetailTableWidget->setItem(row, 0, threadStateDetailItem);
            QTableWidgetItem *threadMessageDetailItem;
            threadMessageDetailItem =  new QTableWidgetItem(list.at(i));
            threadsDetailTableWidget->setItem(row, 1, threadMessageDetailItem);
            QFontMetrics fm = threadsDetailTableWidget->fontMetrics();
            int pixelsHigh = fm.height();
            threadsDetailTableWidget->setRowHeight(row, pixelsHigh);
            threadsDetailTableWidget->resizeColumnToContents(0);
            threadsDetailTableWidget->setColumnWidth(1, threadGraphDetailWidget->width()-threadsDetailTableWidget->columnWidth(0)-38);
            threadsDetailTableWidget->scrollToBottom();
        }
    }
    showThreadMessagesMutex.unlock();
}*/
void AllApplicationDialog::startFinishedThread(int taskInfoListId, int taskThreadListId)
{
    for (int notDownloadListId=0;notDownloadListId<m_taskInfoList.at(taskInfoListId)->notDownloadList.size();notDownloadListId++)
    {
        if (m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->taskThreadListId == -1)
        {
#if defined(DEBUG)
            qDebug("startFinishedThread********************");
            for (int j=0;j<taskInfoList.at(taskInfoListId)->notDownloadList.size();j++)
            {
                qint64 startPosition = taskInfoList.at(taskInfoListId)->notDownloadList.at(j)->startPosition;
                qint64 endPosition = taskInfoList.at(taskInfoListId)->notDownloadList.at(j)->endPosition;
                int listId = taskInfoList.at(taskInfoListId)->notDownloadList.at(j)->taskThreadListId;
                qDebug()<<qPrintable(QString("%1:%2---%3").arg(listId).arg(startPosition).arg(endPosition));
            }
#endif

            m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.newReceivedList.clear();
            threadNewReceivedListAppend(taskInfoListId, taskThreadListId, notDownloadListId);

            startTaskThread(taskInfoListId, taskThreadListId, notDownloadListId);
#if defined(DEBUG)
            qDebug("startedFinishedThread********************");
            for (int j=0;j<taskInfoList.at(taskInfoListId)->notDownloadList.size();j++)
            {
                qint64 startPosition = taskInfoList.at(taskInfoListId)->notDownloadList.at(j)->startPosition;
                qint64 endPosition = taskInfoList.at(taskInfoListId)->notDownloadList.at(j)->endPosition;
                int listId = taskInfoList.at(taskInfoListId)->notDownloadList.at(j)->taskThreadListId;
                qDebug()<<qPrintable(QString("%1:%2---%3").arg(listId).arg(startPosition).arg(endPosition));
            }
#endif
            return;
        }

    }
#if defined(DEBUG)
    qDebug("before********************");
    for (int j=0;j<taskInfoList.at(taskInfoListId)->notDownloadList.size();j++)
    {
        qint64 startPosition = taskInfoList.at(taskInfoListId)->notDownloadList.at(j)->startPosition;
        qint64 endPosition = taskInfoList.at(taskInfoListId)->notDownloadList.at(j)->endPosition;
        int listId = taskInfoList.at(taskInfoListId)->notDownloadList.at(j)->taskThreadListId;
        qDebug()<<qPrintable(QString("%1:%2---%3").arg(listId).arg(startPosition).arg(endPosition));
    }
#endif
    if (newNotDownload(taskInfoListId) == false) return;
#if defined(DEBUG)
    qDebug()<<qPrintable(QString("start finished thread%1").arg(taskThreadListId));
#endif
    m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.newReceivedList.clear();
    threadNewReceivedListAppend(taskInfoListId, taskThreadListId, m_taskInfoList.at(taskInfoListId)->notDownloadList.size()-1);

    startTaskThread(taskInfoListId, taskThreadListId, m_taskInfoList.at(taskInfoListId)->notDownloadList.size()-1);
#if defined(DEBUG)
    qDebug("after********************");
    for (int j=0;j<taskInfoList.at(taskInfoListId)->notDownloadList.size();j++)
    {
        qint64 startPosition = taskInfoList.at(taskInfoListId)->notDownloadList.at(j)->startPosition;
        qint64 endPosition = taskInfoList.at(taskInfoListId)->notDownloadList.at(j)->endPosition;
        int listId = taskInfoList.at(taskInfoListId)->notDownloadList.at(j)->taskThreadListId;
        qDebug()<<qPrintable(QString("%1:%2---%3").arg(listId).arg(startPosition).arg(endPosition));
    }
#endif
}
void AllApplicationDialog::threadsDetailTableWidgetClear()
{
    if (threadsDetailTableWidget->rowCount() > 0)
    {
        for (int row=threadsDetailTableWidget->rowCount()-1;row>=0;row--)
        {
            for (int column=threadsDetailTableWidget->columnCount()-1;column>=0;column--)
            {
                QTableWidgetItem *tempItem = threadsDetailTableWidget->item(row, column);
                threadsDetailTableWidget->takeItem (row, column );
                delete tempItem;
            }
        }
        threadsDetailTableWidget->setRowCount(0);
    }
}
bool AllApplicationDialog::newNotDownload(int taskInfoListId)
{
    if (m_taskInfoList.at(taskInfoListId)->notDownloadList.isEmpty() && m_taskInfoList.at(taskInfoListId)->state==finishedState)
        return false;
    qint64 newThreadStartPosition = 0;
    qint64 newThreadEndPosition = 0;
    qint64 notDownloadBytes = 0;
    if (m_taskInfoList.at(taskInfoListId)->notDownloadList.isEmpty())
    {
#if defined(DEBUG)
        qDebug("newThreadStartPosition::0");
#endif
        notDownload = new NotDownload;
        notDownload->taskThreadListId = -1;
        notDownload->startPosition = 0;
        notDownload->endPosition = 0;
        m_taskInfoList.at(taskInfoListId)->notDownloadList.append(notDownload);
        return true;
    }
    qint64 startPosition = 0;
    qint64 endPosition = 0;
    qint64 tempId = 0;
    for (int i=0;i<m_taskInfoList.at(taskInfoListId)->notDownloadList.size();i++)
    {
        startPosition = m_taskInfoList.at(taskInfoListId)->notDownloadList.at(i)->startPosition;
        endPosition = m_taskInfoList.at(taskInfoListId)->notDownloadList.at(i)->endPosition;
        if (endPosition - startPosition > notDownloadBytes)
        {
            notDownloadBytes = endPosition - startPosition;
            newThreadStartPosition = startPosition + notDownloadBytes/2;
            newThreadEndPosition = endPosition;
            tempId = i;
        }
    }
    if (notDownloadBytes >= taskPreferences.minimumSplitSizeInt64*1024)
    {
        notDownload = new NotDownload;
        m_taskInfoList.at(taskInfoListId)->notDownloadList.append(notDownload);
        m_taskInfoList.at(taskInfoListId)->notDownloadList.at(m_taskInfoList.at(taskInfoListId)->notDownloadList.size()-1)->taskThreadListId = -1;
        m_taskInfoList.at(taskInfoListId)->notDownloadList.at(m_taskInfoList.at(taskInfoListId)->notDownloadList.size()-1)->startPosition = newThreadStartPosition;
        m_taskInfoList.at(taskInfoListId)->notDownloadList.at(m_taskInfoList.at(taskInfoListId)->notDownloadList.size()-1)->endPosition = newThreadEndPosition;
        m_taskInfoList.at(taskInfoListId)->notDownloadList.at(tempId)->endPosition = newThreadStartPosition;
#if defined(DEBUG)
        qDebug()<<qPrintable(QString("newThreadStartPosition:%1").arg(newThreadStartPosition));
#endif

        return true;
    }
    return false;
}
void AllApplicationDialog::saveAllTask()
{
    QSettings settings("MPC","task");
    settings.clear();
    for (int taskInfoListId=0;taskInfoListId<m_taskInfoList.size();taskInfoListId++)
    {
        settings.beginGroup(QString("task%1").arg(taskInfoListId));
        settings.setValue("url", m_taskInfoList.at(taskInfoListId)->url.toString());
        settings.setValue("saveTo", m_taskInfoList.at(taskInfoListId)->saveTo);
        settings.setValue("rename", m_taskInfoList.at(taskInfoListId)->rename);
        settings.setValue("comment", m_taskInfoList.at(taskInfoListId)->comment);
        settings.setValue("split", QString::number(m_taskInfoList.at(taskInfoListId)->splite));
        settings.setValue("user", m_taskInfoList.at(taskInfoListId)->user);
        settings.setValue("password", m_taskInfoList.at(taskInfoListId)->password);
        settings.endGroup();
    }
}
void AllApplicationDialog::saveTaskState(int taskInfoListId)
{
    QString taskName = QString("%1/%2.task").arg(m_taskInfoList.at(taskInfoListId)->saveTo).arg(m_taskInfoList.at(taskInfoListId)->rename);
    QSettings settings(taskName, QSettings::IniFormat);

    settings.beginGroup("file");
    settings.setValue("size", m_taskInfoList.at(taskInfoListId)->size);
    settings.setValue("state", m_taskInfoList.at(taskInfoListId)->state);
    settings.endGroup();

    settings.beginGroup("notDownloadsList");
    settings.remove("");
    for (int notDownloadListId=0;notDownloadListId<m_taskInfoList.at(taskInfoListId)->notDownloadList.size();notDownloadListId++)
    {
        QString notDownloadListIdString = QString("notDownloadId%1").arg(notDownloadListId);
        QStringList notDownloadListIdValueString;
        notDownloadListIdValueString<<QString("%1").arg(m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->startPosition)<<QString("%1").arg(m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->endPosition);
        settings.setValue(notDownloadListIdString, notDownloadListIdValueString);
    }
    settings.endGroup();
    //qDebug() << "2328";
}
void AllApplicationDialog::readTaskState(int taskInfoListId)
{
    //qDebug() << "2332";
    QString taskName = QString("%1/%2.task").arg(m_taskInfoList.at(taskInfoListId)->saveTo).arg(m_taskInfoList.at(taskInfoListId)->rename);
    QSettings settings(taskName, QSettings::IniFormat);

    settings.beginGroup("file");
    m_taskInfoList.at(taskInfoListId)->size = settings.value("size", 0).toLongLong();
    m_taskInfoList.at(taskInfoListId)->state = settings.value("state", 0).toInt();
    if (m_taskInfoList.at(taskInfoListId)->state==pauseState || m_taskInfoList.at(taskInfoListId)->state==runningState)
        m_taskInfoList.at(taskInfoListId)->state = stopState;
    settings.endGroup();
    //qDebug() << m_taskInfoList.at(taskInfoListId)->size << "let's check";
    //qDebug() << "2415";

    settings.beginGroup("notDownloadsList");
    int notDownloadListId=0;
    QStringList notDownloadListIdValueString;
    QStringList defaultValue;
    defaultValue<<"-1"<<"-1";
    m_taskInfoList.at(taskInfoListId)->notDownloadList.clear();

    //qDebug() << "2425";
    while (true)
    {
        QString notDownloadListIdString = QString("notDownloadId%1").arg(notDownloadListId);
        notDownloadListIdValueString.clear();
        notDownloadListIdValueString<<settings.value(notDownloadListIdString, defaultValue).toStringList();
        if (notDownloadListIdValueString.at(0) == "-1") break;
        notDownload = new NotDownload;
        notDownload->taskThreadListId = -1;
        notDownload->startPosition = notDownloadListIdValueString.at(0).toLongLong();
        notDownload->endPosition = notDownloadListIdValueString.at(1).toLongLong();
        m_taskInfoList.at(taskInfoListId)->notDownloadList.append(notDownload);
        notDownloadListId++;
    }
    settings.endGroup();
}
void AllApplicationDialog::connectTaskThread(int taskInfoListId, int taskThreadListId)
{

    //connect(m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId), SIGNAL(returnThreadMessages(int,int,int, QString)), this, SLOT(showThreadMessages(int,int,int, QString)));
    connect(m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId), SIGNAL(fileDownloadFinished(int,int)), this, SLOT(getFileDownloadFinished(int,int)));
    connect(m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId), SIGNAL(returnFileSize(int,qint64)), this, SLOT(getFileSize(int,qint64)));
}

void AllApplicationDialog::threadNewReceivedListAppend(int taskInfoListId, int taskThreadListId, int notDownloadListId)
{
    //qDebug() << "2416";
    qint64 startPosition = m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->startPosition;
    m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.newReceivedList.append(startPosition);
    while (m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.newReceivedList.size() > 3)
        m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.newReceivedList.removeAt(0);
}
int AllApplicationDialog::getRunningTasksCount()
{
    //qDebug() << "2424";
    int runningTasksInt = 0;
    for (int taskInfoListId=0;taskInfoListId<m_taskInfoList.size();taskInfoListId++)
    {
        if (m_taskInfoList.at(taskInfoListId)->state == runningState) runningTasksInt++;
    }
    //qDebug() << runningTasksInt;
    return runningTasksInt;
    //qDebug() << "2432";
}
void AllApplicationDialog::startTaskThread(int taskInfoListId, int taskThreadListId, int notDownloadListId, bool connectTaskThreadBool)
{
    //qDebug() << "2435";
    if (m_taskInfoList.at(taskInfoListId)->singleProxyBool)
    {
        if (m_taskInfoList.at(taskInfoListId)->singleProxyId >= 0)
        {
            QSettings settings2;
            settings2.beginGroup("proxys");
            QStringList proxyStringList;
            proxyStringList << settings2.value(QString("proxy%1").arg(m_taskInfoList.at(taskInfoListId)->singleProxyId)).toStringList();
            settings2.endGroup();
            if (proxyStringList.size() >= 5)
            {
                m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.proxyHost = proxyStringList.at(1);
                m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.proxyPort = proxyStringList.at(2).toInt();
                m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.proxyUser = proxyStringList.at(4);
                m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.proxyPassword = proxyStringList.at(5);
            }
        }
    }
    if (m_taskInfoList.at(taskInfoListId)->mutipleProxyBool)
    {
        if (taskThreadListId < m_taskInfoList.at(taskInfoListId)->mutipleProxyIdsList.size())
        {
            int mutipleProxyId = m_taskInfoList.at(taskInfoListId)->mutipleProxyIdsList.at(taskThreadListId);
            if (mutipleProxyId >= 0)
            {
                QSettings settings2;
                settings2.beginGroup("proxys");
                QStringList proxyStringList;
                proxyStringList << settings2.value(QString("proxy%1").arg(mutipleProxyId)).toStringList();
                settings2.endGroup();
                if (proxyStringList.size() >= 5)
                {
                    m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.proxyHost = proxyStringList.at(1);
                    m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.proxyPort = proxyStringList.at(2).toInt();
                    m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.proxyUser = proxyStringList.at(4);
                    m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.proxyPassword = proxyStringList.at(5);
                }
            }
        }
    }
    m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.taskInfoListId = taskInfoListId;
    m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.taskThreadListId = taskThreadListId;
    m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->taskThreadListId = taskThreadListId;
    if (m_taskInfoList.at(taskInfoListId)->urlsStringList.size()>taskThreadListId && m_taskInfoList.at(taskInfoListId)->urlsStringList.at(taskThreadListId)!="")
        m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.url = m_taskInfoList.at(taskInfoListId)->urlsStringList.at(taskThreadListId);
    else
        m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.url = m_taskInfoList.at(taskInfoListId)->url;
    m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.referrer = m_taskInfoList.at(taskInfoListId)->referrer;
    QString taskFileName = QString("%1/%2").arg(m_taskInfoList.at(taskInfoListId)->saveTo).arg(m_taskInfoList.at(taskInfoListId)->rename);
    m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.fileName = taskFileName;
    m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.size = m_taskInfoList.at(taskInfoListId)->size;
    m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.startPosition = m_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->startPosition;
    m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.completed = 0;
    m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadStopped = false;
    if (connectTaskThreadBool == true)
        connectTaskThread(taskInfoListId, taskThreadListId);
    m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->start();
}
void AllApplicationDialog::showActionState()
{

    //qDebug() << "2496";
    int taskInfoListId = getCurrentTaskInfoListId();
    //qDebug() << taskInfoListId << "the state";
    if (taskInfoListId == -1) return;

    if (m_taskInfoList.at(taskInfoListId)->state == runningState)
    {
        //runTaskAct->setEnabled(false);
        //stopTaskAct->setEnabled(true);
        //deleteTaskAct->setEnabled(false);
        //redownloadTaskAct->setEnabled(false);
        //modifyTaskAct->setEnabled(false);
    }
    else
    {
        //runTaskAct->setEnabled(true);
        //stopTaskAct->setEnabled(false);
        //deleteTaskAct->setEnabled(true);
        //redownloadTaskAct->setEnabled(true);
        //modifyTaskAct->setEnabled(true);
    }
    if (m_taskInfoList.at(taskInfoListId)->state == finishedState)
    {
        //runTaskAct->setEnabled(false);
        //stopTaskAct->setEnabled(false);
        //deleteTaskAct->setEnabled(true);
        //redownloadTaskAct->setEnabled(true);
        //modifyTaskAct->setEnabled(true);
    }
}
void AllApplicationDialog::createTaskTableWidgetContextMenu()
{
    /*
    m_tableWidget->addAction(addTaskAct);
    m_tableWidget->addAction(separatorAct);
    m_tableWidget->addAction(runTaskAct);
    m_tableWidget->addAction(stopTaskAct);
    m_tableWidget->addAction(deleteTaskAct);
    m_tableWidget->addAction(separatorAct2);
    m_tableWidget->addAction(modifyTaskAct);
    m_tableWidget->addAction(redownloadTaskAct);
    m_tableWidget->addAction(separatorAct3);
    m_tableWidget->addAction(runAllTaskAct);
    m_tableWidget->addAction(stopAllTaskAct);
    m_tableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);*/
}
void AllApplicationDialog::copyTaskThreadsDetail()
{
    QList<QTableWidgetItem *> selectedItems;
    selectedItems = threadsDetailTableWidget->selectedItems();
    if (selectedItems.isEmpty()) return;
    QString clipboardString;
    for (int selectedItemsId=0;selectedItemsId<selectedItems.size()/2;selectedItemsId++)
    {
        QTableWidgetItem *item0 = selectedItems.at(selectedItemsId);
        QTableWidgetItem *item1 = selectedItems.at(selectedItemsId+selectedItems.size()/2);
        clipboardString += QString("%1 %2\r\n").arg(item0->text()).arg(item1->text());
    }
    //clipboard->setText(clipboardString);
}
void AllApplicationDialog::createTaskThreadsDetailTableWidgetContextMenu()
{
    //threadsDetailTableWidget->addAction(copyTaskThreadsDetailAct);
    //threadsDetailTableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
}
void AllApplicationDialog::switchLanguage(QAction *action)
{
    QString locale = action->data().toString();

    QSettings settings;
    settings.beginGroup("AllApplicationDialog");
    settings.setValue("language", locale);
    settings.endGroup();

    QMessageBox::information(this, tr("Infomation"), tr("Take effect after restart software."));
}
void AllApplicationDialog::showApplicationNormal()
{
    showNormal();
    //showGraphWidget(true);
}
void AllApplicationDialog::acceptClipboard()
{
    /*
    if (!taskPreferences.monitorClipboardBool) return;
    const QMimeData *mimeData = clipboard->mimeData();
    if (!mimeData->hasText()) return;

    bool allowedUrlBool = false;
    bool allowedTypesBool = false;
    QString string = mimeData->text();
    int n = string.indexOf(" ");
    if (n!=-1 && n<2048)
    {
        string = string.left(n);
    }
    n = string.indexOf("\'");
    if (n!=-1 && n<2048)
    {
        string = string.left(n);
    }
    n = string.indexOf("\"");
    if (n!=-1 && n<2048)
    {
        string = string.left(n);
    }
    n = string.indexOf(">");
    if (n!=-1 && n<2048)
    {
        string = string.left(n);
    }
    QStringList allowedUrlList;
    allowedUrlList<<"http://"<<"https://"<<"ftp://";
    for (int allowedUrlListId=0;allowedUrlListId<allowedUrlList.size();allowedUrlListId++)
    {
        if (string.left(allowedUrlList.at(allowedUrlListId).size()) == allowedUrlList.at(allowedUrlListId) )
        {
            allowedUrlBool = true;
            break;
        }
    }
    if (!allowedUrlBool) return;

    QStringList allowedTypesList = taskPreferences.monitorTypesString.split(";");
    for (int allowedTypesListId=0;allowedTypesListId<allowedTypesList.size();allowedTypesListId++)
    {
        n = QString::compare(string.right(allowedTypesList.at(allowedTypesListId).size()), allowedTypesList.at(allowedTypesListId), Qt::CaseInsensitive);
        if (n == 0)
        {
            allowedTypesBool = true;
            break;
        }
    }
    if (allowedUrlBool==true && allowedTypesBool==true)
    {
#if defined(DEBUG)
        qDebug()<<"clipboard add task:"<<string;
#endif
        newTask(string);
    }
    */
}
int AllApplicationDialog::getProxyTitleToId(QString title)
{
    QSettings settings;
    settings.beginGroup("proxys");
    int n=0;
    while (true)
    {
        QStringList proxyStringList;
        proxyStringList << settings.value(QString("proxy%1").arg(n)).toStringList();
        if ( proxyStringList.isEmpty() ) break;
        if ( title == proxyStringList.at(0) )
        {
            settings.endGroup();
            return n;
        }
        n++;
    }
    settings.endGroup();

    return -1;
}

