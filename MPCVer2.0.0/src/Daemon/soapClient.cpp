/* soapClient.cpp
   Generated by gSOAP 2.8.14 from MPCSoapI.h

Copyright(C) 2000-2013, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under ONE of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#if defined(__BORLANDC__)
#pragma option push -w-8060
#pragma option push -w-8004
#endif
#include "soapH.h"

SOAP_SOURCE_STAMP("@(#) soapClient.cpp ver 2.8.14 2013-07-30 09:01:56 GMT")


SOAP_FMAC5 int SOAP_FMAC6 soap_call_MPC__npRequest(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _MPC__npRequestRequest r_input, struct _MPC__npRequestResponse &r_output)
{	struct MPC__npRequest soap_tmp_MPC__npRequest;
	soap->encodingStyle = NULL;
	soap_tmp_MPC__npRequest.r_input = r_input;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_MPC__npRequest(soap, &soap_tmp_MPC__npRequest);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_MPC__npRequest(soap, &soap_tmp_MPC__npRequest, "MPC:npRequest", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_MPC__npRequest(soap, &soap_tmp_MPC__npRequest, "MPC:npRequest", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&r_output)
		return soap_closesock(soap);
	soap_default_MPC__npRequestResponse(soap, &r_output);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get_MPC__npRequestResponse(soap, &r_output, "MPC:npRequestResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call_MPC__flashReguest(struct soap *soap, const char *soap_endpoint, const char *soap_action, long start_input, struct _MPC__npRequestResponse *pReq, long &count)
{	struct MPC__flashReguest soap_tmp_MPC__flashReguest;
	struct MPC__flashReguestResponse *soap_tmp_MPC__flashReguestResponse;
	soap->encodingStyle = NULL;
	soap_tmp_MPC__flashReguest.start_input = start_input;
	soap_tmp_MPC__flashReguest.pReq = pReq;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_MPC__flashReguest(soap, &soap_tmp_MPC__flashReguest);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_MPC__flashReguest(soap, &soap_tmp_MPC__flashReguest, "MPC:flashReguest", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_MPC__flashReguest(soap, &soap_tmp_MPC__flashReguest, "MPC:flashReguest", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&count)
		return soap_closesock(soap);
	soap_default_long(soap, &count);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_tmp_MPC__flashReguestResponse = soap_get_MPC__flashReguestResponse(soap, NULL, "MPC:flashReguestResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	count = soap_tmp_MPC__flashReguestResponse->count;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call_MPC__typeRequest(struct soap *soap, const char *soap_endpoint, const char *soap_action, long type, long order, struct _MPC__npRequestResponse &r_output)
{	struct MPC__typeRequest soap_tmp_MPC__typeRequest;
	soap->encodingStyle = NULL;
	soap_tmp_MPC__typeRequest.type = type;
	soap_tmp_MPC__typeRequest.order = order;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_MPC__typeRequest(soap, &soap_tmp_MPC__typeRequest);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_MPC__typeRequest(soap, &soap_tmp_MPC__typeRequest, "MPC:typeRequest", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_MPC__typeRequest(soap, &soap_tmp_MPC__typeRequest, "MPC:typeRequest", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&r_output)
		return soap_closesock(soap);
	soap_default_MPC__npRequestResponse(soap, &r_output);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get_MPC__npRequestResponse(soap, &r_output, "MPC:npRequestResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

#if defined(__BORLANDC__)
#pragma option pop
#pragma option pop
#endif

/* End of soapClient.cpp */
