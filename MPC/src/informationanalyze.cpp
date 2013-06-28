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
#include "aesclass.h"
#define SERVER "d:/server.ini"
#define LOCAL "d:/local.ini"
#define UPDATE "d:/update.ini"

InformationAnalyze::InformationAnalyze()
{
     m_sServerini = SERVER;
     m_sLocalini = LOCAL;
     m_sUpdateini = UPDATE;
     m_aesTool = new AesClass(16, "siteview0313");
}

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

    //SERVERINI里的软件列表
    QSettings *l_psetServer = new QSettings(m_sServerini, QSettings::IniFormat) ;
    QString l_sServerSoftware = l_psetServer->value("/LIST/Software").toString();
    QStringList serverSoftwareList = l_sServerSoftware.split("|");

    if(10 > l_sServerSoftware.size())
    {
        qDebug() << "Serverini file is empty!";
        return;
    }

    bDuplicateFile(m_sServerini, m_sUpdateini);

    //LOCALINI里的软件列表
    QSettings *l_setLocal = new QSettings(m_sLocalini, QSettings::IniFormat) ;
    QString l_sLocalSoftware = l_setLocal->value("/LIST/Software").toString();
    QStringList l_slLocalSoftwareList = l_sLocalSoftware.split("|");

    QString  l_sNewSoftware;
    QString  l_sNeedUpdateSoftware;
    QString  l_sNormalSoftware;

    QSettings *l_psetUpdate = new QSettings(m_sUpdateini, QSettings::IniFormat);
    QString  l_sStatus;

    //依次查找服务器上的软件是否存在于本地，如存在则需要比较版本信息
    for (int i = 0; i < serverSoftwareList.size(); ++i)
    {
        QString l_sSoftwareName = serverSoftwareList.at(i);

        if("" == l_sSoftwareName)
        {
            break;
        }

        QStringList::Iterator l_iterator = qFind(l_slLocalSoftwareList.begin(), l_slLocalSoftwareList.end(), l_sSoftwareName);
        l_sStatus = l_sSoftwareName + "/status";

        //不存在
        if((l_slLocalSoftwareList.end() == l_iterator))
        {
            l_sNewSoftware += (l_sSoftwareName + "|");

            l_psetUpdate->setValue(l_sStatus, "new");
        }
        //存在，比较版本信息
        else
        {
            QString l_sVersionServer;
            QString l_sVersionLocal;
            QString l_sVersion= l_sSoftwareName + "/version";

            l_sVersionServer = l_psetServer->value(l_sVersion).toString();
            l_sVersionLocal = l_setLocal->value(l_sVersion).toString();

            if(l_sVersionServer == l_sVersionLocal)
            {
                l_sNormalSoftware += (l_sSoftwareName + "|");
                l_psetUpdate->setValue(l_sStatus, "normal");
            }
            else
            {
                l_sNeedUpdateSoftware += (l_sSoftwareName + "|");
                l_psetUpdate->setValue(l_sStatus, "needupdate");
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

    l_psetUpdate->setValue("list/new", l_sNewSoftware);

    l_psetUpdate->setValue("list/normal", l_sNormalSoftware);

    l_psetUpdate->setValue("list/needupdate", l_sNeedUpdateSoftware);


    delete l_psetServer;
    l_psetServer = 0;
    delete l_setLocal;
    l_setLocal = 0;
    delete l_psetUpdate;
    l_psetUpdate = 0;
}



//从注册表获取本地软件信息，按格式存放在LOCALINI中
void InformationAnalyze::vInitLocalInfomation()
{
    QString l_sLocalSoftware;

    if(QFile::exists(m_sLocalini))
    {
        QFile::remove(m_sLocalini);
    }
    QFile l_fLocaliniFile(m_sLocalini);
    l_fLocaliniFile.open(QFile::ReadOnly);
    l_fLocaliniFile.close();

    //createLocalini->mkdir(localini);

    QSettings *l_psetLocal = new QSettings(m_sLocalini, QSettings::IniFormat) ;
    l_psetLocal->setIniCodec(QTextCodec::codecForName("GB2312"));
    QSettings l_setReg("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",QSettings::NativeFormat);
    QStringList groupsList=l_setReg.childGroups();
    foreach(QString group,groupsList)
    {
        l_setReg.beginGroup(group);
        QString l_sSoftwareName=l_setReg.value("DisplayName").toString();
        QString l_sSoftwareVersion = l_setReg.value("DisplayVersion").toString();
        if(!l_sSoftwareName.isEmpty())
        {
            l_sLocalSoftware += l_sSoftwareName + "|";
            //qDebug() << softwareName << softwareVersion <<softWareCount++ << endl;
            l_psetLocal->setValue(l_sSoftwareName + "/version", l_sSoftwareVersion);
        }
        l_setReg.endGroup();
    }
    l_psetLocal->setValue("list/software", l_sLocalSoftware);

    delete l_psetLocal;
    l_psetLocal = 0;


}

//用来设置SERVERINI文件，URL信息会加密保存
//Format : serverInformation << SoftwareName << URL << Version
void InformationAnalyze::vSetServerInformation(const QString& sFileName, const QStringList &serverInformation)
{
    QSettings *l_sUpdate = new QSettings(sFileName, QSettings::IniFormat);

    QFile l_fServeriniFile(sFileName);
    l_fServeriniFile.open(QFile::ReadOnly);
    l_fServeriniFile.close();

    QString l_sSoftwareName = serverInformation.at(0);

    QString l_sSoftwareNameTotal = l_sUpdate->value("list/software").toString();

    if(l_sSoftwareNameTotal.left(l_sSoftwareName.size()) == l_sSoftwareName
            || l_sSoftwareNameTotal.contains("|" + l_sSoftwareName + "|"))
    {
        ;
    }
    else
    {
        l_sSoftwareNameTotal += l_sSoftwareName + "|";
        l_sUpdate->setValue("list/software", l_sSoftwareNameTotal);
    }


    QString l_sUrl = serverInformation.at(1);
    QString l_sEncryptUrl;
    m_aesTool->OnAesEncrypt(l_sUrl, l_sEncryptUrl);

    QString l_sVersion = serverInformation.at(2);

    l_sUpdate->setValue(l_sSoftwareName + "/version", l_sVersion);
    l_sUpdate->setValue(l_sSoftwareName + "/url", l_sEncryptUrl);


}

//将LOCALINI中的信息读取到QStringList中以供使用
void InformationAnalyze::vGetUpdateInformation(QStringList &updateInformation)
{

    QSettings *l_sUpdate = new QSettings(m_sUpdateini, QSettings::IniFormat);

    QString l_sUpdateSoftware = l_sUpdate->value("/LIST/Software").toString();

    QStringList l_slUpdateSoftwareList = l_sUpdateSoftware.split("|");
    QString l_sUrl;
    QString l_sUncryptUrl;
    QString l_sVersion;
    QString l_sStatus;
    QString l_sSoftwareName;


    for(int i = 0; i < l_slUpdateSoftwareList.size(); i++)
    {
         l_sSoftwareName = l_slUpdateSoftwareList.at(i);
         if("" == l_sSoftwareName)
         {
             break;
         }
         l_sUrl = l_sUpdate->value(l_sSoftwareName+"/url").toString();
         m_aesTool->OnAesUncrypt(l_sUrl, l_sUncryptUrl);

         l_sVersion = l_sUpdate->value(l_sSoftwareName + "/version").toString();
         l_sStatus = l_sUpdate->value(l_sSoftwareName + "/status").toString();

         updateInformation << l_sSoftwareName + "|" + l_sStatus +"|" + l_sVersion + "|" +l_sUncryptUrl;
    }
    delete l_sUpdate;
    l_sUpdate = 0;
}


void InformationAnalyze::vRefresh()
{
    vInitLocalInfomation();
    vCompare();
}
