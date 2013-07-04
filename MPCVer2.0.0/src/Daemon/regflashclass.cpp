#include "regflashclass.h"

RegFlashClass::RegFlashClass(QObject *parent)
	: QThread(parent)
{
	struct soap add_soap;

	add_soap.connect_timeout = 5; //µ•Œª «√Î
	add_soap.send_timeout = 5;
	add_soap.recv_timeout = 5;

	soap_init(&add_soap);
	_MPC__npRequestRequest inputdata;
	_MPC__npRequestResponse outdata;
	inputdata.DisplayName = std::string("shfsdhf");
	int ret = 0;
	ret = soap_call_MPC__npRequest(&add_soap,"127.0.0.1:6600","",inputdata,outdata);

}
void RegFlashClass::run()
{
	forever
	{
		sleep( 5 );
	}

}
RegFlashClass::~RegFlashClass()
{

}
