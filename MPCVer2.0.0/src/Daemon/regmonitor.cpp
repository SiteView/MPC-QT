#include <QDateTime>
#include <QDebug >
#include <QVariant>
#include <QString>
#include <QByteArray>

#include "regmonitor.h"
#include "regflashclass.h"

using namespace std;

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

RegMonitor::RegMonitor(QObject *parent) :
QThread(parent)
{
	
}

RegMonitor::RegMonitor(HKEY hkey, CString cstr, outer_map outMap)
{
	hMainKey = hkey;
	lpSubKey = cstr;
	mapOld = outMap;
}

UINT RegMonitor::GetKey(HKEY hMainKey, CString lpSubKey, cstr_vector &keyVector)
{
	TCHAR    achKey[MAX_KEY_LENGTH];		 // buffer for subkey name
	DWORD    cbName;						 // size of name string
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name
	DWORD    cchClassName = MAX_PATH;		 // size of class string
	DWORD    cSubKeys=0;					 // number of subkeys
	DWORD    cbMaxSubKey;					 // longest subkey size
	DWORD    cchMaxClass;					 // longest class string
	DWORD    cValues;						 // number of values for key
	DWORD    cchMaxValue;					 // longest value name
	DWORD    cbMaxValueData;				 // longest value data
	DWORD    cbSecurityDescriptor;			 // size of security descriptor
	FILETIME ftLastWriteTime;				 // last write time

	HKEY    hKey;
	HANDLE  hEvent;

	LONG   lErrorCode;

	// Open a key.
	lErrorCode = RegOpenKeyEx(hMainKey, lpSubKey, 0, KEY_NOTIFY | KEY_READ, &hKey);

	if (lErrorCode != ERROR_SUCCESS)
	{
		qDebug() << (TEXT("Error in RegOpenKeyEx (%d).\n"), lErrorCode);

		return 0;
	}

	// Create an event.

	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (hEvent == NULL)
	{
		qDebug() << (TEXT("Error in CreateEvent (%d).\n"), GetLastError());

		return 0;
	}

	// Get the class name and the value count.
	lErrorCode = RegQueryInfoKey(
		hKey,                    // key handle
		achClass,                // buffer for class name
		&cchClassName,           // size of class string
		NULL,                    // reserved
		&cSubKeys,               // number of subkeys
		&cbMaxSubKey,            // longest subkey size
		&cchMaxClass,            // longest class string
		&cValues,                // number of values for this key
		&cchMaxValue,            // longest value name
		&cbMaxValueData,         // longest value data
		&cbSecurityDescriptor,   // security descriptor
		&ftLastWriteTime);       // last write time

	if (lErrorCode != ERROR_SUCCESS)
	{
		qDebug() << (TEXT("Error in RegQueryInfoKey (%d).\n"), lErrorCode);

		return 0;
	}

	// Enumerate the subkeys, until RegEnumKeyEx fails.
	if (cSubKeys)
	{
		qDebug() << ( "\nNumber of subkeys: %d\n", cSubKeys);
		for (unsigned int i = 0; i < cSubKeys; ++i)
		{
			cbName = MAX_KEY_LENGTH;
			lErrorCode = RegEnumKeyEx(hKey, i,
				achKey,
				&cbName,
				NULL,
				NULL,
				NULL,
				&ftLastWriteTime);
			if (lErrorCode == ERROR_SUCCESS)
			{
				//qDebug() << (TEXT("(%d) %s\n"), i+1, achKey);
				keyVector.push_back(achKey);
			}
		} // end of for
	}  // end of if(cSubKeys)

	// Close the key.
	lErrorCode = RegCloseKey(hKey);

	if (lErrorCode != ERROR_SUCCESS)
	{
		qDebug() << (TEXT("Error in RegCloseKey (%d).\n"), GetLastError());

		return 0;
	}

	// Close the handle.
	if (!CloseHandle(hEvent))
	{
		qDebug() << (TEXT("Error in CloseHandle.\n"));

		return 0;
	}

	return 1;
}

