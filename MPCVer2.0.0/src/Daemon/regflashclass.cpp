/*
********************************************************************************************
Discription: 注册表、文件夹、文件扫描等服务

Written By:		周志光
Date:			2013-07-03
Version:		1.0
Mail:			zhouklansman@gmail.com

Modified by:    shu-yuan
Modified Date:  2013-07-18
Version:        1.0.1
Discription:    add a function updateFieldValue, repair EstimatedSize、 DisplayIcon, formate path, delete unloaded software
********************************************************************************************
*/
#include "regflashclass.h"
#include "utils/tool.h"
#include <QFile>

RegFlashClass::RegFlashClass(/*QObject *parent*/)
    : QThread(/*parent*/)
{
}
void RegFlashClass::run()
{
    QString val1;
    QString val2;
    QString val3;
    QString val4;
    QString val5;
    QString val6;
    QString val7;
    QString val8;
    QString val9;
    QString val10;
    QDateTime setupdate;

    forever
    {
        sleep( 5 );
        QSqlQuery query(*m_SQLiteDb.getDB());
        QSettings reg("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",QSettings::NativeFormat);
        QStringList groupsList=reg.childGroups();
        m_SQLiteDb.getDB()->transaction();
        //query.prepare  ("delete from LocalAppInfor ;" );
        //if ( !query.exec() )
        //{
        //	qDebug(query.lastError().text().toLocal8Bit().data());
        //	continue;
        //}
        foreach(QString group,groupsList)
        {
            reg.beginGroup(group);
            val1=reg.value("DisplayName").toString();
            val10=reg.value("SystemComponent").toString();
            if(!val1.isEmpty()  && (val10.isEmpty() || val10.compare("0") == 0) &&
                    val1.indexOf("Security Update for Microsoft") == -1 &&  val1.indexOf("(KB") == -1 )
            {
                ;// 空处理
            }
            else
            {
                // 安全更新和系统组件不在管理范围
                reg.endGroup();
                continue;
            }
            if (checkInDb(val1,*m_SQLiteDb.getDB()))
            {
                reg.endGroup();
                continue;
            }
            val2=reg.value("UninstallString").toString();
            val3=reg.value("DisplayIcon").toString();
            val4=reg.value("DisplayVersion").toString();
            val5=reg.value("URLInfoAbout").toString();
            val6=reg.value("Publisher").toString();
            val7=reg.value("InstallLocation").toString();
            val8=reg.value("InstallDate").toString();
            val9=reg.value("EstimatedSize").toString();
            if(!val8.isEmpty() || val8.compare("") != 0 )
                setupdate =  QDateTime::fromString(val8,"yyyyMMdd");
            QString path ;
            bool rethav = false;
            if(val7.isEmpty() || val7.compare("") == 0 )
            {
                rethav	= HavePath(val2,path); // 使用UninstallString 修补
                if(!rethav)
                    rethav = HavePath(val3,path); // 使用DisplayIcon 修补
            }
            if(!val1.isEmpty()  && (val10.isEmpty() || val10.compare("0") == 0) && val1.indexOf("Security Update for Microsoft") == -1 &&  val1.indexOf("(KB") == -1 )  // 锟斤拷全锟斤拷锟铰猴拷系统锟斤拷锟斤拷锟斤拷锟节癸拷锟斤拷围
            {

                query.prepare("insert into LocalAppInfor (DisplayName,UninstallString,DisplayIcon,DisplayVersion,URLInfoAbout,Publisher,InstallLocation,SetupTime,EstimatedSize) values(?,?,?,?,?,?,?,?,?)");
                query.addBindValue(val1);
                query.addBindValue(val2);
                query.addBindValue(val3);
                query.addBindValue(val4);
                query.addBindValue(val5);
                query.addBindValue(val6);
                if(rethav)
                    query.addBindValue(path);// 修补安装路径
                else
                    query.addBindValue(val7);
                query.addBindValue(setupdate);
                query.addBindValue(val9);
            }
            if ( !query.exec() )
            {
                //qDebug(query.lastError().text().toLocal8Bit().data());
            }
            reg.endGroup();
        }
        query.finish();
        m_SQLiteDb.getDB()->commit();
        UpdateInfo();
    }
}
void RegFlashClass::UpdateInfo()
{
    QSqlQuery SQLiteQuery( *m_SQLiteDb.getDB() );
    QSqlQuery updateQuery( *m_SQLiteDb.getDB() );
    m_SQLiteDb.getDB()->transaction();
    if ( !SQLiteQuery.exec( "select DisplayName, InstallLocation, DisplayVersion, SetupTime, DisplayIcon, UninstallString from LocalAppInfor ;" ) )
    {
        qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
    }
    while ( SQLiteQuery.next() )
    {
        QVariant val1 = SQLiteQuery.value(0);
        QVariant val2 = SQLiteQuery.value(1);
        QVariant val3 = SQLiteQuery.value(2);
        QVariant val4 = SQLiteQuery.value(3);
        QVariant val5 = SQLiteQuery.value(4);
        QVariant val6 = SQLiteQuery.value(5);
        QString installPathStr = val2.toString();
        QString iconStr = val5.toString();
        QString uninstallPathStr = val6.toString();

        // 修补安装日期 访问信息
        if(/*(val4.toString().compare("") == 0) &&*/ installPathStr.compare("") )
        {
            QFileInfo pathInfo(installPathStr);
            updateFieldValue(updateQuery, "SetupTime", pathInfo.created(), val1);
        }

       // 修补版本号
        if(val3.toString().compare("") == 0)
        {
            QStringList   liststr =  val1.toString().split(" ");
            QString version ;
            QString displayNane = val1.toString();
            bool ret = TrimVersion(displayNane,version);
            if(ret)
            {
                updateFieldValue(updateQuery, "DisplayVersion", version, val1);
            }
        }

        /* add by shu-yuan
         */

        // repair the size of installed software
        if (installPathStr.compare("")) {
            long long size = 0;
            size = getFolderSize(installPathStr);
            updateFieldValue(updateQuery, "EstimatedSize", size, val1);
        }


		// repair the icon path
//		QFileInfo iconInfo(iconStr);
//		bool bIconExist = iconInfo.exists();
//		bool bIsPngFile = (iconInfo.suffix() == "png") ? true : false;

//		if (bIconExist && !bIsPngFile) {
//			// get icon path
//			iconStr = ::getIcon(iconStr);
//		}
//		if (!bIconExist) {
//			// repair from install location
//			if (installPathStr.compare("")) {
//				QFileInfo fileInfo(installPathStr);
//				QString fileBaseName = fileInfo.baseName();
//				iconStr = installPathStr + "\\" + fileBaseName + ".exe";
//			}
//		}
//		QFileInfo fileInfoIcon(iconStr);
//		if (fileInfoIcon.exists()) {
//			updateFieldValue(updateQuery, "DisplayIcon", iconStr, val1);
//		}


        // delete the unloaded software over 2 days
        if (!updateQuery.exec("delete from LocalAppInfor where UninstallTime is not null and UninstallTime < datetime('now','localtime','-2 days')")) {
            qDebug(updateQuery.lastError().text().toLocal8Bit().data());
        }


        // formate install path ,uninstall path and displayicon path
        if (installPathStr.compare("")) {
            unifyPathFormat(installPathStr);
            updateFieldValue(updateQuery, "InstallLocation", installPathStr, val1);
        }
        if (uninstallPathStr.compare("")) {
            unifyPathFormat(uninstallPathStr);
            updateFieldValue(updateQuery, "UninstallString", uninstallPathStr, val1);
        }
        if (iconStr.compare("")) {
            unifyPathFormat(iconStr);
            updateFieldValue(updateQuery, "DisplayIcon", iconStr, val1);
        }

        // --add end: shu-yuan

    }
    SQLiteQuery.finish();
    m_SQLiteDb.getDB()->commit();
}

bool RegFlashClass::updateFieldValue(QSqlQuery sqlQuery, const QString &fieldName,
                                     const QVariant &value, const QVariant &var)
{
    sqlQuery.prepare(QString("update LocalAppInfor set %1 = ? where  DisplayName = ?").arg(fieldName));
    sqlQuery.addBindValue(value);
    sqlQuery.addBindValue(var);
    if (!sqlQuery.exec()) {
        qDebug(sqlQuery.lastError().text().toLocal8Bit().data());
        return false;
    }

    return true;
}

RegFlashClass::~RegFlashClass()
{

}
