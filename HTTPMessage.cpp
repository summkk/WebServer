#include "include/HTTPMessage.h"


HTTPMessage::HTTPMessage(){}
HTTPMessage::~HTTPMessage(){}
HTTPMessage::HTTPMessage(int ids, int c_socket,sockaddr_in ConAddr):id(ids),client_socket(c_socket),ConnAddrs(ConAddr){
	request = "";
	url = "";
	m_method = "";
	m_type = "";
}
void HTTPData::deal_message(HTTPMessage *test){
	test->message_analysis();
	test->message_response();
}
void HTTPMessage::message_response(){
	char buf[1000];
	string state;
	string error = "/error404.html";
	memset(buf, 0, sizeof(buf));
	ifstream file;
	if (flag == 0) {
		file.open("data/close.html", ios_base::in | ios_base::binary);
		state = "HTTP/1.1 200 OK\r\n";
	}
	else if (m_method == "GET") {
		file.open(url.c_str(), ios_base::in | ios_base::binary);
		printf("GET object file path:%s\n\n", url.c_str());
		if (!file.is_open()) {
			state = "HTTP/1.1 404 Not Found\r\n";
			printf("not find\n");
			file.close();
			file.open(("data" + error).c_str(), ios_base::in | ios_base::binary);
		}
		else
		{
			state = "HTTP/1.1 200 OK\r\n";
		}
	}

	file.seekg(0, ios_base::end);
	int size = file.tellg();

	strcpy(buf, state.c_str());
	sprintf(buf + strlen(buf), "Content-Type: %s\r\n", m_type.c_str());
	sprintf(buf + strlen(buf), "Content-Length: %d\r\n", size);
	sprintf(buf + strlen(buf), "Server: Apache/2.2.3\r\n");
	sprintf(buf + strlen(buf), "Cache-Control: no-cache\r\n");
	sprintf(buf + strlen(buf), "Connection: close\r\n\r\n");

	printf("RESPOND HEADER：\n%s", buf);

	if (send(client_socket, buf, strlen(buf), 0) < 0) {
		printf("send failed\n");
	}

	file.clear();
	file.seekg(0, ios_base::beg);
	int remain = size;
	char databuf[1000];
	while (remain > 0) {
		int tosend = min((int)sizeof(databuf), remain);
		file.read(databuf, sizeof(databuf));
		if (send(client_socket, databuf, tosend, 0) < 0) {
			printf("send failed\n");
		}
		remain -= sizeof(databuf);
	}
	file.close();

	return;
}
void HTTPMessage::message_analysis(){ 
	string method,obj,protocal;	
	cout << "RECIVE "<< request << endl;
	//得到方法，对象，协议类型
	istringstream str(request);
	str >> method;
	str >> obj;
	str >> protocal;
	if (obj == "/exit")
		flag = 0;
	else{
		flag = 1;
	}
	m_method = method;
	if (m_method == "GET") {
		string tmp = string(obj);
		string type;
		//url = root + tmp;
		url = tmp;
		int i = tmp.find(".") + 1;
		int len = tmp.length() - i;
		type = tmp.substr(i, len);

		if (type == "html") m_type = "text/html";
		else if (type == "png") m_type = "image/png";
		else if (type == "jpg") m_type = "image/jpg";
		else if (type == "ico") m_type = "x-ico";
	}
	return;

}