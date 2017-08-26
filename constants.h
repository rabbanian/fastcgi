#ifndef FASTCGI_CONSTANTS_H
#define FASTCGI_CONSTANTS_H


#define FCGI_LISTENSOCK_FILENO	0

#define FCGI_HEADER_LEN		8

#define FCGI_VERSION_1		1

#define FCGI_BEGIN_REQUEST	1
#define FCGI_ABORT_REQUEST	2
#define FCGI_END_REQUEST	3
#define FCGI_PARAMS		4
#define FCGI_STDIN		5
#define FCGI_STDOUT		6
#define FCGI_STDERR		7
#define FCGI_DATA		8
#define FCGI_GET_VALUES		9
#define FCGI_GET_VALUES_RESULT	10
#define FCGI_UNKNOWN_TYPE	11
#define FCGI_MAXTYPE		11

#define FCGI_NULL_REQUEST_ID	0

#define FCGI_KEEP_CONN		1

#define FCGI_RESPONDER		1
#define FCGI_AUTHORIZER		2
#define FCGI_FILTER		3

#define FCGI_REQUEST_COMPLETE	0
#define FCGI_CANT_MPX_CONN	1
#define FCGI_OVERLOADED		2
#define FCGI_UNKNOWN_ROLE	3

//the maximum number of concurrent transport connections this application will accept, e.g. 1 or 10
#define FCGI_MAX_CONNS		5
//the maximum number of concurrent request this application will accept, e.g. 1 or 50
#define FCGI_MAX_REQS		1
//0 if this application does not multiplex connections (i.e. handle concurrent requests over each connection), 1 otherwise
#define FCGI_MPXS_CONNS		0

#define FCGI_BUFFER_SIZE    1024

#endif //FASTCGI_CONSTANTS_H