UINT RegMonitor::GetValue(HKEY hMainKey, CString lpSubKey, inner_map &valueMap)
{
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name
	DWORD    cchClassName = MAX_PATH;        // size of class string
	DWORD    cSubKeys=0;                     // number of subkeys
	DWORD    cbMaxSubKey;                    // longest subkey size
	DWORD    cchMaxClass;                    // longest class string
	DWORD    cValues;						 // number of values for key
	DWORD    cchMaxValue;                    // longest value name
	DWORD    cbMaxValueData;                 // longest value data
	DWORD    cbSecurityDescriptor;           // size of security descriptor
	FILETIME ftLastWriteTime;                // last write time

	DWORD    type=0;
	DWORD    dataSize=0;
	TCHAR    achValue[MAX_VALUE_NAME];
	DWORD    cchValue = MAX_VALUE_NAME;

	HANDLE  hEvent;
	HKEY    hKey;
	LONG   lErrorCode;

	// open the key
	lErrorCode = RegOpenKeyEx(hMainKey, lpSubKey, 0, KEY_NOTIFY | KEY_READ, &hKey);

	if (lErrorCode != ERROR_SUCCESS)
	{
		qDebug() << (TEXT("Error in RegOpenKeyEx (%d).\n"), lErrorCode);

		return 0;
	}

	// Create an event.
	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (hEvent == NULL)
	{
		qDebug() << (TEXT("Error in CreateEvent (%d).\n"), GetLastError());

		return 0;
	}

	// Get the class name and the value count.
	lErrorCode = RegQueryInfoKey(
		hKey,                    // key handle
		achClass,                // buffer for class name
		&cchClassName,           // size of class string
		NULL,                    // reserved
		&cSubKeys,               // number of subkeys
		&cbMaxSubKey,            // longest subkey size
		&cchMaxClass,            // longest class string
		&cValues,                // number of values for this key
		&cchMaxValue,            // longest value name
		&cbMaxValueData,         // longest value data
		&cbSecurityDescriptor,   // security descriptor
		&ftLastWriteTime);       // last write time

	if (lErrorCode != ERROR_SUCCESS)
	{
		qDebug() << (TEXT("Error in RegQueryInfoKey (%d).\n"), lErrorCode);

		return 0;
	}

	// Enumerate the key values.
	if (cValues > 0)
	{
		CString name = "";
		CString value = "";

		//printf( "\nNumber of values: %d\n", cValues);
		for (DWORD i = 0, lErrorCode = ERROR_SUCCESS; i < cValues; ++i)
		{
			name = "";
			value = "";
			cchValue = 16383;
			achValue[0] = '\0';
			LPBYTE lpData  = new BYTE[16383];
			dataSize = 16383;

			lErrorCode = RegEnumValue(
				hKey,
				i,
				achValue,
				&cchValue,
				NULL,
				&type,
				lpData,
				&dataSize);

			// get the values
			if (lErrorCode == ERROR_SUCCESS )
			{
				name = achValue;			
				
				value = (LPCTSTR)lpData;
			
				valueMap[name] = value;
			} // end of if (lErrorCode == ERROR_SUCCESS )

		} // end of for

	} // end of if

	// Close the key.
	lErrorCode = RegCloseKey(hKey);

	if (lErrorCode != ERROR_SUCCESS)
	{
		qDebug() << (TEXT("Error in RegCloseKey (%d).\n"), GetLastError());

		return 0;
	}

	// Close the handle.
	if (!CloseHandle(hEvent))
	{
		qDebug() << (TEXT("Error in CloseHandle.\n"));

		return 0;
	}

	return 1;
}

void RegMonitor::GetKeyAndValue(HKEY hMainKey, CString lpSubKey, outer_map &keyAndValueMap)
{
	vector<CString> keyVector;
	vector<CString>::iterator keyIter;
	GetKey(hMainKey, lpSubKey, keyVector);

	for (keyIter = keyVector.begin(); keyIter != keyVector.end(); ++keyIter) {
		inner_map valueMap;
		GetValue(hMainKey, lpSubKey + "\\" + *keyIter, valueMap);
		keyAndValueMap[*keyIter] = valueMap;
	}
}

