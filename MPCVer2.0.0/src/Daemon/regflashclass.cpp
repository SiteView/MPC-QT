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
#include <QDateTime>
#include <QFile>
#include <QPixmap>
#include "regflashclass.h"
#include "utils/tool.h"

RegFlashClass::RegFlashClass(/*QObject *parent*/)
	: QThread(/*parent*/)
{
}

RegFlashClass::~RegFlashClass()
{

}

void RegFlashClass::run()
{
	QString val1;
	QString val2;
	QString val3 ;
	QString val4;
	QString val5;
	QString val6;
	QString val7;
	QString val8;
	QString val9;
	QString val10;
	QDateTime setupdate;

	QSqlQuery query(*m_SQLiteDb.getDB());

	for (int i = 0; i < 2; ++i)  // TODO: how ensure the scan registry perfectly
	{
		static bool flagReg = true;
		QString regStr;
		if(flagReg)
			regStr = QString("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
		else
			regStr = QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
		flagReg = !flagReg;

		QSettings reg(regStr, QSettings::NativeFormat);
		QStringList groupsList = reg.childGroups();

		foreach(QString group, groupsList)
		{
			reg.beginGroup(group);

			val1 = reg.value("DisplayName").toString();
			val10 = reg.value("SystemComponent").toString();

			// clear the record while none name, system components, security component
			if(!val1.isEmpty()
				&& (val10.isEmpty() || val10.compare("0") == 0)
				&& val1.indexOf("Security Update for Microsoft") == -1
				&& val1.indexOf("(KB") == -1 )
			{
				;// 空处理
			}
			else
			{
				// 安全更新和系统组件不在管理范围
				reg.endGroup();
				continue;
			}


			// check in db
			query.prepare("SELECT DisplayName FROM LocalAppInfor WHERE DisplayName = ?");
			query.addBindValue(val1);
			if(!query.exec())
			{
				qDebug(query.lastError().text().toLocal8Bit().data());
			}            

			bool found = false;
			if(query.next())
			{
				found = true;
			}
			query.finish();

			if(found)
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
				setupdate =  QDateTime::fromString(val8, "yyyyMMdd");

			QString path ;
			bool rethav = false;
			if(val7.isEmpty() || val7.compare("") == 0 )
			{
				rethav = HavePath(val2, path); // repair by UninstallString
				if(!rethav)
					rethav = HavePath(val3, path); // repair by DisplayIcon
			}

			// filter the system components and security components
			if(!val1.isEmpty()
				&& (val10.isEmpty() || val10.compare("0") == 0)
				&& val1.indexOf("Security Update for Microsoft") == -1
				&& val1.indexOf("(KB") == -1 )
			{
				m_SQLiteDb.getDB()->transaction();

				query.prepare("INSERT INTO LocalAppInfor (DisplayName,UninstallString,DisplayIcon,DisplayVersion,URLInfoAbout,Publisher,InstallLocation,SetupTime,EstimatedSize)"
					"VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?)");
				query.addBindValue(val1);
				query.addBindValue(val2);
				query.addBindValue(val3);
				query.addBindValue(val4);
				query.addBindValue(val5);
				query.addBindValue(val6);
				if(rethav)
					query.addBindValue(path); // repair install path
				else
					query.addBindValue(val7);
				query.addBindValue(setupdate);
				query.addBindValue(val9);

				if (!query.exec())
				{
					qDebug(query.lastError().text().toLocal8Bit().data());
				}

				m_SQLiteDb.getDB()->commit();
			}

			reg.endGroup();
		}
	}

	// update all the information.
	query.prepare("SELECT DisplayName FROM LocalAppInfor");
	if (!query.exec())
	{
		qDebug(query.lastError().text().toLocal8Bit().data());
	}
	while (query.next()) {
		QVariant DisplayName = query.value(0);
		UpdateInfo(DisplayName);
	}
	query.finish();
}

void RegFlashClass::UpdateInfo(QVariant DisplayName)
{
	QSqlQuery SQLiteQuery( *m_SQLiteDb.getDB() );
	QSqlQuery updateQuery( *m_SQLiteDb.getDB() );
	QSqlQuery serverQuery( *m_SQLiteDb.getDB() );
	m_SQLiteDb.getDB()->transaction();

	SQLiteQuery.prepare("SELECT DisplayName, InstallLocation, DisplayVersion, SetupTime, DisplayIcon, UninstallString, EstimatedSize FROM LocalAppInfor WHERE DisplayName = ?");
	SQLiteQuery.addBindValue(DisplayName);
	if (!SQLiteQuery.exec())
	{
		qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
	}	

	if (SQLiteQuery.next())
	{
		QVariant valDisplayName = SQLiteQuery.value(0);
		QVariant valInstallLocation = SQLiteQuery.value(1);
		QVariant valDisplayVersion = SQLiteQuery.value(2);
		//QVariant valSetupTime = SQLiteQuery.value(3);
		QVariant valDisplayIcon = SQLiteQuery.value(4);
		QVariant valUninstallString = SQLiteQuery.value(5);

		QString installPathStr = valInstallLocation.toString();
		QString iconPath = valDisplayIcon.toString();
		QString uninstallPathStr = valUninstallString.toString();

		// formate install path ,uninstall path and display icon path
		if (installPathStr.compare("")) {
			unifyPathFormat(installPathStr);
			updateFieldValue(updateQuery, "InstallLocation", installPathStr, valDisplayName);
		}
		if (uninstallPathStr.compare("")) {
			unifyPathFormat(uninstallPathStr);
			updateFieldValue(updateQuery, "UninstallString", uninstallPathStr, valDisplayName);
		}
		if (iconPath.compare("")) {
			unifyPathFormat(iconPath);
			updateFieldValue(updateQuery, "DisplayIcon", iconPath, valDisplayName);
		}

		// repair install time
		if(installPathStr.compare("") )
		{
			QFileInfo pathInfo(installPathStr);
			updateFieldValue(updateQuery, "SetupTime", pathInfo.created(), valDisplayName);
		}

		// repair version
		if(valDisplayVersion.toString().compare("") == 0)
		{
			QString version ;
			QString displayName = valDisplayName.toString();
			bool ret = TrimVersion(displayName,version);
			if(ret)
			{
				updateFieldValue(updateQuery, "DisplayVersion", version, valDisplayName);
			}
		}

		// repair the size of installed software
		if (installPathStr.compare("") && SQLiteQuery.value(6) == "" ) {
			long long size = 0;
			size = getFolderSize(installPathStr);

			updateFieldValue(updateQuery, "EstimatedSize", size, valDisplayName);
		}


		// repair the icon path
		int iconIndx = 0;
		if (iconPath.contains(",")) {
			int indexFirst = iconPath.lastIndexOf(",");
			int indexEnd = iconPath.length() - 1;

			iconIndx = iconPath.mid(indexFirst + 1, indexEnd - indexFirst).toInt();
			iconPath.replace(QString(",%1").arg(QString::number(iconIndx)), "");

			//iconStr.replace(",", "");
		}

		QFileInfo iconInfo(iconPath);
		bool bIconExist = iconInfo.exists();

		QString newIconPath = "./icons/" + valDisplayName.toString() + ".ico";
		QFileInfo fileInfo(newIconPath);

		if (bIconExist) {
			if (!fileInfo.exists()) {
				// file is icon
				if (iconPath.contains(".ico")) {
					QFile::copy(iconPath, newIconPath);
				} else {
					// get icon path
					getIcon(iconPath, valDisplayName.toString(), iconIndx);
				}
			}
		} // bIconExist is true
		else {
			// repair from install location
			if (installPathStr.compare("")) {
				QFileInfo fileInfo(installPathStr);
				QString fileBaseName = fileInfo.baseName();
				iconPath = installPathStr + "\\" + fileBaseName + ".exe";
			}

			QFileInfo fileInfoIcon(iconPath);
			if (fileInfoIcon.exists()) {
				if (getIcon(iconPath, valDisplayName.toString(), iconIndx)) {
					updateFieldValue(updateQuery, "DisplayIcon", iconPath, valDisplayName);
				}
			} else {
				// Microsoft WindowsInstaller install
				if (uninstallPathStr.contains("MsiExec.exe")) {
					QString msIconStr = ":/res/defaulticons/MsiExec.ico";
					QFile::copy(msIconStr, newIconPath);
				}
			}
		} // bIconExist is false


		// delete the unloaded software over 2 days
		if (!updateQuery.exec("delete from LocalAppInfor where UninstallTime is not null and UninstallTime < datetime('now','localtime','-2 days')")) {
			qDebug(updateQuery.lastError().text().toLocal8Bit().data());
		}


		// check new version
		QString serverVersion;
		serverQuery.prepare("SELECT ServerVersion FROM ServerAppInfo WHERE DisplayName = ?");
		serverQuery.addBindValue(valDisplayName);
		if (!serverQuery.exec()) {
			qDebug(serverQuery.lastError().text().toLocal8Bit().data());
		}		

		while (serverQuery.next()) {
			serverVersion = serverQuery.value(0).toString();
			QString localVersion = valDisplayVersion.toString();

			bool bHasLetter = ::bHasLetter(localVersion) || ::bHasLetter(serverVersion);

			bool bHaveNewVersion;
			if (bHasLetter) {
				bHaveNewVersion = versionCompareLet(localVersion, serverVersion);
			} else
				bHaveNewVersion = versionCompareStd(localVersion, serverVersion);

			if (bHaveNewVersion) {
				updateQuery.prepare("UPDATE LocalAppInfor SET HaveNew = ? WHERE DisplayName = ?");
				updateQuery.addBindValue(QVariant(true));
				updateQuery.addBindValue(valDisplayName);
				if (!updateQuery.exec()) {
					qDebug(updateQuery.lastError().text().toLocal8Bit().data());
				}
			} else {
				updateQuery.prepare("UPDATE LocalAppInfor SET HaveNew = ? WHERE DisplayName = ?");
				updateQuery.addBindValue(QVariant(false));
				updateQuery.addBindValue(valDisplayName);
				if (!updateQuery.exec()) {
					qDebug(updateQuery.lastError().text().toLocal8Bit().data());
				}
			}
		}

		SQLiteQuery.finish();
		serverQuery.finish();
	}

	m_SQLiteDb.getDB()->commit();
}

bool RegFlashClass::updateFieldValue(QSqlQuery sqlQuery, const QString &fieldName,
	const QVariant &value, const QVariant &var)
{
	sqlQuery.prepare(QString("UPDATE LocalAppInfor SET %1 = ? WHERE  DisplayName = ?").arg(fieldName));
	sqlQuery.addBindValue(value);
	sqlQuery.addBindValue(var);
	if (!sqlQuery.exec()) {
		qDebug(sqlQuery.lastError().text().toLocal8Bit().data());
		return false;
	}

	return true;
}



