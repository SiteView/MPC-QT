#include "initialize.h"
#include "httphandle.h"
#define SERVERINI "config/server.ini"
#define LOCALINI "config/local.ini"
#define UPDATEINI "config/update.ini"
#define CONFIGINI "config.ini"
#define SERVERURL "http://192.168.9.27:8080/Web-Server/server.ini"

InitializeInfo::InitializeInfo(QObject *parent) :
    QObject(parent)
{
    unsigned char key[] = "siteview20130326";

    m_pAES = new AES(key);
    m_pInformationAnalyze = new InformationAnalyze(SERVERINI, LOCALINI, UPDATEINI);
    createUpdateFile();
    //while(!downloadfinish)
    //{
    //    qDebug() << downloadfinish;
    //}
}


void InitializeInfo::readTaskState(int taskInfoListId)
{
    //qDebug() << "2332";
    QString taskName = QString("%1/%2.task").arg(g_taskInfoList.at(taskInfoListId)->saveTo).arg(g_taskInfoList.at(taskInfoListId)->rename);


    QSettings settings(taskName, QSettings::IniFormat);
         //qDebug() << "2117 is ok";
    settings.beginGroup("file");
     //qDebug() << "2119 is ok";
    g_taskInfoList.at(taskInfoListId)->size = settings.value("size", 0).toLongLong();

    //aaron 0327
    //m_taskInfoList.at(taskInfoListId)->state = settings.value("state", 0).toInt();
    g_taskInfoList.at(taskInfoListId)->state = settings.value("state", g_taskInfoList.at(taskInfoListId)->state).toInt();

     //qDebug() << "2122 is ok";
    if (g_taskInfoList.at(taskInfoListId)->state==pauseState || g_taskInfoList.at(taskInfoListId)->state==runningState)
        g_taskInfoList.at(taskInfoListId)->state = stopState;
    settings.endGroup();
    //qDebug() << "2415";
    //qDebug() << "2122 is ok";
    settings.beginGroup("notDownloadsList");
    int notDownloadListId=0;
    QStringList notDownloadListIdValueString;
    QStringList defaultValue;
    defaultValue<<"-1"<<"-1";
    g_taskInfoList.at(taskInfoListId)->notDownloadList.clear();

    //qDebug() << "2425";
    while (true)
    {
        QString notDownloadListIdString = QString("notDownloadId%1").arg(notDownloadListId);
        notDownloadListIdValueString.clear();
        notDownloadListIdValueString<<settings.value(notDownloadListIdString, defaultValue).toStringList();
        if (notDownloadListIdValueString.at(0) == "-1") break;
        g_notDownload = new NotDownload;
        g_notDownload->taskThreadListId = -1;
        g_notDownload->startPosition = notDownloadListIdValueString.at(0).toLongLong();
        g_notDownload->endPosition = notDownloadListIdValueString.at(1).toLongLong();
        g_taskInfoList.at(taskInfoListId)->notDownloadList.append(g_notDownload);
        notDownloadListId++;
    }
    settings.endGroup();
}

void InitializeInfo::createTaskInformation()
{           
    g_taskInfoList.clear();
    //g_newTaskInfoList.clear();
    //g_normalTaskInfoList.clear();

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
    int splitInt = l_RegSettings.value("split",2).toInt();
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

        //aaron 0408 uninstall
        QString l_sUninstallString = l_psetUpdate.value(l_sSoftwareName + "/uninstallstring").toString();


        if (urlString.isEmpty())
        {
            qDebug() << "url is empty, break";
            break;
        }

        TaskInformation *taskInfo;
        taskInfo = new TaskInformation;

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

        //aaron 0408
        taskInfo->uninstallString = l_sUninstallString;

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

        g_taskInfoList.append(taskInfo);

        //qDebug() << "328 is ok";
        readTaskState(taskInfoListId);
        //qDebug() << "326 is ok";
        qint64 taskInfoListIdNotCompleted = 0;
        for (int notDownloadListId=0;notDownloadListId<g_taskInfoList.at(taskInfoListId)->notDownloadList.size();notDownloadListId++)
        {
            taskInfoListIdNotCompleted += g_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->endPosition - g_taskInfoList.at(taskInfoListId)->notDownloadList.at(notDownloadListId)->startPosition;
        }
        g_taskInfoList.at(taskInfoListId)->completed = g_taskInfoList.at(taskInfoListId)->size - taskInfoListIdNotCompleted;
        g_taskInfoList.at(taskInfoListId)->lastCompleted = g_taskInfoList.at(taskInfoListId)->completed;
        //qDebug() << m_taskInfoList.at(taskInfoListId)->size << "what's wrong?";
        taskInfoListId++;
    }
}

void InitializeInfo::createUpdateFile()
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

    //在下载安装完毕后更新显示
    connect(m_pHttpHandle, SIGNAL(sig_vSetupfinish()), this, SLOT(slt_vUpdate()));
    vUpdate();
}

void InitializeInfo::slt_vUpdate()
{
    m_pInformationAnalyze->vRefresh();
    createTaskInformation();

    emit sig_vUpdate();

}

void InitializeInfo::vUpdate()
{
    m_pInformationAnalyze->vRefresh();
    createTaskInformation();
}

QList <TaskInformation *> InitializeInfo::GetUpdateTaskInformation()
{
    g_updateTaskInfoList.clear();

    for(int taskInfoListId=0;taskInfoListId<g_taskInfoList.size();taskInfoListId++)
    {

        if("needupdate" == g_taskInfoList.at(taskInfoListId)->comment)
        {
            g_updateTaskInfoList.append(g_taskInfoList.at(taskInfoListId));
        }
    }

    return g_updateTaskInfoList;
}
QList <TaskInformation *> InitializeInfo::GetNewTaskInformation()
{
    g_newTaskInfoList.clear();

    for(int taskInfoListId=0;taskInfoListId<g_taskInfoList.size();taskInfoListId++)
    {
        if("new" == g_taskInfoList.at(taskInfoListId)->comment)
        {
            g_newTaskInfoList.append(g_taskInfoList.at(taskInfoListId));
        }
    }
    return g_newTaskInfoList;
}
QList <TaskInformation *> InitializeInfo::GetNormalTaskInformation()
{
    g_normalTaskInfoList.clear();

    for(int taskInfoListId=0;taskInfoListId<g_taskInfoList.size();taskInfoListId++)
    {
        if("normal" == g_taskInfoList.at(taskInfoListId)->comment)
        {
            g_normalTaskInfoList.append(g_taskInfoList.at(taskInfoListId));
        }
    }

    return g_normalTaskInfoList;
}

