#include "soapH.h"
#include "SqliteDb.h"

CSQLiteDb m_SQLiteDb;

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
	int versionInt1[4] = {0};
	int versionInt2[4] = {0};

	versionStrToNumber(versionStr1, versionInt1);
	versionStrToNumber(versionStr2, versionInt2);

	for (int i = 0; i < 4; ++i) {
        if (versionInt1[i] < versionInt2[i])
			return 1;
	}
	return 0;
}

long long generateUUID()
{
//    QUuid uuid;
//    return uuid.createUuid().toString().toStdString();
    QSqlQuery query(*m_SQLiteDb.getDB());
	m_SQLiteDb.getDB()->transaction();
    query.prepare("SELECT AppID FROM ServerAppInfo ORDER BY AppID DESC");
    if (!query.exec()) {
        qDebug(query.lastError().text().toLocal8Bit().data());
    }
    while (query.first()) {
        return query.value(0).toLongLong() + 1;
    }
    return 100000; // TODO
}


int SOAP_FMAC6 MPC__npRequest(struct soap*, struct _MPC__npRequestRequest r_input, struct _MPC__npRequestResponse &r_output)

{

	QSqlQuery query(*m_SQLiteDb.getDB());
	m_SQLiteDb.getDB()->transaction();

    QVariant locAppID = QVariant((QString::fromStdString(r_input.AppID)).toLongLong());
	QString locVersion = QString::fromStdString(r_input.ResetServerVersion);
	QString locDisplayName = QString::fromStdString(r_input.DisplayName);


	// --update server begin
	// check input AppID from server database
    query.prepare("SELECT AppID, ResetServerVersion FROM ServerAppInfo WHERE DisplayName = ?");
	query.addBindValue(locDisplayName);
	if (!query.exec()) {
		qDebug(query.lastError().text().toLocal8Bit().data());
	}

	// the input App exist in server database
	if(query.next()) {
        QString serVersion = query.value(1).toString();
		bool bHaveNewVersion = versionCompare(serVersion, locVersion);

		// have new version, update
        if (bHaveNewVersion) {
            query.prepare("UPDATE ServerAppInfo SET  ResetServerVersion = ? WHERE DisplayName = ?");
			query.addBindValue(QVariant((QString::fromStdString(r_input.ResetServerVersion))));
			query.addBindValue(locDisplayName);
			if (!query.exec()) {
				qDebug(query.lastError().text().toLocal8Bit().data());
			}
		}
	}

	// the response AppId not exist in server database
	else {
        // generate a new AppID

        query.prepare("INSERT INTO ServerAppInfo ( DisplayName, ResetServerVersion) VALUES(:DisplayName, :ResetServerVersion)");
		query.bindValue(":DisplayName", QVariant(locDisplayName));
		query.bindValue(":ResetServerVersion", QVariant(QString::fromStdString(r_input.ResetServerVersion)));
		if (!query.exec()) {
			qDebug(query.lastError().text().toLocal8Bit().data());
		}

        query.prepare("SELECT AppID, AppName, DisplayName, Detailtext, Mark, Type, ServerVersion, ResetServerVersion, Size, OrderNumber, AllDownload, FewDownload, DownloadURL, OS FROM ServerAppInfo WHERE DisplayName = ?");
        query.addBindValue(QVariant(locDisplayName));
        if (!query.exec()) {
            qDebug(query.lastError().text().toLocal8Bit().data());
        }
        while (query.next()) {
            r_output.AppID = query.value(0).toString().toStdString();
            r_output.AppName = query.value(1).toString().toStdString();
            r_output.DisplayName = query.value(2).toString().toStdString();
            r_output.Detailtext = query.value(3).toString().toStdString();
            r_output.Mark = query.value(4).toString().toStdString();
            r_output.Type = query.value(5).toString().toStdString();
            r_output.ServerVersion = query.value(6).toString().toStdString();
            r_output.ResetServerVersion = query.value(7).toString().toStdString();
            r_output.Size = query.value(8).toString().toStdString();
            r_output.OrderNumber = query.value(9).toString().toStdString();
            r_output.AllDownload = query.value(10).toString().toStdString();
            r_output.FewDownload = query.value(11).toString().toStdString();
            r_output.DownloadURL = query.value(12).toString().toStdString();
            r_output.OS = query.value(13).toString().toStdString();
        }

        m_SQLiteDb.getDB()->commit();
        return SOAP_OK;

	}
	// --update server finish

	// randomly get a row for output
	query.prepare("SELECT AppID, AppName, DisplayName, Detailtext, Mark, Type, ServerVersion, ResetServerVersion, Size, OrderNumber, AllDownload, FewDownload, DownloadURL, OS FROM ServerAppInfo ORDER BY RAND() LIMIT 1");
	if (!query.exec()) {
		qDebug(query.lastError().text().toLocal8Bit().data());
	}
	while (query.next()) {
		r_output.AppID = query.value(0).toString().toStdString();
		r_output.AppName = query.value(1).toString().toStdString();
		r_output.DisplayName = query.value(2).toString().toStdString();
		r_output.Detailtext = query.value(3).toString().toStdString();
		r_output.Mark = query.value(4).toString().toStdString();
		r_output.Type = query.value(5).toString().toStdString();
		r_output.ServerVersion = query.value(6).toString().toStdString();
		r_output.ResetServerVersion = query.value(7).toString().toStdString();
		r_output.Size = query.value(8).toString().toStdString();
		r_output.OrderNumber = query.value(9).toString().toStdString();
		r_output.AllDownload = query.value(10).toString().toStdString();
		r_output.FewDownload = query.value(11).toString().toStdString();
		r_output.DownloadURL = query.value(12).toString().toStdString();
		r_output.OS = query.value(13).toString().toStdString();
	}
	query.finish();

    m_SQLiteDb.getDB()->commit();

	return SOAP_OK;
}


int SOAP_FMAC6 MPC__flashReguest(struct soap*, long start_input, struct _MPC__npRequestResponse *pReq, long &count)
{
    return SOAP_OK;
}
int SOAP_FMAC6 MPC__typeRequest(struct soap*, long type, long order, struct _MPC__npRequestResponse &r_output)
{
    return SOAP_OK;
}
