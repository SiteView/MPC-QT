/*
********************************************************************************************
Discription: 公共工具函数.

Written By:		周志光
Date:			2013-07-10
Version:		1.0
Mail:			zhouklansman@gmail.com

Modified by:
Modified Date:
Version:
Discription:
********************************************************************************************
*/
#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QDir>
#include <QFileInfo>

#include "SqliteDb.h"

// 去掉或取得软件明尾部的版本号。
bool TrimVersion( QString &DisplayName ,QString & version)
{
	QStringList   liststr =  DisplayName.split(" ");
	version = liststr.at(liststr.size()-1);
	version = version.replace("V","");
	version = version.replace("v","");
	QRegExp reg ("^([0-9])(\\.)?");
	QString newdisplayname;
	if(!reg.indexIn(version))
	{
		 newdisplayname = DisplayName.replace(version,"");
		 return true;
	}
	return false;
}
bool HavePath (QString InputStr,QString &path )
{
	QString tmpstr = InputStr;
	QString tmppath ;

	QStringList listStr = tmpstr.split(",");
	for(int i = 0 ; i< listStr.size();i++)
	{
		tmppath = listStr.at(i);
		tmppath = tmppath.replace("\"","");
		QFileInfo info(tmppath);
		if( info.exists())
		{
		    path = info.path();
			return true;
		}
	}
	return false;
}

bool checkInDb (QString InputStr ,QSqlDatabase db)
{
	QSqlQuery SQLiteQuery( db );
	bool ret = false;
	db.transaction();
	SQLiteQuery.prepare("select DisplayName from LocalAppInfor where DisplayName = ? ;");
	SQLiteQuery.addBindValue(InputStr);
	if ( !SQLiteQuery.exec(  ) )
	{
		qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
	}
	while ( SQLiteQuery.next() )
	{
		ret = true;
	}
	return ret;
}