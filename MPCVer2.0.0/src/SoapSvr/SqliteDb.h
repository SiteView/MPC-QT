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
#include <QSettings>
#include <QStringList>
#include <QDebug>

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
		//m_db = QSqlDatabase::addDatabase("QSQLITE");
		//m_db.setDatabaseName(":memory:");
		m_db=QSqlDatabase::addDatabase("QMYSQL");
                //m_db.setHostName("localhost");
                m_db.setHostName("127.0.0.1");
                m_db.setDatabaseName("MpcSvr");
                m_db.setUserName("root");
                //m_db.setPassword("admin888");
        }
//        m_db.setConnectOptions( QLatin1String( "QSQLITE_BUSY_TIMEOUT=5000" ) );
        if ( !m_db.open() )
        {
            m_db.setConnectOptions();//去掉参数重新打开
            if ( !m_db.open() )
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
#endif

