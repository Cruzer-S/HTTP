#ifndef HTTP_H__
#define HTTP_H__

// GET / HTTP/1.1
// Host: mythos-cloud.com:443
// Connection: keep-alive
// Cache-Control: max-age=0
// Upgrade-Insecure-Requests: 1
// User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/109.0.0.0 Safari/537.36
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
// Accept-Encoding: gzip, deflate
// Accept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7

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
	HTTP_VERSION_1_1,
	HTTP_VERSION_2_0,
	HTTP_VERSION_3_0,

	HTTP_VERSION_UNKNOWN
};

enum http_status {
	HTTP_STATUS_OK = 200
};

struct http_request_header
{
	enum http_request_method method;
	char *url;
	enum http_version version;
};

struct http_response_header
{
	enum http_version version;
	enum http_status status;
};

struct http_header
{
	int type;

	union {
		struct http_request_header req;
		struct http_response_header res;
	};
};

struct http_request_header *http_request_header_create(char *request);
void http_request_header_destroy(struct http_request_header *header);

#endif
