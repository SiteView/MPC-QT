#include "allapplicationdialog.h"
#include "httphandle.h"
#include "informationanalyze.h"
#include "aes.h"
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
#include <QLabel>
#include <QDir>
#include "AllApplicationDialog.h"
#include "httpdownload.h"
#include "tools.h"
#include "setup.h"

#define SERVERINI "config/server.ini"
#define LOCALINI "config/local.ini"
#define UPDATEINI "config/update.ini"
#define CONFIGINI "config.ini"
#define SERVERURL "http://192.168.9.27:8080/Web-Server/server.ini"

#define KEY  "siteview20130326"
#ifdef WIN32
#include <windows.h>
#endif

#include <QProgressBar>


AllApplicationDialog::AllApplicationDialog(QList <TaskInformation *> taskInformation, QWidget *parent) :
    QWidget(parent)
{
    //aaron 0402

    m_taskInfoList = taskInformation;
    //aaron 0327
    //QString language = tr("English");	//Translate English into your own language
    //versionStatus.append("ALL");
    //versionStatus.append("new");
    //versionStatus.append("needupdate");
    //versionStatus.append("normal");

    //m_iVersionStatus = status;
    QDir dir;
    m_sCurrentPath = dir.currentPath();

    createTaskTableWidget();
    createThreadNumberTreeWidget();

    //aaron 0326 加密
    unsigned char key[] = "siteview20130326";
    m_pAES = new AES(key);

    QWidget::setWindowTitle("Marketplace Client");
    //aaron 0326
    //createTaskInformation();
    //aaron 0327
    //createTaskInformation();

    m_tableWidget->setShowGrid(false);
    m_tableWidget->show();

    m_tableWidget ->setObjectName(QString::fromUtf8("tableWidget"));
    QHBoxLayout *verticalLayout = new QHBoxLayout();
    verticalLayout->addWidget(m_tableWidget);
    this->setLayout(verticalLayout);

    verticalLayout->setAlignment(m_tableWidget,Qt::AlignJustify);
    this->setFixedSize(740,460);

    //createUpdateFile();
    setTableInformation();

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
    QDir l_dirTemp;
    if(!l_dirTemp.exists("config/"))
    {
        l_dirTemp.mkpath("config/");
    }

    QSettings l_psconfig(CONFIGINI, QSettings::IniFormat) ;
    QString l_sServerURL = l_psconfig.value("server/url").toString();

    qDebug() << l_sServerURL << "URL";
    m_pHttpHandle = new HttpHandle(QUrl(l_sServerURL), "config/");
    m_pHttpHandle->vDownloadFile();

    m_pInformationAnalyze = new InformationAnalyze(SERVERINI, LOCALINI, UPDATEINI);

    //在下载安装完毕后更新显示
    connect(m_pHttpHandle, SIGNAL(sig_vSetupfinish()), this, SLOT(slt_vUpdate()));
    slt_vUpdate();
}

void AllApplicationDialog::slt_vUpdate()
{
    qDebug() << "update";
    m_pInformationAnalyze->vRefresh();
    createTaskInformation();
}


void AllApplicationDialog::createTaskTableWidget()
{
    m_tableWidget = new QTableWidget(this);
    m_tableWidget->setColumnCount(11);
    m_tableWidget->setRowCount(0);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_tableWidget->resize(700,500);
    m_tableWidget->setFixedSize(700,420);

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
    m_tableWidget->horizontalHeader()->setFont(QFont("Helvetica"));
    //aaron 3028
    //m_tableWidget->horizontalHeader()->setStretchLastSection(true);
    //m_tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    //m_tableWidget->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    QList <int> columnWidths;
    columnWidths<<60<<80<<80<<160<<70<<180<<180<<0<<90<<45<<45<<45;
    for (int i=0;i<columnWidths.size();i++)
    {
        m_tableWidget->setColumnWidth(i,columnWidths.at(i));
    }
    //m_tableWidget->resizeColumnsToContents();

    m_tableWidget->horizontalHeader()->setVisible(true);   //隐藏列表头
}
void AllApplicationDialog::createThreadNumberTreeWidget()
{

}

void AllApplicationDialog::setTaskInformation(QList <TaskInformation *> &taskInfoList)
{
    //m_taskInfoList = taskInfoList;
}

