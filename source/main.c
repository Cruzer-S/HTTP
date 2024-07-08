#include "http.h"

#include <stdio.h>

char *header =
	"GET / HTTP/1.1\r\n"
	"Host: mythos-cloud.com:443\r\n"
	"Connection: keep-alive\r\n"
	"Cache-Control: max-age=0\r\n"
	"Upgrade-Insecure-Requests: 1\r\n"
	"User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/109.0.0.0 Safari/537.36\r\n"
	"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"
	"Accept-Encoding: gzip, deflate\r\n"
	"Accept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7\r\n"
	"\r\n"
;

/*
HTTP/1.1 200 OK
Access-Control-Allow-Origin: *
Connection: Keep-Alive
Content-Encoding: gzip
Content-Type: text/html; charset=utf-8
Date: Mon, 18 Jul 2016 16:06:00 GMT
Etag: "c561c68d0ba92bbeb8b0f612a9199f722e3a621a"
Keep-Alive: timeout=5, max=997
Last-Modified: Mon, 18 Jul 2016 02:36:04 GMT
Server: Apache
Set-Cookie: mykey=myvalue; expires=Mon, 17-Jul-2017 16:06:00 GMT; Max-Age=31449600; Path=/; secure
Transfer-Encoding: chunked
Vary: Cookie, Accept-Encoding
X-Backend-Server: developer2.webapp.scl3.mozilla.com
X-Cache-Info: not cacheable; meta data too large
X-kuma-revision: 1085259
x-frame-options: DENY
*/

int main(int argc, char *argv[])
{
	struct http_request_header *request;
	struct http_response_header *response;

	request = http_request_header_create(header);

	response = http_make_response_header(HTTP_VERSION_1_1, 200, 4,
		"Connection", "Keep-Alive",
		"Content-Type", "text/html; charset=utf-8",
		"Content-Length", "1024",
		"Server", "mythos server",
		"Date: Tue, 7 Jul 2024 08:02:04 GMT"
		"Last-Modified: Mon, 6 Jul 2024 20:32:12 GMT"
	);

	printf("response:\n%s\n", response->buffer);

	http_request_header_destroy(request);

	return 0;
}
