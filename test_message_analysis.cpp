#include "include/HTTPMessage.h"

int main()
{
    ifstream file;
    HTTPMessage test;
    test.request = "GET /home/smx/proj/web/data/test_txt.html HTTP/1.1\r\nHost: localhost:5050\r\nConnection: keep-alive\r\nsec-ch-ua: \"Google Chrome\";v=\"93\", \" Not;A Brand\";v=\"99\", \"Chromium\";v=\"93\"\r\nsec-ch-ua-mobile: ?0\r\nsec-ch-ua-platform: \"Windows\"\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/93.0.4577.82 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nSec-Fetch-Site: none\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-User: ?1\r\nSec-Fetch-Dest: document\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: zh-CN,zh;q=0.9\r\n\r\n";
    test.message_analysis();
    file.open(test.url.c_str(), ios_base::in | ios_base::binary);
    if (!file.is_open()) {
		printf("not find\n");
	}
    //test.message_response();
}
/*
http://localhost:5050/home/smx/proj/web/data/test_txt.html
*/