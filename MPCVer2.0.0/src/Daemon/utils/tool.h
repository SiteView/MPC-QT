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
#include <QVector>

#include "../Common/SqliteDb.h"
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

QString get_size( qint64 byte )
{
    double kb=0,mb=0,gb=0;
    QString size;
    if ( byte > 1024 ) kb= byte/1024;
    if ( kb > 1024 ) mb=kb/1024;
    if ( mb > 1024 ) gb=mb/1024;
    size=QObject::tr("%1B").arg(byte);
    if ( kb != 0 ) size=QObject::tr("%1KB").arg(kb,0,'f',2);
    if ( mb != 0 ) size=QObject::tr("%1MB").arg(mb,0,'f',2);
    if ( gb != 0 ) size=QObject::tr("%1GB").arg(gb,0,'f',2);

    return size;
}


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

bool getIconQt(QString &filePath, QString &fileName)
{
    filePath.replace(" ", "\040");
    QFileInfo fileInfo(filePath);
    QFileIconProvider seekIcon;
    QIcon icon = seekIcon.icon(fileInfo);
    QPixmap pixmap=icon.pixmap(QSize(32,32));  // the size can smaller, but never larger.

    QString iconPath = QString("./icons/%1.ico").arg(fileName);
    if(!pixmap.save(iconPath, "ico")) {
        qDebug() << "GetIcon" << "Save Icon Fail";
        return false;
    }
    return true;
}

// get icon from app's path
bool getIcon(QString &filePath, QString &fileName, int iconIndex)
{
    LPCWSTR lstrFilePath = LPCWSTR(filePath.utf16());
    HICON* pIcons = (HICON*)LocalAlloc( LPTR, 100*sizeof(HICON) );

    int cIcons;
    if( pIcons != NULL ) {
        cIcons = (int)ExtractIcons::Get( lstrFilePath, iconIndex, 128, 128, pIcons, NULL, 100, LR_COLOR );
    }

    if (!QDir("./icons").exists()) {
        QDir dIcon;
        if (dIcon.mkdir("./icons")) {
            qDebug() << "Creat the icons directory OK.";
        }
    }
    QString savePath = "./icons/" + fileName + ".ico";

    LPCWSTR lstrSavePath = LPCWSTR(savePath.utf16());

    SaveIcon(pIcons[0], lstrSavePath,32);

    GlobalFree(pIcons);

    bool ret;
    QFileInfo fileInfo(savePath);
    if (fileInfo.exists()) {
        ret = true;
    } else {
        // get icon false, use qt get again.
        if (getIconQt(filePath, fileName))
            ret = true;
        else
            ret = false;
    }

    return ret;
}

// Helper functions
int versionCompareStd(QString &verFirst, QString &verSecond)
{
    //qDebug() << verFirst << "," << verSecond;
    QVector<int> intVectorFisrt, intVectorSecond;
    QStringList strListFisrt, strListSecond;

    strListFisrt = verFirst.split(".");
    strListSecond = verSecond.split(".");

    foreach (QString str, strListFisrt) {
        intVectorFisrt.push_back(str.toInt());
    }
    foreach (QString str, strListSecond) {
        intVectorSecond.push_back(str.toInt());
    }

    int countFirst = intVectorFisrt.count();
    int countSecond = intVectorSecond.count();

    // qDebug() << countFirst << countSecond;
    int iCount = countFirst < countSecond ? countFirst : countSecond;

    if (countFirst == countSecond) {
        for (int i = 0; i < iCount; i++) {
            if (intVectorFisrt.at(i) < intVectorSecond.at(i)) {
                return 1;
            }
        }
    } else {
        return 0;
    }

    return 0;
}

int versionCompareLet(QString &locVer, QString &serVer) {
    return QString::compare(locVer, serVer);
}

bool bHasLetter(QString &str)
{
    int i = 0;
    foreach (str[i], str) {
        if (str[i].isLetter())
            return true;
        i++;
    }

    return false;
}



