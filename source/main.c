#include "http.h"

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

int main(int argc, char *argv[])
{
	struct http_request_header *request;

	request = http_request_header_create(header);

	http_request_header_destroy(request);

	return 0;
}
