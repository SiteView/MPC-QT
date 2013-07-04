namespace SOAService {

#ifndef _C_SOASERVICE_INTERFACE_
#define _C_SOASERVICE_INTERFACE_


//gsoap MPC service name: MPCSoap
//gsoap MPC service style: rpc
//gsoap MPC service encoding: encoded
//gsoap MPC service namespace: http://127.0.0.1:8089/MPCSoap.wsdl
//gsoap MPC service location: http://127.0.0.1:8089/MPCSoap
//gsoap MPC schema  namespace:	urn:MPCSoap

typedef std::string xsd__string;



typedef struct _MPC__npRequestRequest
{ 
  xsd__string DisplayName;
  xsd__string DisplayVersion;
  xsd__string URLInfoAbout;
  xsd__string Publisher;
}MPC__npRequestRequest,*PMPC__npRequestRequest;

typedef struct _MPC__npRequestResponse
{
 xsd__string	Appid;
 xsd__string	DisplayName;
 xsd__string	Detailtext;
 xsd__string	Mark;
 xsd__string	Type;
 xsd__string	ServerVersion;
 xsd__string	ResetServerVerSion;
 xsd__string	OrderNumber;
 xsd__string	AllDownload;
 xsd__string	FewDownload;
}MPC__npRequestResponse,*PMPC__npRequestResponse;

int MPC__npRequest( MPC__npRequestRequest r_input,  MPC__npRequestResponse& r_output); // 更新特定的软件信息

int MPC__flashReguest( long start_input,  PMPC__npRequestResponse pReq , long & count); // 跟新一批软件信息

int MPC__typeRequest( long type , long order, MPC__npRequestResponse& r_output); // 更新某一类软件信息

#endif

}
