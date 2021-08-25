#ifndef HTTP_H__
#define HTTP_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "cjson.h"
#include "uri.h"

#define HTTP_HEADER_SIZE 4096

#define HTTP_HEADER_REQUEST (true)
#define HTTP_HEADER_RESPONSE (false)

#define HTTP_FILENAME_MAX 1024

#define HTTP_HEADER_TYPE bool

enum http_method {
	HTTP_METHOD_GET = 0,
	HTTP_METHOD_POST,
	HTTP_METHOD_PUT,
	HTTP_METHOD_DELETE,

	HTTP_METHOD_SIZE
};

enum http_state_code {
	// Success State Code
	HTTP_RES_OK	= 200,	// Okay
	HTTP_RES_CREATE	= 201,	// Resource created

	// Redirection State Code
	HTTP_RES_MOVEDP	= 301,	// Moved Permanently

	// Failure State Code
	HTTP_RES_BADREQ	= 400,	// Bad Request
	HTTP_RES_UNAUTH	= 401,	// Unauthorized
	HTTP_RES_FORBID	= 403,	// Forbidden
	HTTP_RES_NOTFND	= 404,	// Not Found 
	HTTP_RES_NALLOW	= 405,	// Method Not Allowd

	// Server Error State Code
	HTTP_RES_SERROR	= 500	// Server Error
};

enum http_content_type {
	HTTP_CONTENT_MP4,
	HTTP_CONTENT_JSON,
	HTTP_CONTENT_UNKNOWN
};

extern const char *http_method_string[];
extern const char *http_response_string[];
extern const char *http_ctype_string[];

// HTTP Uniform Resource Locator (URL)

struct http_header {
	HTTP_HEADER_TYPE type;

	union {
		struct http_header_request {
			struct url *url;
			int method;
			int content_length;
			enum http_content_type content_type;
		} req;

		struct http_header_response {
			enum http_state_code state;
			int content_length;
			enum http_content_type content_type;
			char *server;
		} res;
	};
};

bool http_is_that_header(char *raw);

struct http_header *http_header_create(HTTP_HEADER_TYPE type,
		                       enum http_state_code code,
				       ...);

void http_header_destroy(struct http_header *header);

int http_header_stringify(struct http_header *header, char *raw);

int http_header_parse(HTTP_HEADER_TYPE type, struct http_header *header, char *raw);

enum http_content_type http_find_ctype(const char *ext);

#endif
