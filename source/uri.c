#include "uri.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>

#include "cjson.h"

#define container_of(PTR, TYPE, MEMBER) \
	((TYPE *) (((void *) PTR) - offsetof(TYPE, MEMBER)))

struct uri_buffer {
	char origin[URI_MAX_LEN + 1];
	struct uri uri;
};

struct uri *uri_create(char *uristr)
{
	struct uri_buffer *real_uri;

	if (strlen(uristr) > URI_MAX_LEN)
		return NULL;

	real_uri = malloc(sizeof(struct uri_buffer));
	if (real_uri == NULL)	return NULL;
	else strcpy(real_uri->origin, uristr);

	return &real_uri->uri;
}

void uri_destroy(struct uri *uri)
{
	free(container_of(uri, struct uri_buffer, uri));
}

int url_split(struct url *url, char *str)
{
	url->path = strtok(str, "?#");
	if (url->path == NULL)
		return -1;

	url->anchor = strtok(NULL, "?#");
	url->parameter = strtok(NULL, "?#");

	return 0;
}

struct url *url_create(char *urlstr)
{
	struct uri_buffer *real_url;

	if (strlen(urlstr) > URI_MAX_LEN)
		return NULL;

	real_url = malloc(sizeof(struct uri_buffer));
	if (real_url == NULL)	return NULL;
	else strcpy(real_url->origin, urlstr);

	if (url_split(&real_url->uri.url, real_url->origin) < 0) {
		free(real_url);
		return NULL;
	}

	return &(real_url->uri.url);
}

void url_destroy(struct url *url)
{
	struct uri *uri_ptr;

	uri_ptr = container_of(url, struct uri, url);

	free(container_of(uri_ptr, struct uri_buffer, uri));
}
