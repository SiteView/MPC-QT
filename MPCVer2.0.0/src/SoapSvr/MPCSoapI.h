
#ifndef _C_SOASERVICE_INTERFACE_
#define _C_SOASERVICE_INTERFACE_

	//gsoap ns service name: MPC
	//gsoap ns service executable: MPC.cgi
	//gsoap ns service encoding: encoded
	//gsoap ns schema : urn:soap

	typedef std::string xsd__string;

	typedef struct _MPC__npRequestRequest
	{ 
	     xsd__string AppID;
		 xsd__string DisplayName;
		 xsd__string ResetServerVersion;
         xsd__string URLInfoAbout;
         xsd__string Publisher;
	}MPC__npRequestRequest, *PMPC__npRequestRequest;

	typedef struct _MPC__npRequestResponse
	{
     	xsd__string	AppID;
		xsd__string AppName;
		xsd__string	DisplayName;
		xsd__string	Detailtext;
		xsd__string	Mark;
		xsd__string	Type;
		xsd__string	ServerVersion;
		xsd__string	ResetServerVersion;
		xsd__string Size;
		xsd__string	OrderNumber;
		xsd__string	AllDownload;
		xsd__string	FewDownload;
		xsd__string DownloadURL;
		xsd__string OS;
	}MPC__npRequestResponse, *PMPC__npRequestResponse;

	int MPC__npRequest(MPC__npRequestRequest r_input, MPC__npRequestResponse &r_output); // 更新特定的软件信息

	int MPC__flashReguest(long start_input, PMPC__npRequestResponse pReq, long &count); // 跟新一批软件信息

	int MPC__typeRequest(long type, long order, MPC__npRequestResponse &r_output); // 更新某一类软件信息

#endif


