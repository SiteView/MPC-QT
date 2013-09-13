#ifndef REGMONITOR_H
#define REGMONITOR_H

#include <QThread>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <map>
#include <vector>
#include <cstring>
#include <atlstr.h>
#include <iostream>
#include "../Common/SqliteDb.h"

typedef std::map<CString, CString> inner_map;
typedef std::map<CString, inner_map> outer_map;
typedef std::vector<CString> cstr_vector;

class RegMonitor : public QThread
{
    Q_OBJECT
public:
    explicit RegMonitor(QObject *parent = 0);
	RegMonitor(HKEY, CString, outer_map);

public:
    UINT GetKey(HKEY hMainKey, CString lpSubKey, cstr_vector &keyVector);
    UINT GetValue(HKEY hMainKey, CString lpSubKey, inner_map &valueMap);
    void GetKeyAndValue(HKEY hMainKey, CString lpSubKey, outer_map &keyAndValueMap);
    void Compare(outer_map mapOld, outer_map mapNew, inner_map &mapChanged, int indx);
    void WINAPI MonitorReg(HKEY hMainKey, CString lpSubKey, outer_map &mapOld);
	void UpdateTable(inner_map mapChanged, int indx);

signals:

public slots:

protected:
    virtual void run();

private:
	CSQLiteDb m_SQLiteDb;
	HKEY hMainKey;
	CString lpSubKey;
	outer_map mapOld;

};

#endif // REGMONITOR_H
