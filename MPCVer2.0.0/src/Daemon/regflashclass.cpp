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
#include "tool.h"

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
			if(!val1.isEmpty()  && (val10.isEmpty() || val10.compare("0") == 0) && val1.indexOf("Security Update for Microsoft") == -1 &&  val1.indexOf("(KB") == -1 ) 
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
			if(!val1.isEmpty()  && (val10.isEmpty() || val10.compare("0") == 0) && val1.indexOf("Security Update for Microsoft") == -1 &&  val1.indexOf("(KB") == -1 )  // 安全更新和系统组件不在管理范围
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
		m_SQLiteDb.getDB()->commit();
		UpdateInfo();
	}
}
void RegFlashClass::UpdateInfo()
{
	QSqlQuery SQLiteQuery( *m_SQLiteDb.getDB() );
	QSqlQuery updateQuery( *m_SQLiteDb.getDB() );
	m_SQLiteDb.getDB()->transaction();
	if ( !SQLiteQuery.exec( "select DisplayName,InstallLocation,DisplayVersion, SetupTime  from LocalAppInfor ;" ) )
	{
		qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
	}
	while ( SQLiteQuery.next() )
	{
		QVariant val1 = SQLiteQuery.value(0);
		QVariant val2 = SQLiteQuery.value(1);
		QVariant val3 = SQLiteQuery.value(2);
		QVariant val4 = SQLiteQuery.value(3);
		QString pahtstr = val2.toString();
		// 修补安装日期 访问信息 
		if((val4.toString().compare("") == 0) && pahtstr.compare("") )
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
			QString version ;
			QString displayNane = val1.toString();
			bool ret = TrimVersion(displayNane,version);
			if(ret)
			{
				updateQuery.prepare  ("update LocalAppInfor set DisplayVersion = ?  where  DisplayName = ?" );
				updateQuery.addBindValue(version);
				updateQuery.addBindValue(val1);
				if ( !updateQuery.exec() )
				{
					qDebug(updateQuery.lastError().text().toLocal8Bit().data());
				}
			}
		}
		// 删除已经卸载 超过一天的软件
	}
	SQLiteQuery.finish();
	m_SQLiteDb.getDB()->commit();
}
RegFlashClass::~RegFlashClass()
{

}
