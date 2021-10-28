#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <functional>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <unistd.h>

#include "include/HTTPMessage.h"
#include "include/ThreadPool.h"

using namespace std;

class Server{
public:
	Server();
	~Server();
	void handel_new_connetion();
};

Server::Server(){}
Server::~Server(){}

void Server::handel_new_connetion()
{
	//创建套接字
	int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//初始化socket元素
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(5050);

	//绑定文件描述符和服务器的ip和端口号
	int nRc = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if(nRc == -1){
		//绑定出错处理
        printf("bind failed\n");
    }
	printf("bind success\n");
	
	//进入监听状态，等待用户发起请求
	nRc = listen(serv_sock, SOMAXCONN);
	if(nRc == -1){
		//监听出错处理
        printf("listen failed\n");
    }
	printf("listen success\n");
	
	socklen_t len = sizeof(sockaddr);
	char Buf[1000] = {};

	ThreadPool pool(3);
	pool.start();
	bool flag = true;
	int messageid = 0;
 	while(flag){
		sockaddr_in ConnAddrs = {};//请求连接的客户端地址
		int sConns = accept(serv_sock, (sockaddr*)&ConnAddrs, &len);
		printf("accept\n");
		HTTPMessage *info = new HTTPMessage(messageid++, sConns, ConnAddrs);
		char recvBuf[1000];
		memset(recvBuf, 0, sizeof(recvBuf));
		if(recv(sConns, recvBuf, 1000, 0) < 0){
			printf("receive failed\n");
		}
		else if(!recvBuf[0]){
			printf("client closed\n");
		}
		else{
			info->request = (string)recvBuf;
			//这个线程池不支持类成员函数?
			typedef std::function<void()> Functor;
			HTTPData *da = new HTTPData(info);
			//da->deal_message(info);
			Functor functor(std::bind(&HTTPData::deal_message, da, info));
			//Functor functor(std::bind(&Server::make_response,this, info->request));
			//终于解决了提交普通成员函数的问题
    		pool.submit(functor);
			close(sConns);
			//pool.stop();
		}
		//delete info;
		//flag = false;
 	}
	pool.stop();
	close(serv_sock);
}
int main()
{
	int threadNum = 4;
	int port = 80;
	Server *myHTTPServer = new Server();
	myHTTPServer->handel_new_connetion();
	return 0;
}

//TODO：检查线程池能否使用