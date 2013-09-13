#include <stddef.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <winsock2.h>
#include <Windows.h>
#include <stdlib.h>

#include "daemon.h"

using namespace std;

extern const char *g_genie_server_name;

TestCon2::TestCon2()
{
	m_daemon = GenieDaemon::create();
}

TestCon2::~TestCon2()
{
	delete m_daemon;
}

int TestCon2::run()
{
	if (!m_daemon->start()) {
		return -1;
	}

	for (;;) {
		std::string cmd;
		std::cin >> cmd;

		if (!std::cin) {
			break;
		}

		if (cmd == "quit") {
			break;
		} else if (cmd == "restart") {
		} else if (cmd == "stop") {
		} else if (cmd == "start") {
		} else if (cmd == "arp") {
		} else if (cmd == "netstat") {
		}
	}

	m_daemon->stop();
	delete m_daemon;
	return 0;
}

int console_main(int argc, char **argv)
{
	TestCon2 con;
	return con.run();
}

GenieDaemon::GenieDaemon() 
{
	hMainKey_Primary = HKEY_LOCAL_MACHINE;
	lpSubKey_Primary = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";

	hMainKey_Assistant = HKEY_CURRENT_USER;
	lpSubKey_Assistant = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";

	regScan = new RegFlashClass();
	synServer = new SynServerThread();
	regMonitor_Primary = new RegMonitor(hMainKey_Primary, lpSubKey_Primary, mapOld_Primary);
	regMonitor_Assistant = new RegMonitor(hMainKey_Assistant, lpSubKey_Assistant, mapOld_Assistant);
}


bool GenieDaemon::start(){	
	regScan->start();
	synServer->start();
	regMonitor_Primary->start();
	regMonitor_Assistant->start();

	return true;
}


void GenieDaemon::stop(){
	regScan->terminate();
	synServer->terminate();
	regMonitor_Primary->terminate();
	regMonitor_Assistant->terminate();

	return;
}
