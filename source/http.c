#include "http.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TOKSIZ 128

const char *http_request_method[HTTP_REQUEST_UNKNOWN] = {
	[HTTP_REQUEST_GET] = "GET",
	[HTTP_REQUEST_PUT] = "PUT",
	[HTTP_REQUEST_POST] = "POST",
	[HTTP_REQUEST_HEAD] = "HEAD",
	[HTTP_REQUEST_DELETE] = "DELETE",
	[HTTP_REQUEST_OPTIONS] = "OPTIONS"
};

const char *http_version[HTTP_VERSION_UNKNOWN] = {
	[HTTP_VERSION_1_1] = "HTTP/1.1",
	[HTTP_VERSION_2_0] = "HTTP/2.0",
	[HTTP_VERSION_3_0] = "HTTP/3.0"
};

static size_t read_token(char *string, char *token, size_t maxlen)
{
	size_t readlen = 0;

	while ( !isspace(string[readlen]) && readlen < maxlen) {
		token[readlen] = string[readlen];
		readlen++;
	}

	return readlen;
}

static char *skip_whitespace(char *string)
{
	while (isspace(*string)) string++;

	return string;
}

static enum http_request_method parse_method(char *method, int len)
{
	for (int i = 0; i < HTTP_REQUEST_UNKNOWN; i++)
		if ( !strncmp(http_request_method[i], method, len) )
			return i;

	return HTTP_REQUEST_UNKNOWN;
}

static enum http_version parse_version(char *version, int len)
{
	for (int i = 0; i <HTTP_VERSION_UNKNOWN; i++)
		if ( !strncmp(http_version[i], version, len) )
			return i;

	return HTTP_VERSION_UNKNOWN;
}

struct http_request_header *http_request_header_create(char *request)
{
	struct http_request_header *header;

	header = malloc(sizeof(struct http_header));
	if (header == NULL)
		return NULL;

	char token[TOKSIZ];
	size_t readlen;

	readlen = read_token(request, token, TOKSIZ - 1); request += readlen;
	header->method = parse_method(token, readlen);

	request = skip_whitespace(request);

	readlen = read_token(request, token, TOKSIZ - 1); request += readlen;
	token[readlen] = '\0';
	header->url = strndup(token, readlen);
	if (header->url == NULL)
		goto FREE_HEADER;

	request = skip_whitespace(request);

	readlen = read_token(request, token, TOKSIZ - 1); request += readlen;
	header->version = parse_version(token, readlen);

	return header;

FREE_HEADER:	free(header);
RETURN_NULL:	return NULL;
}

void http_request_header_destroy(struct http_request_header *header)
{
	free(header->url);
	free(header);
}