// we appoint: old map size > new map size, indx < 0;
//             old map size = new map size, indx = 0;
//			   old map size < new map size, indx > 0;
void RegMonitor::Compare(outer_map mapOld, outer_map mapNew, inner_map &mapChanged, int indx)
{
	outer_map::iterator outer_iter_old, outer_iter_new;
	inner_map::iterator inner_iter;

	// changed: remove
	if (indx < 0) {
		for (outer_iter_old = mapOld.begin(); outer_iter_old != mapOld.end(); ++outer_iter_old) {
			if (mapNew.find(outer_iter_old->first) == mapNew.end()) {
				mapChanged = outer_iter_old->second;
				return;
			}
		}
	}

	// changed:
	if (indx == 0) {
		for (outer_iter_old = mapOld.begin(); outer_iter_old != mapOld.end(); ++outer_iter_old) {
			CString outer_key = outer_iter_old->first;
			inner_map innerMapOld = outer_iter_old->second;
			inner_map innerMapNew = mapNew.at(outer_key);
			if (innerMapOld != innerMapNew) {
				mapChanged = innerMapNew;
				return;
			}
		}
	}

	// changed: add
	if (indx > 0) {
		for (outer_iter_new = mapNew.begin(); outer_iter_new != mapNew.end(); ++outer_iter_new) {
			if (mapOld.find(outer_iter_new->first) == mapOld.end()) {
				mapChanged = outer_iter_new->second;
				return;
			}
		}
	}
}

QString convertCStrToQStr(CString cstrIn, inner_map mapChanged, inner_map::iterator it)
{
	QString qstr; // 0x635a02ef 0x040ac0d8
	QString tmpStr = "hello Qt!";//0x040ac070
	it = mapChanged.find(cstrIn);
	if (it != mapChanged.end()) {
		CString cstrOut = it->second;

#ifdef _UNICODE 
		qstr = QString::fromWCharArray((LPCTSTR)cstrOut, cstrOut.GetLength()); 
		//qstr = QString::fromUtf16(cstrOut.GetBuffer(), cstrOut.GetLength());
#else
		qstr = QString((LPCTSTR)cstr);
#endif

	}

	return qstr;
}

void RegMonitor::UpdateTable(inner_map mapChanged, int indx)
{
	inner_map::iterator it;

	QString DisplayName;
	QString UninstallString;
	QString DisplayIcon;
	QString DisplayVersion;
	QString URLInfoAbout;
	QString Publisher;
	QString InstallLocation;
	QString InstallDate;
	QString EstimatedSize;
	QString SystemComponent;
	QDateTime SetupTime;

	DisplayName = convertCStrToQStr(CString("DisplayName"), mapChanged, it);
	UninstallString = convertCStrToQStr(CString("UninstallString"), mapChanged, it);
	DisplayIcon = convertCStrToQStr(CString("DisplayIcon"), mapChanged, it);
    DisplayVersion = convertCStrToQStr(CString("DisplayVersion"), mapChanged, it); 
	URLInfoAbout = convertCStrToQStr(CString("URLInfoAbout"), mapChanged, it);
	Publisher = convertCStrToQStr(CString("Publisher"), mapChanged, it);
	InstallLocation = convertCStrToQStr(CString("InstallLocation"), mapChanged, it);
	InstallDate = convertCStrToQStr(CString("SetupTime"), mapChanged, it);
	EstimatedSize = convertCStrToQStr(CString("EstimatedSize"), mapChanged, it); 
	SystemComponent = convertCStrToQStr(CString("SystemComponent"), mapChanged, it);

	SetupTime =  QDateTime::fromString(InstallDate, "yyyyMMdd");

	QSqlQuery query(*m_SQLiteDb.getDB());
	m_SQLiteDb.getDB()->transaction();

	// changed: remove
	if (indx < 0) {
		query.exec("DELETE FROM LocalAppInfor WHERE DisplayName = ?");
		query.addBindValue(DisplayName);

		if (!query.exec()) {
			qDebug() << (query.lastError().text().toLocal8Bit().data());
		}
	}

	// changed: null
	if (indx == 0) {
		query.prepare("UPDATE LocalAppInfor SET UninstallString = ?, DisplayIcon = ?, DisplayVersion = ?, URLInfoAbout = ?, Publisher = ?, InstallLocation = ?, SetupTime = ?, EstimatedSize = ? WHERE DisplayName = ?");
		query.addBindValue(UninstallString);
		query.addBindValue(DisplayIcon);
		query.addBindValue(DisplayVersion);
		query.addBindValue(URLInfoAbout);
		query.addBindValue(Publisher);
		query.addBindValue(InstallLocation);
		query.addBindValue(SetupTime);
		query.addBindValue(EstimatedSize);
		query.addBindValue(DisplayName);

		if (!query.exec()) {
			qDebug() << (query.lastError().text().toLocal8Bit().data());
		}
	}

	// changed: add
	if (indx > 0) {
		if(!DisplayName.isEmpty()  // exclude none name
			&& (SystemComponent.isEmpty() || SystemComponent.compare("0") == 0)   // exclude system component
			&& DisplayName.indexOf("Security Update for Microsoft") == -1 &&  DisplayName.indexOf("(KB") == -1 ) {   // exclude security component , 
				query.prepare("INSERT INTO LocalAppInfor (DisplayName, UninstallString, DisplayIcon, DisplayVersion, URLInfoAbout, Publisher, InstallLocation, SetupTime, EstimatedSize)" 
					"VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?)");
				query.addBindValue(DisplayName);
				query.addBindValue(UninstallString);
				query.addBindValue(DisplayIcon);
				query.addBindValue(DisplayVersion);
				query.addBindValue(URLInfoAbout);
				query.addBindValue(Publisher);
				query.addBindValue(InstallLocation);
				query.addBindValue(SetupTime);
				query.addBindValue(EstimatedSize);			


				if (!query.exec()) {
					qDebug() << (query.lastError().text().toLocal8Bit().data());
				}
		}
	}

	m_SQLiteDb.getDB()->commit();

	// update the changed record.
	RegFlashClass *reg = new RegFlashClass();
	reg->UpdateInfo(DisplayName);
	delete reg;
}