void AllApplicationDialog::setTableInformation()
{
    //qDebug() <<  "set table information";
    for(int taskInfoListId = 0; taskInfoListId < m_taskInfoList.size(); taskInfoListId++)
    {
        //qDebug() << "what is the status:" <<  status;
        //qDebug() << versionStatus[status];
        //qDebug() << m_taskInfoList.at(taskInfoListId)->comment;
        //if(versionStatus[status] != m_taskInfoList.at(taskInfoListId)->comment)
        //{
        //    continue;
        //}
        qDebug() << "set table information" << m_taskInfoList.at(taskInfoListId)->comment;

        int row = m_tableWidget->rowCount();
        m_tableWidget->insertRow(row);
        if (m_taskInfoList.at(taskInfoListId)->state == runningState)
            m_taskInfoList.at(taskInfoListId)->state = stopState;
        showTaskState(taskInfoListId);

        //aaron 0328
        //aaron 0328
        QTextCodec *textcodec = QTextCodec::codecForName("GBK");
        QString l_sTemp = textcodec->toUnicode(m_taskInfoList.at(taskInfoListId)->applicationName.toLatin1().data());
        m_tableWidget->setItem(taskInfoListId, fileNameHeader, new QTableWidgetItem(l_sTemp));
        //m_tableWidget->setItem(taskInfoListId, fileNameHeader, new QTableWidgetItem(m_taskInfoList.at(taskInfoListId)->applicationName));
        //m_tableWidget->setItem(taskInfoListId, fileNameHeader, new QTableWidgetItem("你好"));


        m_tableWidget->setItem(taskInfoListId, sizeHeader, new QTableWidgetItem(QString("%1").arg(m_taskInfoList.at(taskInfoListId)->size)));
        //m_tableWidget->setItem(taskInfoListId, compeletedHeader, new QTableWidgetItem(QString("%1").arg(taskInfoList.at(taskInfoListId)->completed)));
        float percent = 0;
        if (m_taskInfoList.at(taskInfoListId)->size != 0)
            percent=float(m_taskInfoList.at(taskInfoListId)->completed)/float(m_taskInfoList.at(taskInfoListId)->size)*100;
        //m_tableWidget->setItem(taskInfoListId, progressHeader, new QTableWidgetItem(QString("%1").arg(percent,0,'f',2)));
        //m_tableWidget->setItem(taskInfoListId, urlHeader, new QTableWidgetItem(taskInfoList.at(taskInfoListId)->url.toString()));

        //0329 aaron
        //m_tableWidget->setItem(taskInfoListId, commentHeader, new QTableWidgetItem(m_taskInfoList.at(taskInfoListId)->comment));
        m_tableWidget->setItem(taskInfoListId, elapsedTimeHeader, new QTableWidgetItem("Current version:" + m_taskInfoList.at(taskInfoListId)->currentVersion));
        m_tableWidget->setItem(taskInfoListId, timeLeftHeader, new QTableWidgetItem("Last version:" + m_taskInfoList.at(taskInfoListId)->updateVersion));

        //aaron 0328 图片
        QString l_sPicture = QString("%1/%2.jpg").arg(m_taskInfoList.at(taskInfoListId)->saveTo).arg(m_taskInfoList.at(taskInfoListId)->rename);
        qDebug() <<  l_sPicture  ;
        QLabel *label = new QLabel();
        label->setPixmap(QPixmap(l_sPicture));
        m_tableWidget->setCellWidget(taskInfoListId, stateHeader, label);
        //m_tableWidget->setItem(taskInfoListId, stateHeader, new QTableWidgetItem(QIcon(l_sPicture),""));

        //aaron 0329


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

        //aaron 0326 根据状态显示
        QPushButton *l_pbDownload = new QPushButton("Update");
        QPushButton *l_pbPause = new QPushButton("Pause");


        if("needupdate" == m_taskInfoList.at(taskInfoListId)->comment)
        {
            l_pbDownload->setText("Update");
            //m_tableWidget->setCellWidget(taskInfoListId, DownloadHeader, l_pbDownload);
            //connect(l_pbDownload, SIGNAL(clicked()),this,SLOT(OnBtnDownloadClicked()));

            l_pbPause->setText("Pause");
            //m_tableWidget->setCellWidget(taskInfoListId, PauseHeader, l_pbPause);
            //connect(l_pbPause, SIGNAL(clicked()),this,SLOT(OnBtnPauseClicked()));

            //aaron 0329
            //l_pbPause->resize(50, 30);
            //l_pbPause->setFont(QFont("Times", 18, QFont::Bold));

            //aaron 0408 uninstall
            QPushButton *l_pbUninstall = new QPushButton("Uninstall");
            //aaron 0408 uninstall
            connect(l_pbUninstall, SIGNAL(clicked()),this,SLOT(OnBtnUninstallClicked()));
            //aaron 0408 uninstall
            m_tableWidget->setCellWidget(taskInfoListId, UninstallHeader, l_pbUninstall);

        }
        else if("new" == m_taskInfoList.at(taskInfoListId)->comment)
        {
            l_pbDownload->setText("Download");
            l_pbPause->setText("Pause");
        }
        else
        {
            l_pbDownload->setText("Done");
            l_pbPause->setText("Pause");

            qDebug() << "stopState";
            m_taskInfoList.at(taskInfoListId)->state = doneState;

            //aaron 0408 uninstall
            QPushButton *l_pbUninstall = new QPushButton("Uninstall");
            //aaron 0408 uninstall
            connect(l_pbUninstall, SIGNAL(clicked()),this,SLOT(OnBtnUninstallClicked()));
            //aaron 0408 uninstall
            m_tableWidget->setCellWidget(taskInfoListId, UninstallHeader, l_pbUninstall);


        }
        connect(l_pbDownload, SIGNAL(clicked()),this,SLOT(OnBtnDownloadClicked()));
        connect(l_pbPause, SIGNAL(clicked()),this,SLOT(OnBtnPauseClicked()));




        //QLabel * labelDownload = new QLabel();
        //QVBoxLayout * layoutDownload = new QVBoxLayout();
        //layoutDownload->addWidget(l_pbDownload);
        //labelDownload->setLayout(layoutDownload);
        m_tableWidget->setCellWidget(taskInfoListId, DownloadHeader, l_pbDownload);

        //QLabel * labelPause = new QLabel();
        //QVBoxLayout * layoutPause = new QVBoxLayout();
        //layoutPause->addWidget(l_pbPause);
        //labelPause->setLayout(layoutPause);
        m_tableWidget->setCellWidget(taskInfoListId, PauseHeader, l_pbPause);






        //m_tableWidget->horizontalHeader()->resizeSection(0,300);
        m_tableWidget->setRowHeight(taskInfoListId,60);
    }


    //aaron 0329
    //setCentralWidget(m_tableWidget);

    //aaron 0326
    //timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(showTaskMessages()) );
    //timer->start(1000);
}
void AllApplicationDialog::createTaskInformation()
{
    /*
    m_taskInfoList.clear();
    int taskInfoListId = 0;

    //aaron
    QSettings l_RegSettings("MPC","task");

    //settings.beginGroup(QString("task%1").arg(taskInfoListId));
    l_RegSettings.beginGroup(QString("task"));

    //QString urlString = l_RegSettings.value("url").toString();
    QString referrerString  = l_RegSettings.value("referrer").toString();
    //QString saveToString = l_RegSettings.value("saveTo").toString();

    //QString l_cu
    QString saveToString = "downloads";
    //int splitInt = 5;
    //QString renameString = l_RegSettings.value("rename").toString();

    //aaron 以下配置暂未使用
    int splitInt = l_RegSettings.value("split",3).toInt();
    QString userString = l_RegSettings.value("user").toString();
    QString passwordString = l_RegSettings.value("password").toString();
    //QString commentString = l_RegSettings.value("comment").toString();
    bool noProxyBool = l_RegSettings.value("noProxy", true).toBool();
    bool singleProxyBool = l_RegSettings.value("singleProxy", false).toBool();
    int singleProxyId = l_RegSettings.value("singleProxyId", -1).toInt();
    bool mutipleProxyBool = l_RegSettings.value("mutipleProxy", false).toBool();
    QList <QVariant> mutipleProxyIdsVariant = l_RegSettings.value("mutipleProxyIds").toList();;
    QList <QVariant> urlsListVariant = l_RegSettings.value("urls").toList();
    l_RegSettings.endGroup();

    //UPDATEFILE需要做到设置里
    QSettings l_psetUpdate(UPDATEINI, QSettings::IniFormat) ;
    QString l_sUpdateSoftware = l_psetUpdate.value("Software").toString();
    QStringList l_slSoftwareList = l_sUpdateSoftware.split("|");
    //unsigned char key[] = "siteview20130326";
    //AES l_aes(key);

    for (int i = 0; i < l_slSoftwareList.size(); ++i)
    {


        QString l_sSoftwareName = l_slSoftwareList.at(i);

        //aaron 0326 加密
        QString urlString = l_psetUpdate.value(l_sSoftwareName+"/url").toString();

        QString l_sEncryptURL = l_psetUpdate.value(l_sSoftwareName+"/url").toString();
        QString l_sUncryptURL;

        m_pAES->OnAesUncrypt(l_sEncryptURL, l_sUncryptURL);

        qDebug() << l_sUncryptURL;
        urlString = l_sUncryptURL;


        QFileInfo fileInfo(QUrl(urlString).path());
        QString fileName = fileInfo.fileName();


        //aaron 0326 设置状态
        QString commentString = l_psetUpdate.value(l_sSoftwareName+"/status").toString();

        //aaron 0328 静默安装参数
        QString l_sSilentParam = l_psetUpdate.value(l_sSoftwareName+"/silentparam").toString();

        //aaron 0329
        QString l_sUpdateVersion = l_psetUpdate.value(l_sSoftwareName + "/Version").toString();
        QString l_sCurrentVersion = l_psetUpdate.value(l_sSoftwareName + "/localVersion").toString();

        if (urlString.isEmpty())
        {
            qDebug() << "url is empty, break";
            break;
        }

        TaskInfomation *taskInfo;
        taskInfo = new TaskInfomation;

        taskInfo->applicationName = l_sSoftwareName;
        taskInfo->taskInfoListId = taskInfoListId;
        taskInfo->controlState = noState;
        taskInfo->state = stopState;
        taskInfo->url.setUrl(urlString);

        //aaron 0328
        taskInfo->silentParam = l_sSilentParam;

        //aaron 0329
        taskInfo->updateVersion = l_sUpdateVersion;
        taskInfo->currentVersion =l_sCurrentVersion;

        //qDebug() << "299 is ok";
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
        //qDebug() << "315 is ok";
        foreach( QVariant mutipleProxyId, mutipleProxyIdsVariant )
            taskInfo->mutipleProxyIdsList<<mutipleProxyId.toInt();
        foreach( QVariant url, urlsListVariant )
            taskInfo->urlsStringList<<url.toString();

        //qDebug() << "320 is ok";

        m_taskInfoList.append(taskInfo);

        //qDebug() << "328 is ok";
        readTaskState(taskInfoListId);
        //qDebug() << "326 is ok";
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
    */
    //setTableInformation();
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

    qApp->quit();
}

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

        qApp->quit();
    }
}
void AllApplicationDialog::runTask(int taskInfoListId)
{

    qDebug() << "needupdate";
    qDebug() << taskInfoListId;
    qDebug() << m_taskInfoList.at(taskInfoListId)->comment;
    qDebug() << taskInfoListId;

    if (taskInfoListId == -1) taskInfoListId = getCurrentTaskInfoListId();
    if (taskInfoListId == -1) return;

    if (m_taskInfoList.at(taskInfoListId)->state == finishedState)
    {
        return;
    }

    qDebug() << "511";

    if (getRunningTasksCount() >= taskPreferences.maxRunningTasksInt)
    {
        qDebug() << taskPreferences.maxRunningTasksInt << "maximum task account";
        m_taskInfoList.at(taskInfoListId)->state = pauseState;
        return;
    }

    m_taskInfoList.at(taskInfoListId)->state = runningState;
    m_taskInfoList.at(taskInfoListId)->retry = 0;
    m_taskInfoList.at(taskInfoListId)->elapsedTime = 0;
    //resume task
    if (!m_taskInfoList.at(taskInfoListId)->notDownloadList.isEmpty() && m_taskInfoList.at(taskInfoListId)->size>0)
    {
        int threadsCount = 0;
        qDebug() << "notDownloadList.size" << m_taskInfoList.at(taskInfoListId)->notDownloadList.size();
        qDebug() << "m_taskInfoList.at(taskInfoListId)->splite" << m_taskInfoList.at(taskInfoListId)->splite;

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

        return;
    }

    //start new download
    QString taskFileName = QString("%1/%2").arg(m_taskInfoList.at(taskInfoListId)->saveTo).arg(m_taskInfoList.at(taskInfoListId)->rename);
    int n = taskFileName.lastIndexOf("?");
    taskFileName = taskFileName.left(n);
    if (QFile::exists(taskFileName))
    {
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

    qint64 totalSpeed = 0;
    for (int taskInfoListId=0;taskInfoListId<m_taskInfoList.size();taskInfoListId++)
    {
        //qDebug() << "what is the status:" <<  m_iVersionStatus;
        //qDebug() << versionStatus[m_iVersionStatus];
        //qDebug() << m_taskInfoList.at(taskInfoListId)->comment;
        //if(versionStatus[m_iVersionStatus] != m_taskInfoList.at(taskInfoListId)->comment)
       // {
        //    qDebug() << "continue";
         //   continue;
        //}

        //if(m_taskInfoList.at(taskInfoListId)->state == runningState)
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


            m_taskInfoList.at(taskInfoListId)->speed = m_taskInfoList.at(taskInfoListId)->completed - m_taskInfoList.at(taskInfoListId)->lastCompleted;
            m_taskInfoList.at(taskInfoListId)->lastCompleted = m_taskInfoList.at(taskInfoListId)->completed;
            totalSpeed += speed;

            Tools tools;
            m_tableWidget->setItem(taskInfoListId, sizeHeader, new QTableWidgetItem(tools.get_size(m_taskInfoList.at(taskInfoListId)->size)));

            //m_tableWidget->setItem(taskInfoListId, compeletedHeader, new QTableWidgetItem(tools.get_size(taskInfoList.at(taskInfoListId)->completed)));

            //aaron 0328
            //QString l_sPicture = QString("%1/%2.jpg").arg(m_taskInfoList.at(taskInfoListId)->saveTo).arg(m_taskInfoList.at(taskInfoListId)->rename);
            //qDebug() <<  l_sPicture  ;
            //m_tableWidget->setItem(taskInfoListId, stateHeader, new QTableWidgetItem(QIcon(l_sPicture),""));



            float percent = 0;
            if (m_taskInfoList.at(taskInfoListId)->size != 0)
                percent=float(m_taskInfoList.at(taskInfoListId)->completed)/float(m_taskInfoList.at(taskInfoListId)->size)*100;

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

            if (m_taskInfoList.at(taskInfoListId)->state == runningState)
            {
                m_tableWidget->setItem(taskInfoListId, speedHeader, new QTableWidgetItem(tools.get_size(speed)));
            }
            else
            {
                m_tableWidget->setItem(taskInfoListId, speedHeader, new QTableWidgetItem(""));
            }
        }
    }
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
            //aaron 0407
            //startFinishedThread(taskInfoListId, taskThreadListId);
        }

        if (m_taskInfoList.at(taskInfoListId)->state==runningState && m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadStopped==true && m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->isRunning()==false && m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadError==true && m_taskInfoList.at(taskInfoListId)->retry<taskPreferences.maxRetryNumberInt)
        {
#if defined(DEBUG)
            qDebug()<<"start error thread";
#endif
            //qDebug() << taskPreferences.maxRetryNumberInt << ":" << m_taskInfoList.at(taskInfoListId)->retry;
            qDebug("retry++");
            qDebug() << m_taskInfoList.at(taskInfoListId)->retry;
            m_taskInfoList.at(taskInfoListId)->retry++;
            m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.startPosition += m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.completed;
            m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadTaskInfomation.completed = 0;
            m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->threadStopped = false;

            //aaron 20130401 终止线程
            if(m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->isRunning())
            {
                qDebug() << "terminate thread";
                m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->terminate();
                m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->wait();
            }

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
            //aaron 20130403 终止线程
            if(m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->isRunning())
            {
                qDebug() << "terminate thread";
                m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->terminate();
                m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->wait();
            }

            m_taskInfoList.at(taskInfoListId)->controlState = errorState;
            deleteTaskList.append(taskInfoListId);

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

    }

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
        //m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId)->setThreadStopped();
        DownloadThread *downloadThread = m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId);
        downloadThread->terminate();
        downloadThread->wait();
        downloadThread->deleteLater() ;
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
        //delete m_taskInfoList.at(taskInfoListId)->taskThreadList.at(taskThreadListId);
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
    TaskInformation *tempTaskInfo=m_taskInfoList.at(taskInfoListId);
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
    //saveAllTask();
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

    qDebug() << l_ppbSenderObj->parentWidget()->frameGeometry().x();
    qDebug() << l_ppbSenderObj->parentWidget()->frameGeometry().y();

    //aaron 0329 把BUTTON放到LABEL上，方便调整大小
    //QModelIndex l_miModelIndex = m_tableWidget->indexAt(QPoint(l_ppbSenderObj->parentWidget()->frameGeometry().x(),l_ppbSenderObj->parentWidget()->frameGeometry().y()));
    QModelIndex l_miModelIndex = m_tableWidget->indexAt(QPoint(l_ppbSenderObj->frameGeometry().x(),l_ppbSenderObj->frameGeometry().y()));
    int l_iRow = l_miModelIndex.row();

    //qDebug() <<l_ppbSenderObj->frameGeometry().x();
    //qDebug() << l_ppbSenderObj->frameGeometry().y();
    qDebug() << l_iRow;

    qDebug() << "needupdate";
    runTask(l_iRow);
}

