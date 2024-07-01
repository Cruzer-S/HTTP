#ifndef HTTP_H__
#define HTTP_H__

#define HTTP_HEADER_MAX_SIZE (4 * 1024)

// GET / HTTP/1.1
// Host: mythos-cloud.com:443
// Connection: keep-alive
// Cache-Control: max-age=0
// Upgrade-Insecure-Requests: 1
// User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/109.0.0.0 Safari/537.36
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
// Accept-Encoding: gzip, deflate
// Accept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7

enum http_request_field {
	HTTP_REQUEST_FIELD_HOST = 0,
	HTTP_REQUEST_FIELD_CONNECTION,
	HTTP_REQUEST_FIELD_CACHE_CONTROL,
	HTTP_REQUEST_FIELD_UPGRADE_REQUEST,
	HTTP_REQUEST_FIELD_USER_AGENT,
	HTTP_REQUEST_FIELD_ACCEPT,
	HTTP_REQUEST_FIELD_ACCEPT_ENCODING,
	HTTP_REQUEST_FIELD_ACCEPT_LANGUAGE,

	HTTP_REQUEST_FIELD_UNKNOWN
};

enum http_request_method {
	HTTP_REQUEST_GET = 0,
	HTTP_REQUEST_PUT,
	HTTP_REQUEST_POST,
	HTTP_REQUEST_HEAD,
	HTTP_REQUEST_DELETE,
	HTTP_REQUEST_OPTIONS,

	HTTP_REQUEST_UNKNOWN
};

enum http_version {
	HTTP_VERSION_1_1 = 0,
	HTTP_VERSION_2_0,
	HTTP_VERSION_3_0,

	HTTP_VERSION_UNKNOWN
};

struct http_header_field {
	char *key;
	char *value;

	struct http_header_field *next;
};

struct http_request_header
{
	char buffer[HTTP_HEADER_MAX_SIZE];

	char *method; char *url; char *version;

	struct http_header_field *field_head;
};

struct http_request_header *http_request_header_create(char *request);
void http_request_header_destroy(struct http_request_header *header);

#endif
