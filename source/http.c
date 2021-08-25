#include "http.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>

#include "uri.h"

#define SIZE(X) (sizeof(X) / sizeof(*(X)))

const char *http_method_string[] = {
	[HTTP_METHOD_GET]	= "GET",
	[HTTP_METHOD_POST]	= "POST",
	[HTTP_METHOD_PUT]	= "PUT",
	[HTTP_METHOD_DELETE]	= "DELETE",
};

const char *http_response_string[] = {
	[HTTP_RES_OK]		= "OK",
};

const char *http_ctype_string[] = {
	[HTTP_CONTENT_MP4] = "video/mp4"
};

bool http_is_that_header(char *raw)
{
	return strstr(raw, "\r\n\r\n");
}

static char *get_token(char *origin, char *dest, int len)
{
	char *save;
	char *backup_dest = dest;

	while (isspace(*origin))
		origin++;

	save = origin;
	while (!isspace(*origin) && (dest - backup_dest) < len)
		*dest++ = *origin++;

	if (save == origin)
		return NULL;

	*dest = '\0';

	return origin;
}

static int http_method_stot(char *method)
{
	for (int i = 0; i < SIZE(http_method_string); i++)
		if (!strcmp(method, http_method_string[i]))
			return i;

	return -1;
}

static int http_parse_request(struct http_header_request *req, char *raw)
{
	char temp[URI_MAX_LEN];

	raw = get_token(raw, temp, sizeof(temp) - 1);
	if (raw == NULL) return -1;
	else req->method = http_method_stot(temp);

	raw = get_token(raw, temp, sizeof(temp) - 1);
	if (raw == NULL) return -2;
	else req->url = url_create(temp);

	return 0;
}

struct http_header *http_header_create(HTTP_HEADER_TYPE type,
		                       enum http_state_code code,
				       ...)
{
	struct http_header *header;
	int ret;
	va_list ap;

	header = malloc(sizeof(struct http_header));
	if (header == NULL)	return NULL;
	else			va_start(ap, code);

	if (http_header_parse(type, header, va_arg(ap, char *)) < 0)
		return NULL;

	// printf("hello world!\n");

	return header;
}

void http_cleanup_request(struct http_header_request *req)
{
	url_destroy(req->url);
}

// This function can be improved using by Radix Tree
enum http_content_type http_find_ctype(const char *ext)
{
	if (!strcmp(ext, "mp4"))
		return HTTP_CONTENT_MP4;

	return HTTP_CONTENT_UNKNOWN;
}

int http_header_stringify(struct http_header *header, char *raw)
{
	struct http_header_response *res;
	struct http_header_request *req;
	char *ptr = (char *) raw;

	if (header->type == HTTP_HEADER_RESPONSE) {
		res = &header->res;

		ptr += sprintf(ptr, "HTTP/1.1 %d %s\r\n",
			res->state, http_response_string[res->state]
		);

		if (res->content_length > 0) {
			ptr += sprintf(ptr, "Content-Length: %d\r\n",
				res->content_length);
			ptr += sprintf(ptr, "Content-Type: %s\r\n",
				http_ctype_string[res->content_type]);
		}

		ptr += sprintf(ptr, "Connection: close\r\n");
		ptr += sprintf(ptr, "Server: %s\r\n", res->server);

		ptr += sprintf(ptr, "\r\n");
	} else {
		req = &header->req;
	}

	return (void *) ptr - (void *) raw;
}

int http_header_parse(HTTP_HEADER_TYPE type, struct http_header *header, char *raw)
{
	struct http_header_request *req = &header->req;
	struct http_header_response *res = &header->res;

	header->type = type;

	if (type == HTTP_HEADER_REQUEST) {
		char temp[URI_MAX_LEN];

		raw = get_token(raw, temp, sizeof(temp) - 1);
		if (raw == NULL) return -1;
		else req->method = http_method_stot(temp);

		raw = get_token(raw, temp, sizeof(temp) - 1);
		if (raw == NULL) return -2;
		else req->url = url_create(temp);
	} else {
	}


        return 0;
}

void http_cleanup_response(struct http_header_response *res)
{

}

void http_header_destroy(struct http_header *header)
{
	if (header->type == HTTP_HEADER_REQUEST) {
		http_cleanup_request(&header->req);
	} else if (header->type == HTTP_HEADER_RESPONSE) {
		http_cleanup_response(&header->res);
	}
}

