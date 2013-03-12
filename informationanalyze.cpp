#include "informationanalyze.h"
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QIODevice>
#include <QTextCodec>

bool InformationAnalyze::bDuplicateFile(const QString &source, const QString &target)
{
    if(QFile::exists(target))
    {
        QFile::remove(target);
    }

    return QFile::copy(source, target);
}

//比较SERVERINI文件与LOCALINI文件，得到UPDATEINI文件，包含软件是否存在，是否需要升级，升级文件链接等
void InformationAnalyze::vCompare()
{
    bDuplicateFile(m_sServerini, m_sUpdateini);

    //SERVERINI里的软件列表
    QSettings *server = new QSettings(m_sServerini, QSettings::IniFormat) ;
    QString serverSoftware = server->value("/LIST/Software").toString();
    QStringList serverSoftwareList = serverSoftware.split("|");

    qDebug()<<serverSoftware;

    //LOCALINI里的软件列表
    QSettings *local = new QSettings(m_sLocalini, QSettings::IniFormat) ;
    QString localSoftware = local->value("/LIST/Software").toString();
    QStringList localSoftwareList = localSoftware.split("|");

    qDebug() << localSoftware << endl;

    QString  newSoftware;
    QString  needUpdateSoftware;
    QString  normalSoftware;

    QSettings *update = new QSettings(m_sUpdateini, QSettings::IniFormat);
    QString  status;

    //依次查找服务器上的软件是否存在于本地，如存在则需要比较版本信息
    for (int i = 0; i < serverSoftwareList.size(); ++i)
    {
        QString softwareName = serverSoftwareList.at(i);
        QStringList::Iterator iterator = qFind(localSoftwareList.begin(), localSoftwareList.end(), softwareName);
        status = softwareName + "/status";

        //不存在
        if((localSoftwareList.end() == iterator))
        {
            newSoftware += (softwareName + "|");

            update->setValue(status, "new");
        }
        //存在，比较版本信息
        else
        {
            QString versionServer;
            QString versionLocal;
            QString version= softwareName + "/version";

            versionServer = server->value(version).toString();
            versionLocal = local->value(version).toString();
            qDebug() << versionServer << endl;
            qDebug() << versionLocal << endl;

            if(versionServer == versionLocal)
            {
                normalSoftware += (softwareName + "|");
                update->setValue(status, "normal");
            }
            else
            {
                needUpdateSoftware += (softwareName + "|");
                update->setValue(status, "needupdate");
            }
        }
    }


    //依次查找本地软件是否在服务器上，可做优化，暂时不需要
    /*QString unknowSoftware;
    for(int i = 0; i < localSoftwareList.size(); i++)
    {
        QString softwareName = localSoftwareList.at(i);
        QStringList::Iterator iterator = qFind(serverSoftwareList.begin(), serverSoftwareList.end(), softwareName);


        if(serverSoftwareList.end() == iterator)
        {
            unknowSoftware += softwareName;

            QString url = local->value(softwareName+"/url").toString();
            QString version = local->value(softwareName + "/version").toString();

            update->setValue(softwareName+"/url", url);
            update->setValue(softwareName + "/version", version);
            update->setValue(softwareName + "/status", "unknow");


            qDebug() << softwareName << endl;
            qDebug() << version << endl;
            //out << softwareName <<"       " << version << endl;
        }
    }*/

    update->setValue("list/new", newSoftware);
     qDebug() << newSoftware << endl;
    update->setValue("list/normal", normalSoftware);
    qDebug() << normalSoftware << endl;
    update->setValue("list/needupdate", needUpdateSoftware);
    qDebug() << needUpdateSoftware << endl;

    delete server;
    server = 0;
    delete local;
    local = 0;
    delete update;
    update = 0;
}



//从注册表获取本地软件信息，按格式存放在LOCALINI中
void InformationAnalyze::vInitLocalInfomation()
{
    QString localSoftware;

    if(QFile::exists(m_sLocalini))
    {
        QFile::remove(m_sLocalini);
    }
    QFile localiniFile(m_sLocalini);
    localiniFile.open(QFile::ReadOnly);
    localiniFile.close();

    //createLocalini->mkdir(localini);

    QSettings *local = new QSettings(m_sLocalini, QSettings::IniFormat) ;
    local->setIniCodec(QTextCodec::codecForName("GB2312"));
    QSettings reg("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",QSettings::NativeFormat);
    QStringList groupsList=reg.childGroups();
    foreach(QString group,groupsList)
    {
        reg.beginGroup(group);
        QString softwareName=reg.value("DisplayName").toString();
        QString softwareVersion = reg.value("DisplayVersion").toString();
        if(!softwareName.isEmpty())
        {
            localSoftware += softwareName + "|";
            //qDebug() << softwareName << softwareVersion <<softWareCount++ << endl;
            local->setValue(softwareName + "/version", softwareVersion);
        }
        reg.endGroup();
    }
    local->setValue("list/software", localSoftware);

    delete local;
    local = 0;


}


//将LOCALINI中的信息读取到QStringList中以供使用
void InformationAnalyze::vGetUpdateInformation(QStringList &updateInformation)
{

    QSettings *update = new QSettings(m_sUpdateini, QSettings::IniFormat);

    QString updateSoftware = update->value("/LIST/Software").toString();

    qDebug() << updateSoftware;
    QStringList updateSoftwareList = updateSoftware.split("|");
    QString url;
    QString version;
    QString status;
    QString softwareName;


    for(int i = 0; i < updateSoftwareList.size(); i++)
    {
         softwareName = updateSoftwareList.at(i);
         //qDebug() << softwareName;
         url = update->value(softwareName+"/url").toString();
         version = update->value(softwareName + "/version").toString();
         status = update->value(softwareName + "/status").toString();

         updateInformation << softwareName + "|" + status +"|" + version + "|" +url;
         qDebug() << updateInformation.at(i);

    }
    delete update;
    update = 0;
}


void InformationAnalyze::vRefresh()
{
    vInitLocalInfomation();
    vCompare();
}
