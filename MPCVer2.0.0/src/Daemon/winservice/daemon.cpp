
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
	return 0;
}

int console_main(int argc, char **argv)
{
	TestCon2 con;
	return con.run();
}