void WINAPI RegMonitor::MonitorReg(HKEY hMainKey, CString lpSubKey, outer_map &mapOld)
{
	while(TRUE)
	{
		DWORD  dwFilter = REG_NOTIFY_CHANGE_NAME |
			REG_NOTIFY_CHANGE_ATTRIBUTES |
			REG_NOTIFY_CHANGE_LAST_SET |
			REG_NOTIFY_CHANGE_SECURITY;

		HANDLE hEvent;
		HKEY   hKey;
		LONG   lErrorCode;

		// Open a key.

		lErrorCode = RegOpenKeyEx(hMainKey, lpSubKey, 0, KEY_NOTIFY, &hKey);

		if (lErrorCode != ERROR_SUCCESS)
		{
			qDebug() << (TEXT("Error in RegOpenKeyEx (%d).\n"), lErrorCode);

			return;
		}

		// Create an event.

		hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

		if (hEvent == NULL)
		{
			qDebug() << (TEXT("Error in CreateEvent (%d).\n"), GetLastError());

			return;
		}

		// Watch the registry key for a change of value.

		lErrorCode = RegNotifyChangeKeyValue(hKey,
			TRUE,
			dwFilter,
			hEvent,
			TRUE);

		if (lErrorCode != ERROR_SUCCESS)
		{
			qDebug() << (TEXT("Error in RegNotifyChangeKeyValue (%d).\n"), lErrorCode);

			return;
		}

		// Wait for an event to occur.

		qDebug() << (TEXT("Waiting for a change in the specified key...\n"));

		if (WaitForSingleObject(hEvent, INFINITE) == WAIT_FAILED)
		{
			qDebug() << (TEXT("Error in WaitForSingleObject (%d).\n"), GetLastError());

			return;
		}
		else
		{
			qDebug() << (TEXT("\nChange has occurred.\n"));

			outer_map mapNew;
			inner_map mapChanged;

			GetKeyAndValue(hMainKey, lpSubKey, mapNew);

			LONG mapOldSize = mapOld.size();
			LONG mapNewSize = mapNew.size();

			INT indx;
			indx = mapNewSize - mapOldSize;

			Compare(mapOld, mapNew, mapChanged, indx);

			mapOld = mapNew;

			UpdateTable(mapChanged, indx);

		}

		// Close the key.

		lErrorCode = RegCloseKey(hKey);

		if (lErrorCode != ERROR_SUCCESS)
		{
			qDebug() << (TEXT("Error in RegCloseKey (%d).\n"), GetLastError());

			return;
		}

		// Close the handle.

		if (!CloseHandle(hEvent))
		{
			qDebug() << (TEXT("Error in CloseHandle.\n"));

			return;
		}
	}

	return;
}

void RegMonitor::run()
{	
	GetKeyAndValue(hMainKey, lpSubKey, mapOld);

	MonitorReg(hMainKey, lpSubKey, mapOld);

	getchar();

}
