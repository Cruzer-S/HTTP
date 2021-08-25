#ifndef URI_H__
#define URI_H__

#include "cjson.h"

#define URI_MAX_LEN	(1024)

struct url {
	char *path;
	cjson parameter;
	char *anchor;
};

struct uri {
	char *protocol;
	char *hostname;
	char *password;
	char *host;
	char *port;

	struct url url;
};

struct uri *uri_create(char *uristr);
void uri_destroy(struct uri *url);

struct url *url_create(char *urlstr);
void url_destroy(struct url *url);

#endif
