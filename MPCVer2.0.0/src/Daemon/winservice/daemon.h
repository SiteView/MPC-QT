#ifndef __daemon_h__
#define __daemon_h__

#include <winsock2.h>
#include <Windows.h>
#include "../regflashclass.h"
#include "../synserverthread.h"
#include "../regmonitor.h"

typedef std::map<CString, CString> inner_map;
typedef std::map<CString, inner_map> outer_map;
typedef std::vector<CString> cstr_vector;

class GenieDaemon
{
public:

	static GenieDaemon *create()
	{
		return new GenieDaemon();

    }
    virtual ~GenieDaemon(){}
	virtual bool start();
	virtual void stop();

protected:
	GenieDaemon();

private:
	HKEY hMainKey_Primary, hMainKey_Assistant;
	CString lpSubKey_Primary, lpSubKey_Assistant;
	outer_map mapOld_Primary, mapOld_Assistant;

	RegFlashClass  *regScan;
	SynServerThread  *synServer;
	RegMonitor *regMonitor_Primary;
	RegMonitor *regMonitor_Assistant;

};

class TestCon2
{
public:
	TestCon2();
	~TestCon2();

	int run();

private:
	GenieDaemon *m_daemon;
};

#endif // __daemon_h__