//aaron 0408 uninstall

void AllApplicationDialog::OnBtnUninstallClicked()
{
    //qDebug() << "1580 is ok";
    //获取按钮所在行
    QPushButton * l_ppbSenderObj=qobject_cast<QPushButton *>(sender());
    if(l_ppbSenderObj == 0)
    {
         return;
    }

    qDebug() << l_ppbSenderObj->parentWidget()->frameGeometry().x();
    qDebug() << l_ppbSenderObj->parentWidget()->frameGeometry().y();

    //aaron 0329 把BUTTON放到LABEL上，方便调整大小
    //QModelIndex l_miModelIndex = m_tableWidget->indexAt(QPoint(l_ppbSenderObj->parentWidget()->frameGeometry().x(),l_ppbSenderObj->parentWidget()->frameGeometry().y()));
    QModelIndex l_miModelIndex = m_tableWidget->indexAt(QPoint(l_ppbSenderObj->frameGeometry().x(),l_ppbSenderObj->frameGeometry().y()));
    int l_iRow = l_miModelIndex.row();

    QString l_sCMD = m_taskInfoList.at(l_iRow)->uninstallString;
    qDebug() << l_sCMD;

    Setup * mySetup = new Setup(l_sCMD,"" , l_iRow);
    QThread *thread = new QThread;
    mySetup->moveToThread(thread);
    thread->start();
    connect(thread, SIGNAL(started()), mySetup, SLOT(vExecuteUninstall()));
    //connect(mySetup, SIGNAL(sig_vSetupfinish(int)),this, SLOT(slt_vSetupFinished(int)));
    connect(mySetup,SIGNAL(sig_vSetupfinish(int)),thread, SLOT(quit()));

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


void AllApplicationDialog::closeEvent(QCloseEvent *event)
{
    quitTriggered();
}


void AllApplicationDialog::readSettings()
{
    //aaron 0402
    QSettings settings;

    settings.beginGroup("task");
    taskPreferences.maxRunningTasksInt = settings.value("maxRunningTasks", 5).toInt();
    taskPreferences.splitInt = settings.value("split", 1).toInt();
    taskPreferences.minimumSplitSizeInt64 = settings.value("minimumSplitSize", 300).toInt();
    settings.endGroup();

    settings.beginGroup("savePath");
    taskPreferences.useLastPathBool = settings.value("useLastPath", true).toBool();
    taskPreferences.useDefaultPathBool = settings.value("useDefaultPath", false).toBool();
    taskPreferences.saveToString = settings.value("defaultPath").toString();
    settings.endGroup();

    settings.beginGroup("connection");
    taskPreferences.connectTimeOutInt = settings.value("connectTimeOut", 30).toInt();
    taskPreferences.maxRetryNumberInt = settings.value("maxRetryNumber", 9).toInt();
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

        //aaron 0327
        QString taskName = QString("%1/%2.task").arg(m_taskInfoList.at(taskInfoListId)->saveTo).arg(m_taskInfoList.at(taskInfoListId)->rename);
        if(QFile::exists(taskName))
        {
            QFile::remove(taskName);
            qDebug() << "delete" << taskName;
        }

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

    //aaron 0326
    //QPushButton *l_pDownloadButton = new QPushButton("Download");
    //m_tableWidget->setCellWidget(taskInfoListId, DownloadHeader, l_pDownloadButton);
    //connect(l_pDownloadButton, SIGNAL(clicked()),this,SLOT(OnBtnDownloadClicked()));

    //QPushButton *l_pPauseButton = new QPushButton("Pause");
    //m_tableWidget->setCellWidget(taskInfoListId, PauseHeader, l_pPauseButton);
    //connect(l_pPauseButton, SIGNAL(clicked()),this,SLOT(OnBtnPauseClicked()));
    //aaron 0326 根据状态显示
    /*
    QPushButton *l_pDownloadButton ;
    QPushButton *l_pPauseButton;
    if("needupdate" == m_taskInfoList.at(taskInfoListId)->comment)
    {
        l_pDownloadButton = new QPushButton("Update");
        m_tableWidget->setCellWidget(taskInfoListId, DownloadHeader, l_pDownloadButton);
        connect(l_pDownloadButton, SIGNAL(clicked()),this,SLOT(OnBtnDownloadClicked()));

        l_pPauseButton = new QPushButton("Pause");
        m_tableWidget->setCellWidget(taskInfoListId, PauseHeader, l_pPauseButton);
        connect(l_pPauseButton, SIGNAL(clicked()),this,SLOT(OnBtnPauseClicked()));
    }
    else if("new" == m_taskInfoList.at(taskInfoListId)->comment)
    {
        l_pDownloadButton = new QPushButton("Download");
        m_tableWidget->setCellWidget(taskInfoListId, DownloadHeader, l_pDownloadButton);
        connect(l_pDownloadButton, SIGNAL(clicked()),this,SLOT(OnBtnDownloadClicked()));

        l_pPauseButton = new QPushButton("Pause");
        m_tableWidget->setCellWidget(taskInfoListId, PauseHeader, l_pPauseButton);
        connect(l_pPauseButton, SIGNAL(clicked()),this,SLOT(OnBtnPauseClicked()));
    }
    else
    {
        //aaron 0327
        //l_pDownloadButton = new QPushButton("Download");
        l_pDownloadButton = new QPushButton("Done");

        m_tableWidget->setCellWidget(taskInfoListId, DownloadHeader, l_pDownloadButton);
        connect(l_pDownloadButton, SIGNAL(clicked()),this,SLOT(OnBtnDownloadClicked()));

        l_pPauseButton = new QPushButton("Pause");
        m_tableWidget->setCellWidget(taskInfoListId, PauseHeader, l_pPauseButton);
        connect(l_pPauseButton, SIGNAL(clicked()),this,SLOT(OnBtnPauseClicked()));

        //aaron 0327
        m_taskInfoList.at(taskInfoListId)->state = doneState;
        //m_taskInfoList.at(taskInfoListId)->state = finishedState;
    }
    */


    //aaron 0328
    QWidget * l_pWidgetDown = m_tableWidget->cellWidget(taskInfoListId,DownloadHeader);
    QPushButton *l_pDownloadButton ;
    //l_pDownloadButton = (QPushButton*)l_pWidgetDown;


    if(l_pWidgetDown != NULL)
    {

       l_pDownloadButton = (QPushButton*)l_pWidgetDown;
    }
    else
    {
        if("needupdate" == m_taskInfoList.at(taskInfoListId)->comment)
        {
            l_pDownloadButton = new QPushButton("Update");
            m_tableWidget->setCellWidget(taskInfoListId, DownloadHeader, l_pDownloadButton);
            connect(l_pDownloadButton, SIGNAL(clicked()),this,SLOT(OnBtnDownloadClicked()));

            //aaron 0329
            //l_pDownloadButton->resize(50, 20);
            //l_pDownloadButton->setFont(QFont("Times", 18, QFont::Bold));
        }
        else if("new" == m_taskInfoList.at(taskInfoListId)->comment)
        {
            l_pDownloadButton = new QPushButton("Download");
            m_tableWidget->setCellWidget(taskInfoListId, DownloadHeader, l_pDownloadButton);
            connect(l_pDownloadButton, SIGNAL(clicked()),this,SLOT(OnBtnDownloadClicked()));

            //aaron 0329
            //l_pDownloadButton->resize(50, 20);
            //l_pDownloadButton->setFont(QFont("Times", 18, QFont::Bold));

        }
        else
        {
            //aaron 0327
            //l_pDownloadButton = new QPushButton("Download");
            l_pDownloadButton = new QPushButton("Done");
            m_tableWidget->setCellWidget(taskInfoListId, DownloadHeader, l_pDownloadButton);
            connect(l_pDownloadButton, SIGNAL(clicked()),this,SLOT(OnBtnDownloadClicked()));
            m_taskInfoList.at(taskInfoListId)->state = doneState;

             //aaron 0329
             //l_pDownloadButton->resize(50, 20);
             //l_pDownloadButton->setFont(QFont("Times", 18, QFont::Bold));
       }
    }


    QWidget * l_pWidgetPause = m_tableWidget->cellWidget(taskInfoListId,PauseHeader);
    QPushButton *l_pPauseButton ;
    if(l_pWidgetPause != NULL)
    {
        //qDebug() << "old one";
       l_pPauseButton = (QPushButton*)l_pWidgetPause;
    }
    else
    {
        if("needupdate" == m_taskInfoList.at(taskInfoListId)->comment)
        {
            l_pPauseButton = new QPushButton("Pause");
            m_tableWidget->setCellWidget(taskInfoListId, PauseHeader, l_pPauseButton);
            connect(l_pPauseButton, SIGNAL(clicked()),this,SLOT(OnBtnPauseClicked()));
        }
        else if("new" == m_taskInfoList.at(taskInfoListId)->comment)
        {

            l_pPauseButton = new QPushButton("Pause");
            m_tableWidget->setCellWidget(taskInfoListId, PauseHeader, l_pPauseButton);
            connect(l_pPauseButton, SIGNAL(clicked()),this,SLOT(OnBtnPauseClicked()));
        }
        else
        {
            l_pPauseButton = new QPushButton("Pause");
            m_tableWidget->setCellWidget(taskInfoListId, PauseHeader, l_pPauseButton);
            connect(l_pPauseButton, SIGNAL(clicked()),this,SLOT(OnBtnPauseClicked()));
        }
    }

    switch (m_taskInfoList.at(taskInfoListId)->state)
    {
    case stopState:
        //aaron 0327
        l_pDownloadButton->setEnabled(true);
        //l_pDownloadButton->setEnabled(false);
        l_pPauseButton->setEnabled(false);
        //m_tableWidget->setItem(taskInfoListId, stateHeader, new QTableWidgetItem(QIcon(":/images/stopstate.png"),""));
        break;
    case runningState:
        l_pDownloadButton->setEnabled(false);
         l_pPauseButton->setEnabled(true);
        //m_tableWidget->setItem(taskInfoListId, stateHeader, new QTableWidgetItem(QIcon(":/images/runningstate.png"),""));
        break;
    case finishedState:
    {
         l_pDownloadButton->setEnabled(false);
         l_pPauseButton->setEnabled(true);

         //l_pDownloadButton->setEnabled(false);
         //l_pPauseButton->setEnabled(false);

         //aaron 0327
         QString l_sSetupFileName = QString("%1/%2").arg(m_taskInfoList.at(taskInfoListId)->saveTo).arg(m_taskInfoList.at(taskInfoListId)->rename);
         QString l_sSilentParam = m_taskInfoList.at(taskInfoListId)->silentParam;

         Setup * mySetup = new Setup(l_sSetupFileName,l_sSilentParam , taskInfoListId);
         QThread *thread = new QThread;
         mySetup->moveToThread(thread);
         thread->start();
         connect(thread, SIGNAL(started()), mySetup, SLOT(vExecute()));
         connect(mySetup, SIGNAL(sig_vSetupfinish(int)),this, SLOT(slt_vSetupFinished(int)));
         connect(mySetup,SIGNAL(sig_vSetupfinish(int)),thread, SLOT(quit()));
         //mySetup->vExecute();
         m_taskInfoList.at(taskInfoListId)->state = installingState;
         //m_tableWidget->setItem(taskInfoListId, stateHeader, new QTableWidgetItem(QIcon(":/images/finishedstate.png"),""));
         break;
    }
    case pauseState:
        qDebug() << "pause state";
        l_pDownloadButton->setEnabled(true);
        l_pPauseButton->setEnabled(false);
        m_tableWidget->setItem(taskInfoListId, stateHeader, new QTableWidgetItem(QIcon(":/images/pausestate.png"),""));
        break;
    case errorState:
        qDebug() << "error state";
        l_pDownloadButton->setEnabled(true);
         l_pPauseButton->setEnabled(false);
        //m_tableWidget->setItem(taskInfoListId, stateHeader, new QTableWidgetItem(QIcon(":/images/errorstate.png"),""));
        break;
        //aaron 0327
    case installingState:
        //qDebug() << "installing state";
    {
        //QPushButton *l_pbDownloadNew = new QPushButton("Installing");
        //connect(l_pbDownloadNew, SIGNAL(clicked()),this,SLOT(OnBtnDownloadClicked()));

        //QLabel * labelDownload = new QLabel();
        //QVBoxLayout * layoutDownload = new QVBoxLayout();
        //layoutDownload->addWidget(l_pbDownloadNew);
        //labelDownload->setLayout(layoutDownload);
        //m_tableWidget->setCellWidget(taskInfoListId, DownloadHeader, labelDownload);

        //l_pbDownloadNew->setEnabled(false);


        l_pDownloadButton->setEnabled(false);
         l_pPauseButton->setEnabled(false);
        l_pDownloadButton->setText("Installing");


        break;
    }

    case doneState:
    {

        //QPushButton *l_pbDownloadNew = new QPushButton("Done");
        //connect(l_pbDownloadNew, SIGNAL(clicked()),this,SLOT(OnBtnDownloadClicked()));

        //QLabel * labelDownload = new QLabel();
        //QVBoxLayout * layoutDownload = new QVBoxLayout();
        //layoutDownload->addWidget(l_pbDownloadNew);
        //labelDownload->setLayout(layoutDownload);
        //m_tableWidget->setCellWidget(taskInfoListId, DownloadHeader, labelDownload);

        //l_pbDownloadNew->setEnabled(false);
        //l_pPauseButton->setEnabled(false);

        l_pDownloadButton->setEnabled(false);
        l_pPauseButton->setEnabled(false);
        l_pDownloadButton->setText("Done");

        break;
    }

    }

}

void AllApplicationDialog::slt_vSetupFinished(int iTaskId)
{
    m_taskInfoList.at(iTaskId)->state = doneState;
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

                downloadThread->deleteLater();

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
        //delete deleteTaskTimer;
        deleteTaskTimer->deleteLater();
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
        qDebug() << "new thread for download";
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
/*
void AllApplicationDialog::saveAllTask()
{
    QSettings settings("MPC","task");
    settings.clear();
    for (int taskInfoListId=0;taskInfoListId<m_taskInfoList.size();taskInfoListId++)
    {
        settings.beginGroup(QString("task%1").arg(taskInfoListId));
        //settings.setValue("url", m_taskInfoList.at(taskInfoListId)->url.toString());
        settings.setValue("saveTo", m_taskInfoList.at(taskInfoListId)->saveTo);
        settings.setValue("rename", m_taskInfoList.at(taskInfoListId)->rename);
        settings.setValue("comment", m_taskInfoList.at(taskInfoListId)->comment);
        settings.setValue("split", QString::number(m_taskInfoList.at(taskInfoListId)->splite));
        settings.setValue("user", m_taskInfoList.at(taskInfoListId)->user);
        settings.setValue("password", m_taskInfoList.at(taskInfoListId)->password);
        settings.endGroup();
    }
}*/
void AllApplicationDialog::saveTaskState(int taskInfoListId)
{
    //qDebug() << "1947";
    QString taskName = QString("%1/%2.task").arg(m_taskInfoList.at(taskInfoListId)->saveTo).arg(m_taskInfoList.at(taskInfoListId)->rename);



    if(m_taskInfoList.at(taskInfoListId)->state == finishedState
            || m_taskInfoList.at(taskInfoListId)->state == installingState
            || m_taskInfoList.at(taskInfoListId)->state == doneState)
    {
        //aaron 0326 delete *.task file after download finished
        if(QFile::exists(taskName))
        {
            QFile::remove(taskName);
            qDebug() << "delete" << taskName;
        }
        return;
    }

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
         //qDebug() << "2117 is ok";
    settings.beginGroup("file");
     //qDebug() << "2119 is ok";
    m_taskInfoList.at(taskInfoListId)->size = settings.value("size", 0).toLongLong();

    //aaron 0327
    //m_taskInfoList.at(taskInfoListId)->state = settings.value("state", 0).toInt();
    m_taskInfoList.at(taskInfoListId)->state = settings.value("state", m_taskInfoList.at(taskInfoListId)->state).toInt();

     //qDebug() << "2122 is ok";
    if (m_taskInfoList.at(taskInfoListId)->state==pauseState || m_taskInfoList.at(taskInfoListId)->state==runningState)
        m_taskInfoList.at(taskInfoListId)->state = stopState;
    settings.endGroup();
    //qDebug() << "2415";
    //qDebug() << "2122 is ok";
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
    qDebug()<<"taskInfoListId"<< taskInfoListId <<"taskThreadListId" << taskThreadListId ;

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

