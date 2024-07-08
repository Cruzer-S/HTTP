#include "http.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

const char *http_request_field[HTTP_REQUEST_FIELD_UNKNOWN] = {
	[HTTP_REQUEST_FIELD_HOST] = "Host",
	[HTTP_REQUEST_FIELD_CONNECTION] = "Connection",
	[HTTP_REQUEST_FIELD_CACHE_CONTROL] = "Cache-Control",
	[HTTP_REQUEST_FIELD_UPGRADE_REQUEST] = "Upgrade-Insecure-Requests",
	[HTTP_REQUEST_FIELD_USER_AGENT] = "User-Agent",
	[HTTP_REQUEST_FIELD_ACCEPT] = "Accept",
	[HTTP_REQUEST_FIELD_ACCEPT_ENCODING] = "Accept-Encoding",
	[HTTP_REQUEST_FIELD_ACCEPT_LANGUAGE] = "Accept-Language"
};

static char *skip_whitespace(char *string)
{
	while (isspace(*string)) string++;

	return string;
}

static char *until_whitespace(char *string)
{
	while (*string != '\0' && !isspace(*string)) string++;

	return string;
}
/*
static enum http_request_method parse_method(char *method, int len)
{
	for (int i = 0; i < HTTP_REQUEST_UNKNOWN; i++)
		if ( !strncmp(http_request_method[i], method, len) )
			return i;

	return HTTP_REQUEST_UNKNOWN;
}

static enum http_version parse_version(char *version, int len)
{
	for (int i = 0; i < HTTP_VERSION_UNKNOWN; i++)
		if ( !strncmp(http_version[i], version, len) )
			return i;

	return HTTP_VERSION_UNKNOWN;
}
*/
static struct http_header_field *parse_request_field(
		struct http_request_header *header,
		char *line, char **next_line
) {
	struct http_header_field *field;
	char *endptr;

	field = malloc(sizeof(struct http_header_field));
	if (field == NULL)
		goto RETURN_NULL;

	field->next = NULL;

	endptr = strchr(line, ':');
	if (endptr == NULL)
		goto FREE_FIELD;

	field->key = line; *endptr = '\0';

	line = endptr + 1; line = skip_whitespace(line);

	field->value = line;
	endptr = strstr(line, "\r\n");
	if (endptr == NULL)
		goto FREE_FIELD;

	*endptr = '\0';

	if (next_line != NULL)
		*next_line = endptr + 2;

	return field;

FREE_FIELD:	free(field);
RETURN_NULL:	return NULL;
}

static char *parse_request_first_line(struct http_request_header *header)
{
	char *endptr, *ptr = header->buffer;

	endptr = until_whitespace(ptr); *endptr = '\0';
	if (ptr == endptr)
		return NULL;

	header->method = ptr; ptr = endptr + 1;
	ptr = skip_whitespace(ptr);

	endptr = until_whitespace(ptr); *endptr = '\0';
	if (ptr == endptr)
		return NULL;

	header->url = ptr; ptr = endptr + 1;
	ptr = skip_whitespace(ptr);

	endptr = strstr(ptr, "\r\n");
	if (endptr == NULL)
		return NULL;

	*endptr = '\0';
	header->version = ptr; ptr = endptr + 1;

	ptr = skip_whitespace(ptr);

	return ptr;
}

int http_request_header_parse(struct http_request_header *header)
{
	struct http_header_field *field;
	char *line;

	line = parse_request_first_line(header);
	if (line == NULL)
		return -1;

	while ( strncmp(line, "\r\n", 2) 
	     && (field = parse_request_field(header, line, &line)) )
	{
		if (header->field_head == NULL) {
			header->field_head = field;
			continue;
		}

		field->next = header->field_head;
		header->field_head = field;
	}

	return 0;
}

struct http_request_header *http_request_header_create(char *string)
{
	struct http_request_header *header;

	size_t header_len = strlen(string);

	if ((header_len + 1) >= HTTP_HEADER_MAX_SIZE)
		goto RETURN_NULL;

	header = malloc(sizeof(struct http_request_header));
	if (header == NULL)
		goto RETURN_NULL;

	memcpy(header->buffer, string, header_len + 1);

	if (http_request_header_parse(header) == -1)
		goto FREE_HEADER;

	return header;

FREE_HEADER:	free(header);
RETURN_NULL:	return NULL;
}

static void free_field_list(struct http_header_field *list)
{
	if (list == NULL)
		return ;

	free_field_list(list->next);

	free(list);
}

struct http_header_field *http_find_field(
	struct http_request_header *header,char *key
) {
	for (struct http_header_field *field = header->field_head;
	     field != NULL; field = field->next)
	{
		if (strstr(field->key, key))
			return  field;
	}

	return NULL;
}

enum http_request_method http_get_method(struct http_request_header *header)
{
	for (int i = 0; i < HTTP_REQUEST_OPTIONS; i++)
		if ( !strcmp(header->method, http_request_method[i]) )
			return i;

	return HTTP_REQUEST_UNKNOWN;
}

void http_request_header_destroy(struct http_request_header *header)
{
	free_field_list(header->field_head);
	free(header);
}
