/* opc_soap_Client.cpp
   Generated by gSOAP 2.7.9d from opc_msg.h
   Copyright(C) 2000-2006, Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/
#include "opc_soap_H.h"

SOAP_SOURCE_STAMP("@(#) opc_soap_Client.cpp ver 2.7.9d 2007-02-26 14:18:39 GMT")


SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetStatus(struct soap *soap, const char *soap_endpoint, const char *soap_action, _ns1__GetStatus *ns1__GetStatus, _ns1__GetStatusResponse *ns1__GetStatusResponse)
{	struct __ns1__GetStatus soap_tmp___ns1__GetStatus;
	if (!soap_action)
		soap_action = "http://opcfoundation.org/webservices/XMLDA/1.0/GetStatus";
	soap->encodingStyle = NULL;
	soap_tmp___ns1__GetStatus.ns1__GetStatus = ns1__GetStatus;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___ns1__GetStatus(soap, &soap_tmp___ns1__GetStatus);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__GetStatus(soap, &soap_tmp___ns1__GetStatus, "-ns1:GetStatus", "")
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
	 || soap_put___ns1__GetStatus(soap, &soap_tmp___ns1__GetStatus, "-ns1:GetStatus", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!ns1__GetStatusResponse)
		return soap_closesock(soap);
	ns1__GetStatusResponse->soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__GetStatusResponse->soap_get(soap, "ns1:GetStatusResponse", "");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__Read(struct soap *soap, const char *soap_endpoint, const char *soap_action, _ns1__Read *ns1__Read, _ns1__ReadResponse *ns1__ReadResponse)
{	struct __ns1__Read soap_tmp___ns1__Read;
	if (!soap_action)
		soap_action = "http://opcfoundation.org/webservices/XMLDA/1.0/Read";
	soap->encodingStyle = NULL;
	soap_tmp___ns1__Read.ns1__Read = ns1__Read;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___ns1__Read(soap, &soap_tmp___ns1__Read);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__Read(soap, &soap_tmp___ns1__Read, "-ns1:Read", "")
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
	 || soap_put___ns1__Read(soap, &soap_tmp___ns1__Read, "-ns1:Read", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!ns1__ReadResponse)
		return soap_closesock(soap);
	ns1__ReadResponse->soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__ReadResponse->soap_get(soap, "ns1:ReadResponse", "");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__Write(struct soap *soap, const char *soap_endpoint, const char *soap_action, _ns1__Write *ns1__Write, _ns1__WriteResponse *ns1__WriteResponse)
{	struct __ns1__Write soap_tmp___ns1__Write;
	if (!soap_action)
		soap_action = "http://opcfoundation.org/webservices/XMLDA/1.0/Write";
	soap->encodingStyle = NULL;
	soap_tmp___ns1__Write.ns1__Write = ns1__Write;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___ns1__Write(soap, &soap_tmp___ns1__Write);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__Write(soap, &soap_tmp___ns1__Write, "-ns1:Write", "")
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
	 || soap_put___ns1__Write(soap, &soap_tmp___ns1__Write, "-ns1:Write", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!ns1__WriteResponse)
		return soap_closesock(soap);
	ns1__WriteResponse->soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__WriteResponse->soap_get(soap, "ns1:WriteResponse", "");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__Subscribe(struct soap *soap, const char *soap_endpoint, const char *soap_action, _ns1__Subscribe *ns1__Subscribe, _ns1__SubscribeResponse *ns1__SubscribeResponse)
{	struct __ns1__Subscribe soap_tmp___ns1__Subscribe;
	if (!soap_action)
		soap_action = "http://opcfoundation.org/webservices/XMLDA/1.0/Subscribe";
	soap->encodingStyle = NULL;
	soap_tmp___ns1__Subscribe.ns1__Subscribe = ns1__Subscribe;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___ns1__Subscribe(soap, &soap_tmp___ns1__Subscribe);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__Subscribe(soap, &soap_tmp___ns1__Subscribe, "-ns1:Subscribe", "")
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
	 || soap_put___ns1__Subscribe(soap, &soap_tmp___ns1__Subscribe, "-ns1:Subscribe", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!ns1__SubscribeResponse)
		return soap_closesock(soap);
	ns1__SubscribeResponse->soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__SubscribeResponse->soap_get(soap, "ns1:SubscribeResponse", "");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__SubscriptionPolledRefresh(struct soap *soap, const char *soap_endpoint, const char *soap_action, _ns1__SubscriptionPolledRefresh *ns1__SubscriptionPolledRefresh, _ns1__SubscriptionPolledRefreshResponse *ns1__SubscriptionPolledRefreshResponse)
{	struct __ns1__SubscriptionPolledRefresh soap_tmp___ns1__SubscriptionPolledRefresh;
	if (!soap_action)
		soap_action = "http://opcfoundation.org/webservices/XMLDA/1.0/SubscriptionPolledRefresh";
	soap->encodingStyle = NULL;
	soap_tmp___ns1__SubscriptionPolledRefresh.ns1__SubscriptionPolledRefresh = ns1__SubscriptionPolledRefresh;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___ns1__SubscriptionPolledRefresh(soap, &soap_tmp___ns1__SubscriptionPolledRefresh);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__SubscriptionPolledRefresh(soap, &soap_tmp___ns1__SubscriptionPolledRefresh, "-ns1:SubscriptionPolledRefresh", "")
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
	 || soap_put___ns1__SubscriptionPolledRefresh(soap, &soap_tmp___ns1__SubscriptionPolledRefresh, "-ns1:SubscriptionPolledRefresh", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!ns1__SubscriptionPolledRefreshResponse)
		return soap_closesock(soap);
	ns1__SubscriptionPolledRefreshResponse->soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__SubscriptionPolledRefreshResponse->soap_get(soap, "ns1:SubscriptionPolledRefreshResponse", "");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__SubscriptionCancel(struct soap *soap, const char *soap_endpoint, const char *soap_action, _ns1__SubscriptionCancel *ns1__SubscriptionCancel, _ns1__SubscriptionCancelResponse *ns1__SubscriptionCancelResponse)
{	struct __ns1__SubscriptionCancel soap_tmp___ns1__SubscriptionCancel;
	if (!soap_action)
		soap_action = "http://opcfoundation.org/webservices/XMLDA/1.0/SubscriptionCancel";
	soap->encodingStyle = NULL;
	soap_tmp___ns1__SubscriptionCancel.ns1__SubscriptionCancel = ns1__SubscriptionCancel;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___ns1__SubscriptionCancel(soap, &soap_tmp___ns1__SubscriptionCancel);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__SubscriptionCancel(soap, &soap_tmp___ns1__SubscriptionCancel, "-ns1:SubscriptionCancel", "")
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
	 || soap_put___ns1__SubscriptionCancel(soap, &soap_tmp___ns1__SubscriptionCancel, "-ns1:SubscriptionCancel", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!ns1__SubscriptionCancelResponse)
		return soap_closesock(soap);
	ns1__SubscriptionCancelResponse->soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__SubscriptionCancelResponse->soap_get(soap, "ns1:SubscriptionCancelResponse", "");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__Browse(struct soap *soap, const char *soap_endpoint, const char *soap_action, _ns1__Browse *ns1__Browse, _ns1__BrowseResponse *ns1__BrowseResponse)
{	struct __ns1__Browse soap_tmp___ns1__Browse;
	if (!soap_action)
		soap_action = "http://opcfoundation.org/webservices/XMLDA/1.0/Browse";
	soap->encodingStyle = NULL;
	soap_tmp___ns1__Browse.ns1__Browse = ns1__Browse;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___ns1__Browse(soap, &soap_tmp___ns1__Browse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__Browse(soap, &soap_tmp___ns1__Browse, "-ns1:Browse", "")
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
	 || soap_put___ns1__Browse(soap, &soap_tmp___ns1__Browse, "-ns1:Browse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!ns1__BrowseResponse)
		return soap_closesock(soap);
	ns1__BrowseResponse->soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__BrowseResponse->soap_get(soap, "ns1:BrowseResponse", "");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetProperties(struct soap *soap, const char *soap_endpoint, const char *soap_action, _ns1__GetProperties *ns1__GetProperties, _ns1__GetPropertiesResponse *ns1__GetPropertiesResponse)
{	struct __ns1__GetProperties soap_tmp___ns1__GetProperties;
	if (!soap_action)
		soap_action = "http://opcfoundation.org/webservices/XMLDA/1.0/GetProperties";
	soap->encodingStyle = NULL;
	soap_tmp___ns1__GetProperties.ns1__GetProperties = ns1__GetProperties;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___ns1__GetProperties(soap, &soap_tmp___ns1__GetProperties);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__GetProperties(soap, &soap_tmp___ns1__GetProperties, "-ns1:GetProperties", "")
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
	 || soap_put___ns1__GetProperties(soap, &soap_tmp___ns1__GetProperties, "-ns1:GetProperties", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!ns1__GetPropertiesResponse)
		return soap_closesock(soap);
	ns1__GetPropertiesResponse->soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__GetPropertiesResponse->soap_get(soap, "ns1:GetPropertiesResponse", "");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

/* End of opc_soap_Client.cpp */