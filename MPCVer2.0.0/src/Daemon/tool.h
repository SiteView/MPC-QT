/*
********************************************************************************************
Discription: 公共工具函数.

Written By:		周志光
Date:			2013-07-10
Version:		1.0
Mail:			zhouklansman@gmail.com

Modified by:    shu-yuan
Modified Date:  2013-07-18
Version:        1.0.1
Discription:    add function getFolderSize and unifyPathFormat
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

/* add by shu-yuan
 */
// get the folder size by recursion
long long getFolderSize(const QString &filePath) {
    long long sizeTotal = 0;

    QDir dir(filePath);

    if (!dir.exists()) {
        return -1;
    }
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);

    QFileInfoList list = dir.entryInfoList();

    if (list.size() < 1) {
        return -1;
    }

    for (int i = 0; i < list.count(); ++i) {
        QFileInfo fileInfo = list.at(i);

        bool bIsDir = fileInfo.isDir();
        if (bIsDir) {
            sizeTotal += getFolderSize(fileInfo.filePath());
        }
        else {
            sizeTotal += fileInfo.size();
        }
    }

    return sizeTotal;
}

// unify path format
QString unifyPathFormat(QString &pathStr)
{
    // Delete \"
    pathStr.replace("\"", "");

    // Let the first char be upper
    pathStr[0] = pathStr[0].toUpper();

    // Unify the separator to '\'
    pathStr = pathStr.replace("/", "\\");

    // Delete the last char when is '\'
    int lastIndex = pathStr.size() - 1;
    if (pathStr.lastIndexOf("\\") == lastIndex) {
        pathStr.replace(lastIndex, 1, "");
    }

    return pathStr;
}

// --end add: shu-yuan


