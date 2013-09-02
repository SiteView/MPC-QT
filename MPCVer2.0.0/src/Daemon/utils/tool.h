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
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QFileIconProvider>
#include <QPixmap>
#include <QIcon>
#include <QDebug>


#include "SqliteDb.h"
#include "../Common/ExtractIcon.h"

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
    if ( !SQLiteQuery.exec() )
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

// get icon from app's path
QString getIcon(QString &filePath, QString &fileName)
{
    LPCWSTR lstrFilePath = LPCWSTR(filePath.utf16());
    HICON* pIcons = (HICON*)LocalAlloc( LPTR, 100*sizeof(HICON) );

    int cIcons;
    if( pIcons != NULL ) {
        cIcons = (int)ExtractIcons::Get( lstrFilePath, 0, 128, 128, pIcons, NULL, 100, LR_COLOR );
    }

    QString savePath = "./icons/" + fileName + ".ico";

    LPCWSTR lstrSavePath = LPCWSTR(savePath.utf16());

    qDebug() << SaveIcon(pIcons[0], lstrSavePath,32);

    GlobalFree(pIcons);

    return QString::fromStdWString(lstrSavePath);


//    filePath.replace(" ", "\040");
//    QFileInfo fileInfo(filePath);
//    QFileIconProvider seekIcon;
//    QIcon icon = seekIcon.icon(fileInfo);
//    QPixmap pixmap = icon.pixmap(QSize(45, 45));

//    QDir *dir = new QDir();
//    if (!dir->exists("icons")) {
//        int b = dir->mkdir("icons");
//        qDebug() << b << "make a directory for icons successfully";
//    }
//    QString iconPath = QString("./icons/%1.png").arg(fileName);  // icon path
//   // QString iconPath = filePath.replace(fileInfo.completeSuffix(), "png");  // some path only read, so is not nice.
//    if(!pixmap.save(iconPath, "png"))
//    {
//        //qDebug() << "GetIcon" << "Save Icon Fail";
//    }

//    return iconPath;
}

// Helper functions
void versionStrToNumber(QString &versionStr, int *versionInt)
{
    QStringList strList;
    strList = versionStr.split(".");
    int i = 0;
    foreach(QString str, strList) {
        versionInt[i] = str.toInt();
        i++;
    }
}

int versionCompare(QString &versionStr1, QString &versionStr2)
{
    int versionInt1[5] = {0};
    int versionInt2[5] = {0};

    versionStrToNumber(versionStr1, versionInt1);
    versionStrToNumber(versionStr2, versionInt2);

    for (int i = 0; i < 4; i++) {
        if (versionInt1[i] < versionInt2[i])
            return 1;
    }
    return 0;
}
// --end add: shu-yuan


