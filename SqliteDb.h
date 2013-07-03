/*
********************************************************************************************
Discription: 主存数据库sqlite.

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
#ifndef MODELDB_H_
#define MODELDB_H_

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class  CSQLiteDb
{
public:
    CSQLiteDb()
    {
    }
    ~CSQLiteDb()
    {
    }
private:
    static  QSqlDatabase m_db;
public:
    static bool
    ConnectionDB( QString fileName )
    {
        if(fileName.compare("") != 0)
        {
            m_db = QSqlDatabase::addDatabase( "QSQLITE", "local_db" );
            m_db.setDatabaseName( fileName );
        }
        else
        {
            m_db = QSqlDatabase::addDatabase("QSQLITE");
            m_db.setDatabaseName(":memory:");

        }
        m_db.setConnectOptions( QLatin1String( "QSQLITE_BUSY_TIMEOUT=5000" ) );
        if ( !m_db.open() )
        {
            m_db.setConnectOptions();//去掉参数重新打开
            if ( !m_db.open() )
            {
                return false;
            }
        }
        if(fileName.compare("") == 0)
        {
            QSqlQuery query(m_db);
            if(!query.exec("CREATE TABLE Communication (CallNumber  bigint NOT NULL,  Type integer,  flow  char(64),  HEX  varchar(512), SendTime datetime);"))
            {
                return false;
            }
            if(!query.exec("CREATE TABLE OnlineTable (Callnumber bigint NOT NULL,  TerminalID integer,  IP  varchar(50),  L_Port  integer,  R_Port  integer, LogonTime  datetime, Updatetime  datetime,  location varchar(200) );"))

            {
                return false;
            }
        }
        return true;
    };
    static void
    DisConnectionDB()
    {
        m_db.close();
    }

    static QSqlDatabase*
    getDB()
    {
        return &m_db;
    };
};

int  getlistA()
{
	int softWareCount=0;  
	QSettings reg("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",QSettings::NativeFormat);  
	QStringList groupsList=reg.childGroups();  
	foreach(QString group,groupsList)  
	{  
		reg.beginGroup(group);  
		QString softWareName=reg.value("DisplayName").toString();  
		QString softWareName1=reg.value("Publisher").toString();  
		if(!softWareName.isEmpty())  
		{  
			qDebug() << softWareName;
			qDebug() << softWareName1;
			softWareCount++;  
		}  
		reg.endGroup();  
	}
	return 0;
}

#endif

