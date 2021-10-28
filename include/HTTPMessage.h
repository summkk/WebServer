#include <iostream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <algorithm>
#include <fstream>
#include <cstring>

using namespace std;

class HTTPMessage{
public:
	string request;//请求报文
	string url;//url字段，路径
	string m_method;//请求方法 GET
	string m_type;//请求的对象的类型
	int id;
	int client_socket;//会话socket
	sockaddr_in ConnAddrs;
	int flag;//结束信号
	HTTPMessage();
	HTTPMessage(int ids, int c_socket,sockaddr_in ConAddr);
	~HTTPMessage();
	void message_analysis();
	void message_response();
};
class HTTPData{
public:
	HTTPMessage *info;
	HTTPData(){}
	HTTPData(HTTPMessage *a):info(a){}
	~HTTPData(){}
	void deal_message(HTTPMessage *a);
};