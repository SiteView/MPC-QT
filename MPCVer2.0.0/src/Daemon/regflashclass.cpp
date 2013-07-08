/*
********************************************************************************************
Discription: 注册表、文件夹、文件扫描等服务

Written By:		周志光
Date:			2013-07-03
Version:		1.0
Mail:			zhouklansman@gmail.com

Modified by:
Modified Date:
Version:
Discription:
********************************************************************************************
*/

#include "regflashclass.h"

RegFlashClass::RegFlashClass(QObject *parent)
	: QThread(parent)
{
	struct soap add_soap;

	add_soap.connect_timeout = 5; //单位是秒
	add_soap.send_timeout = 5;
	add_soap.recv_timeout = 5;
	soap_init(&add_soap);
	_MPC__npRequestRequest inputdata;
	_MPC__npRequestResponse outdata;
	inputdata.DisplayName = std::string("shfsdhf");
	int ret = 0;
	ret = soap_call_MPC__npRequest(&add_soap,"127.0.0.1:6600","",inputdata,outdata);

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
	forever
	{
		sleep( 5 );
		QSqlQuery query(*m_SQLiteDb.getDB());
		QSettings reg("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",QSettings::NativeFormat);  
		QStringList groupsList=reg.childGroups();  
		m_SQLiteDb.getDB()->transaction();
		query.prepare  ("delete from LocalAppInfor ;" );
		if ( !query.exec() )
		{
			qDebug(query.lastError().text().toLocal8Bit().data());
			continue;
		}
		foreach(QString group,groupsList)  
		{  
			reg.beginGroup(group);  
			val1=reg.value("DisplayName").toString();  
			val2=reg.value("UninstallString").toString();
			val3=reg.value("DisplayIcon").toString();  
			val4=reg.value("DisplayVersion").toString();  
			val5=reg.value("URLInfoAbout").toString();  
			val6=reg.value("Publisher").toString();  
			val7=reg.value("InstallLocation").toString();  
			val8=reg.value("SetupTime").toString();
			if(val8.compare("")!=0)
				qDebug() << val8;
			val9=reg.value("EstimatedSize").toString();  
			if(!val1.isEmpty()  && val1.indexOf("Security Update")== -1 && val1.indexOf("(KB") == -1  && !val2.isEmpty() )  
			{  
				query.prepare("insert into LocalAppInfor (DisplayName,UninstallString,DisplayIcon,DisplayVersion,URLInfoAbout,Publisher,InstallLocation,SetupTime,EstimatedSize) values(?,?,?,?,?,?,?,?,?)");
				query.addBindValue(val1);
				query.addBindValue(val2);
				query.addBindValue(val3);
				query.addBindValue(val4);
				query.addBindValue(val5);
				query.addBindValue(val6);
				query.addBindValue(val7);
				query.addBindValue(val8);
				query.addBindValue(val9);
			}
			if ( !query.exec() )
			{
				//qDebug(query.lastError().text().toLocal8Bit().data());
			}
			reg.endGroup();  
		}
		m_SQLiteDb.getDB()->commit();
		UpdateInfo();
	}
}
void RegFlashClass::UpdateInfo()
{
	QSqlQuery SQLiteQuery( *m_SQLiteDb.getDB() );
	QSqlQuery updateQuery( *m_SQLiteDb.getDB() );
	m_SQLiteDb.getDB()->transaction();
	if ( !SQLiteQuery.exec( "select DisplayName,InstallLocation,DisplayVersion from LocalAppInfor ;" ) )
	{
		qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
	}
	while ( SQLiteQuery.next() )
	{
		QVariant val1 = SQLiteQuery.value(0);
		QVariant val2 = SQLiteQuery.value(1);
		QVariant val3 = SQLiteQuery.value(2);
		QString pahtstr = val2.toString();
		// 修补安装信息 访问信息 
		if(pahtstr.compare("") !=0)
		{
			QFileInfo pathInfo(pahtstr);
			QDir dir(pahtstr);
			QFileInfoList list = dir.entryInfoList();
			long long size = 0 ;
			for (int i = 0; i < list.size(); ++i) 
			{
				QFileInfo fileInfo = list.at(i); // 下级目录需细化
				size+=fileInfo.size();
			}
			updateQuery.prepare  ("update LocalAppInfor set SetupTime = ? ,EstimatedSize = ? where  DisplayName = ?" );
			updateQuery.addBindValue(pathInfo.created());
			updateQuery.addBindValue(size);
			updateQuery.addBindValue(val1);
			if ( !updateQuery.exec() )
			{
				qDebug(updateQuery.lastError().text().toLocal8Bit().data());
			}
		}
		// 修补版本号
		if(val3.toString().compare("") == 0)
		{
			QStringList   liststr =  val1.toString().split(" ");
			QString lastStr = liststr.at(liststr.size()-1).toUpper();
			lastStr = lastStr.replace("V","");
			QRegExp reg ("^([0-9])(\\.)?");
			if(!reg.indexIn(lastStr))
			{
				updateQuery.prepare  ("update LocalAppInfor set DisplayVersion = ?  where  DisplayName = ?" );
				updateQuery.addBindValue(lastStr);
				updateQuery.addBindValue(val1);
				if ( !updateQuery.exec() )
				{
					qDebug(updateQuery.lastError().text().toLocal8Bit().data());
				}
				//qDebug() << val1 << lastStr;
			}
		}
	}
	SQLiteQuery.finish();
	m_SQLiteDb.getDB()->commit();
}
RegFlashClass::~RegFlashClass()
{

}
