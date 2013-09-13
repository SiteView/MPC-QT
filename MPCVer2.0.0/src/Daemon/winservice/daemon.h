#ifndef __daemon_h__
#define __daemon_h__

#include <winsock2.h>
#include <Windows.h>
#include "../regflashclass.h"
#include "../synserverthread.h"

class GenieDaemon
{
public:
	RegFlashClass  regScan;
	SynServerThread  synServer;

	static GenieDaemon *create()
	{
		return new GenieDaemon();

	};
	virtual ~GenieDaemon() {}
	virtual bool start(){	
		regScan.start();
		synServer.start();
		return true;
	}
	virtual void stop(){
		regScan.terminate();
		synServer.terminate();

		return;
	};

protected:
	GenieDaemon() {}
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
