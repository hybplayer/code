#include <stdio.h>
#include <Windows.h>
#include <process.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#pragma comment(lib,"Ws2_32.lib")
using namespace std;
#define MAXSIZE 65507 //发送数据报文的最大长度
#define HTTP_PORT 80 //http 服务器端口
#define DATELENGTH 50 //时间字节数
#define CACHE_NUM 50  //定义最大缓存数量
//Http 重要头部数据
struct HttpHeader {
	char method[4]; // POST 或者 GET，注意有些为 CONNECT，本实验暂不考虑
	char url[1024]; // 请求的 url
	char host[1024]; // 目标主机
	char cookie[1024 * 10]; //cookie
	HttpHeader() {
		ZeroMemory(this, sizeof(HttpHeader));
	}
};

//Cache信息分别储存在两个文件中，其中一个储存头部信息（Header），另一个储存完整的报文信息
struct cacheHttpHead {
	int index;
	char method[4]; // POST 或者 GET，注意有些为 CONNECT，本实验暂不考虑
	char url[1024]; // 请求的 url
	char host[1024]; // 目标主机
	char date[DATELENGTH];//缓存内容的最后修改时间
	cacheHttpHead() {
		ZeroMemory(this, sizeof(cacheHttpHead));
		index = -1;
	}
	// 通过 id 获得 Cache 头部和报文、头部文件名
	// 无效文件令 id = -1
	cacheHttpHead(int id, string& msgFileName, string& headerFileName) {
		ZeroMemory(this, sizeof(cacheHttpHead));
		index = id;
		ifstream ifsMsg(msgFileName.c_str());
		ifstream ifs(headerFileName.c_str());
		//文件若存在
		if (ifsMsg.good() && ifs.good()) {
			char buf[1024] = { 0 };
			char* p;
			char* ptr;
			const char* delim = "\r\n";
			bool isValid = 1;
			//从编号为id的文件中读取有效的头部信息
			if (ifs.getline(buf, 1024)) {
				p = strtok_s(buf, delim, &ptr);
				memcpy(method, p, strlen(p));
			}
			else isValid = 0;
			if (ifs.getline(buf, 1024)) {
				p = strtok_s(buf, delim, &ptr);
				memcpy(url, p, strlen(p));
			}
			else isValid = 0;
			if (ifs.getline(buf, 1024)) {
				p = strtok_s(buf, delim, &ptr);
				memcpy(host, p, strlen(p));
			}
			else isValid = 0;
			if (ifs.getline(buf, 1024)) {
				p = strtok_s(buf, delim, &ptr);
				memcpy(date, p, strlen(p));
			}
			else isValid = 0;
			if (!isValid) index = -1;
		}
		else {
			//文件不存在即打开（创建）对应名称的新文件
			index = -1;
			ofstream ofs;
			ofs.open(msgFileName, ios::out);
			ofs.close();
			ofs.open(headerFileName, ios::out);
			ofs.close();
		}
	}
	// 通过 id 获得报文文件名和日期
	// 无效文件返回 0
	bool getCacheHttpHeader(int id, string& msgFileName, string& fileDate) {
		char fir = id / 10 + '0', sec = id % 10 + '0';
		msgFileName = (string)"store\\msg" + fir + sec + ".txt";
		string headerFileName = (string)"store\\header" + fir + sec + ".txt";
		(*this) = cacheHttpHead(id, msgFileName, headerFileName);
		if (index != -1) {
			fileDate = date;
			return 1;
		}
		return 0;
	}

	bool operator == (const HttpHeader& o) const {
		if (strcmp(method, o.method)) return 0;
		if (strcmp(url, o.url)) return 0;
		if (strcmp(host, o.host)) return 0;
		return 1;
	}
};

//代理服务器缓存技术
struct CACHE {
	int cache_index;//记录当前应该将缓存放在哪个位置(指针)
	CACHE() {
		cache_index = 0;
	}
	// 通过 id 获得报文、头部文件名
	void getCacheFileName(int id, string& msgFileName, string& headerFileName) {
		char fir = id / 10 + '0', sec = id % 10 + '0';
		msgFileName = (string)"store\\msg" + fir + sec + ".txt";
		headerFileName = (string)"store\\header" + fir + sec + ".txt";
	}
	// 更新下标为 id 的报文和头部文件
	// 若 id 为 -1，表示加入的是新的头部，则更新指针末尾的文件（类似循环队列进行覆盖）
	void update(int id, char* buffer, HttpHeader* httpHeader, char* date) {
		string msgFileName, headerFileName;
		if (id == -1) {
			getCacheFileName(cache_index, msgFileName, headerFileName);
			cache_index++;
			if (cache_index == CACHE_NUM) cache_index = 0;
		}
		else {
			getCacheFileName(id, msgFileName, headerFileName);
		}

		ofstream ofs;
		ofs.open(msgFileName, ios::out);
		ofs.write(buffer, strlen(buffer));
		ofs.close();
		ofs.open(headerFileName, ios::out);
		ofs << httpHeader->method << '\n';
		ofs << httpHeader->url << '\n';
		ofs << httpHeader->host << '\n';
		ofs.write(date, strlen(date));
		ofs.close();
	}
}cache;

BOOL InitSocket();
void ParseHttpHead(char* buffer, HttpHeader* httpHeader);
BOOL ConnectToServer(SOCKET* serverSocket, char* host);
unsigned int __stdcall ProxyThread(LPVOID lpParameter);
int isInCache(HttpHeader* httpHeader, string& fileName, string& fileDate);//寻找缓存中是否存在，如果存在返回index，不存在返回-1
void insertIMS(char* buffer, char* date);//用于修改HTTP报文（If-Modified-Since段）
//代理相关参数
SOCKET ProxyServer;
sockaddr_in ProxyServerAddr;
const int ProxyPort = 10240;
//由于新的连接都使用新线程进行处理，对线程的频繁的创建和销毁特别浪费资源
//可以使用线程池技术提高服务器效率
//const int ProxyThreadMaxNum = 20;
//HANDLE ProxyThreadHandle[ProxyThreadMaxNum] = {0};
//DWORD ProxyThreadDW[ProxyThreadMaxNum] = {0};
struct ProxyParam {
	SOCKET clientSocket;
	SOCKET serverSocket;
};
//选做功能参数定义
bool button1 = false;
bool button2_1 = false;
bool button2_2 = false;
bool button2_3 = false;//取true的时候表示开始运行选做功能
//禁止访问网站
char hitedu[] = "http://www.hit.edu.cn";
char* invalid_website[10] = { hitedu };
const int invalid_website_num = 1;//有多少个禁止网站
//钓鱼网站
char today_hit[] = "http://today.hit.edu.cn";
char* fishing_src = today_hit;//钓鱼网站原网址
char jwes[] = "http://jwts.hit.edu.cn";
char* fishing_dest = jwes;//钓鱼网站目标网址
char jwts[] = "jwts.hit.edu.cn";
char* fishing_dest_host = jwts;//钓鱼目的地址主机名
//限制访问用户
char host[] = "127.0.0.1";
char* restrict_host[10] = { host };
int restrict_host_num = 1;

int main(int argc, char* argv[])
{
	printf("代理服务器正在启动\n");
	printf("初始化...\n");
	if (!InitSocket()) {
		printf("socket 初始化失败\n");
		return -1;
	}
	printf("代理服务器正在运行，监听端口 %d\n", ProxyPort);
	printf("是否启用cache(y or n)\n");
	if (getchar() == 'y')
	{
		button1 = true;
	}
	getchar();
	printf("是否启用网站过滤(y or n)\n");
	if (getchar() == 'y')
	{
		button2_1 = true;
	}
	getchar();
	printf("是否启用用户过滤(y or n)\n");
	if (getchar() == 'y')
	{
		button2_2 = true;
	}
	getchar();
	printf("是否启用网站引导(y or n)\n");
	if (getchar() == 'y')
	{
		button2_3 = true;
	}
	SOCKET acceptSocket = INVALID_SOCKET;
	ProxyParam* lpProxyParam;
	HANDLE hThread;
	DWORD dwThreadID;
	sockaddr_in addr_in;
	int addr_len = sizeof(SOCKADDR);
	//代理服务器不断监听
	while (true) {
		acceptSocket = accept(ProxyServer, (SOCKADDR*)&addr_in, &(addr_len));
		lpProxyParam = new ProxyParam;
		if (lpProxyParam == NULL) {
			continue;
		}
		//受限用户,与列表中匹配不上的都无法访问
		//inet_ntoa转换成网络地址
		bool isRestrict = 0;
		for (int i = 0; i < restrict_host_num; i++) {
			if (strcmp(restrict_host[i], inet_ntoa(addr_in.sin_addr)) == 0 && button2_2)
			{
				printf("该用户访问受限\n");
				isRestrict = 1;
				break;
			}
		}
		if (isRestrict) continue;
		lpProxyParam->clientSocket = acceptSocket;
		hThread = (HANDLE)_beginthreadex(NULL, 0,
			&ProxyThread, (LPVOID)lpProxyParam, 0, 0);
		CloseHandle(hThread);
		Sleep(200);
	}
	closesocket(ProxyServer);
	WSACleanup();
	return 0;
}
//************************************
// Method: InitSocket
// FullName: InitSocket
// Access: public
// Returns: BOOL
// Qualifier: 初始化套接字
//************************************
BOOL InitSocket() {
	//加载套接字库（必须）
	WORD wVersionRequested;
	WSADATA wsaData;
	//套接字加载时错误提示
	int err;
	//版本 2.2
	wVersionRequested = MAKEWORD(2, 2);
	//加载 dll 文件 Scoket 库
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		//找不到 winsock.dll
		printf("加载 winsock 失败，错误代码为: %d\n", WSAGetLastError());
		return FALSE;
	}
	//if中的语句主要用于比对是否是2.2版本
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		printf("不能找到正确的 winsock 版本\n");
		WSACleanup();
		return FALSE;
	}
	//创建的socket文件描述符基于IPV4，TCP
	ProxyServer = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == ProxyServer) {
		printf("创建套接字失败，错误代码为： %d\n", WSAGetLastError());
		return FALSE;
	}
	ProxyServerAddr.sin_family = AF_INET;
	ProxyServerAddr.sin_port = htons(ProxyPort);//整型变量从主机字节顺序转变成网络字节顺序,转换为大端法
	ProxyServerAddr.sin_addr.S_un.S_addr = INADDR_ANY;//泛指本机也就是表示本机的所有IP，多网卡的情况下，这个就表示所有网卡ip地址的意思
	if (bind(ProxyServer, (SOCKADDR*)&ProxyServerAddr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		printf("绑定套接字失败\n");
		return FALSE;
	}
	if (listen(ProxyServer, SOMAXCONN) == SOCKET_ERROR) {
		printf("监听端口%d 失败", ProxyPort);
		return FALSE;
	}
	return TRUE;
}
//************************************
// Method: ProxyThread
// FullName: ProxyThread
// Access: public
// Returns: unsigned int __stdcall
// Qualifier: 线程执行函数
// Parameter: LPVOID lpParameter
//************************************
unsigned int __stdcall ProxyThread(LPVOID lpParameter) {
	ifstream ifs;
	string filename, filedate;

	char Buffer[MAXSIZE];
	char* CacheBuffer;
	ZeroMemory(Buffer, MAXSIZE);
	SOCKADDR_IN clientAddr;
	int length = sizeof(SOCKADDR_IN);
	int recvSize;
	int ret;
	recvSize = recv(((ProxyParam
		*)lpParameter)->clientSocket, Buffer, MAXSIZE, 0);//接收到报文
	if (recvSize <= 0) {
		printf("关闭套接字\n");
		Sleep(200);
		closesocket(((ProxyParam*)lpParameter)->clientSocket);
		closesocket(((ProxyParam*)lpParameter)->serverSocket);
		delete lpParameter;
		_endthreadex(0);
		return 0;
	}
	HttpHeader* httpHeader = new HttpHeader();
	CacheBuffer = new char[recvSize + 1];
	ZeroMemory(CacheBuffer, recvSize + 1);
	memcpy(CacheBuffer, Buffer, recvSize);
	//处理HTTP头部
	ParseHttpHead(CacheBuffer, httpHeader);
	//处理禁止访问网站
	for (int i = 0; i < invalid_website_num; i++) {
		if (strstr(httpHeader->url, invalid_website[i]) != NULL && button2_1)
		{
			printf("\n=====================\n");
			printf("--------该网站已被屏蔽!----------\n");
			printf("关闭套接字\n");
			Sleep(200);
			closesocket(((ProxyParam*)lpParameter)->clientSocket);
			closesocket(((ProxyParam*)lpParameter)->serverSocket);
			delete lpParameter;
			_endthreadex(0);
			return 0;
		}
	}
	//处理钓鱼网站
	if (strstr(httpHeader->url, fishing_src) != NULL && button2_3)
	{
		printf("\n=====================\n");
		printf("-------------已从源网址：%s 转到 目的网址 ：%s ----------------\n", fishing_src, fishing_dest);
		//修改HTTP报文
		memcpy(httpHeader->host, fishing_dest_host, strlen(fishing_dest_host) + 1);
		memcpy(httpHeader->url, fishing_dest, strlen(fishing_dest));
	}
	delete CacheBuffer;
	//连接目标主机
	if (!ConnectToServer(&((ProxyParam
		*)lpParameter)->serverSocket, httpHeader->host)) {
		printf("关闭套接字\n");
		Sleep(200);
		closesocket(((ProxyParam*)lpParameter)->clientSocket);
		closesocket(((ProxyParam*)lpParameter)->serverSocket);
		delete lpParameter;
		_endthreadex(0);
		return 0;
	}
	printf("代理连接主机 %s 成功\n", httpHeader->host);

	int index = isInCache(httpHeader,filename,filedate);
	//如果在缓存中存在
	if (index > -1)
	{
		char* cacheBuffer;
		char Buf[MAXSIZE];
		ZeroMemory(Buf, MAXSIZE);
		memcpy(Buf, Buffer, recvSize);
		char NowDate[DATELENGTH];
		ZeroMemory(NowDate, DATELENGTH);
		strcpy(NowDate, filedate.c_str());
		//插入"If-Modified-Since: "
		insertIMS(Buf, NowDate);
		printf("-------------------请求报文------------------------\n%s\n", Buf);
		ret = send(((ProxyParam
			*)lpParameter)->serverSocket, Buf, strlen(Buf) + 1, 0);
		recvSize = recv(((ProxyParam
			*)lpParameter)->serverSocket, Buf, MAXSIZE, 0);
		printf("------------------Server返回报文-------------------\n%s\n", Buf);
		if (recvSize <= 0) {
			printf("关闭套接字\n");
			Sleep(200);
			closesocket(((ProxyParam*)lpParameter)->clientSocket);
			closesocket(((ProxyParam*)lpParameter)->serverSocket);
			delete lpParameter;
			_endthreadex(0);
			return 0;
		}
		char no_mo[] = "304";
		char* No_Modified = no_mo;

		//没有改变，直接返回cache中的内容
		if (!memcmp(&Buf[9], No_Modified, strlen(No_Modified)))
		{
			ifs.open(filename, ios::in);
			ifs.seekg(0, ifs.end);
			int fileLength = ifs.tellg();
			ifs.seekg(0, ifs.beg);
			ifs.read(Buffer, fileLength);
			ifs.close();
			ret = send(((ProxyParam
				*)lpParameter)->clientSocket, Buffer, strlen(Buffer) + 1, 0);
			printf("将cache中的缓存返回客户端\n");
			printf("============================\n");
			printf("关闭套接字\n");
			Sleep(200);
			closesocket(((ProxyParam*)lpParameter)->clientSocket);
			closesocket(((ProxyParam*)lpParameter)->serverSocket);
			delete lpParameter;
			_endthreadex(0);
			return 0;
		}
	}

	//将客户端发送的 HTTP 数据报文直接转发给目标服务器
	ret = send(((ProxyParam*)lpParameter)->serverSocket, Buffer, strlen(Buffer)
		+ 1, 0);
	//等待目标服务器返回数据
	recvSize = recv(((ProxyParam
		*)lpParameter)->serverSocket, Buffer, MAXSIZE, 0);
	if (recvSize <= 0) {
		printf("关闭套接字\n");
		Sleep(200);
		closesocket(((ProxyParam*)lpParameter)->clientSocket);
		closesocket(((ProxyParam*)lpParameter)->serverSocket);
		delete lpParameter;
		_endthreadex(0);
		return 0;
	}
	//以下部分将返回报文加入缓存
	//从服务器返回报文中解析时间
	char* cacheBuffer2 = new char[MAXSIZE];
	ZeroMemory(cacheBuffer2, MAXSIZE);
	memcpy(cacheBuffer2, Buffer, MAXSIZE);
	char a_1[] = "\r\n";
	char* delim = a_1;
	char date[DATELENGTH];
	char* nextStr;
	ZeroMemory(date, DATELENGTH);
	char* p = strtok_s(cacheBuffer2, delim, &nextStr);
	bool flag = false;//表示是否含有修改时间报文
	//不断分行，直到分出具有修改时间的那一行
	while (p)
	{
		if (p[0] == 'L')//找到Last-Modified:那一行
		{
			if (strlen(p) > 15)
			{
				char header[15];
				ZeroMemory(header, sizeof(header));
				memcpy(header, p, 14);
				if (!(strcmp(header, "Last-Modified:")))
				{
					memcpy(date, &p[15], strlen(p) - 15);
					flag = true;
					break;
				}
			}
		}
		p = strtok_s(NULL, delim, &nextStr);
	}
	if (flag && button1)
	{
		cache.update(index, Buffer, httpHeader, date);
	}
	//将目标服务器返回的数据直接转发给客户端
	ret = send(((ProxyParam
		*)lpParameter)->clientSocket, Buffer, sizeof(Buffer), 0);
	//错误处理
error:
	printf("关闭套接字\n");
	Sleep(200);
	closesocket(((ProxyParam*)lpParameter)->clientSocket);
	closesocket(((ProxyParam*)lpParameter)->serverSocket);
	delete lpParameter;
	_endthreadex(0);
	return 0;
}
//************************************
// Method: ParseHttpHead
// FullName: ParseHttpHead
// Access: public
// Returns: void
// Qualifier: 解析 TCP 报文中的 HTTP 头部
// Parameter: char * buffer
// Parameter: HttpHeader * httpHeader
//************************************
void ParseHttpHead(char* buffer, HttpHeader* httpHeader) {
	char* p;
	char* ptr;
	const char* delim = "\r\n";
	p = strtok_s(buffer, delim, &ptr);//提取第一行
	printf("%s\n", p);
	if (p[0] == 'G') {//GET 方式
		memcpy(httpHeader->method, "GET", 3);
		memcpy(httpHeader->url, &p[4], strlen(p) - 13);
	}
	else if (p[0] == 'P') {//POST 方式
		memcpy(httpHeader->method, "POST", 4);
		memcpy(httpHeader->url, &p[5], strlen(p) - 14);
	}
	else if (p[0] == 'C') {//CONNECT 方式
		memcpy(httpHeader->method, "CONN", 4);
		memcpy(httpHeader->url, &p[8], strlen(p) - 17);
	}
	printf("%s\n", httpHeader->url);
	p = strtok_s(NULL, delim, &ptr);
	while (p) {
		switch (p[0]) {
		case 'H'://Host
			memcpy(httpHeader->host, &p[6], strlen(p) - 6);
			break;
		case 'C'://Cookie
			if (strlen(p) > 8) {
				char header[8];
				ZeroMemory(header, sizeof(header));
				memcpy(header, p, 6);
				if (!strcmp(header, "Cookie")) {
					memcpy(httpHeader->cookie, &p[8], strlen(p) - 8);
				}
			}
			break;
		default:
			break;
		}
		p = strtok_s(NULL, delim, &ptr);
	}
}
//************************************
// Method: ConnectToServer
// FullName: ConnectToServer
// Access: public
// Returns: BOOL
// Qualifier: 根据主机创建目标服务器套接字，并连接
// Parameter: SOCKET * serverSocket
// Parameter: char * host
//************************************
BOOL ConnectToServer(SOCKET* serverSocket, char* host) {
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(HTTP_PORT);
	HOSTENT* hostent = gethostbyname(host);
	if (!hostent) {
		return FALSE;
	}
	in_addr Inaddr = *((in_addr*)*hostent->h_addr_list);
	serverAddr.sin_addr.s_addr = inet_addr(inet_ntoa(Inaddr));//将一个将网络地址转换成一个长整数型数
	*serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (*serverSocket == INVALID_SOCKET) {
		return FALSE;
	}
	if (connect(*serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr))
		== SOCKET_ERROR) {
		closesocket(*serverSocket);
		return FALSE;
	}
	return TRUE;
}
int isInCache(HttpHeader* httpHeader, string& msgFileName, string& fileDate)
{
	//遍历所有缓存文件，判断其是否命中，命中返回命中文件编号，否则返回-1
	for (int i = 0; i < CACHE_NUM; i++) {
		cacheHttpHead cacheHttpHeader;
		if (cacheHttpHeader.getCacheHttpHeader(i, msgFileName, fileDate)) {
			if (cacheHttpHeader == (*httpHeader)) {
				return i;
			}
		}
	}
	return -1;
}
void insertIMS(char* buffer, char* date)
{
	//此函数在HTTP中间插入"If-Modified-Since: "
	const char* strHost = "Host";
	const char* inputStr = "If-Modified-Since: ";
	char temp[MAXSIZE];
	ZeroMemory(temp, MAXSIZE);
	char* pos = strstr(buffer, strHost);//找到Host位置
	int i = 0;
	//将host与之后的部分写入temp
	for (i = 0; i < strlen(pos); i++) {
		temp[i] = pos[i];
	}
	*pos = '\0';
	while (*inputStr != '\0') {  //插入If-Modified-Since字段
		*pos++ = *inputStr++;
	}
	while (*date != '\0') {
		*pos++ = *date++;
	}
	*pos++ = '\r';
	*pos++ = '\n';
	//将host之后的字段复制到buffer中
	for (i = 0; i < strlen(temp); i++) {
		*pos++ = temp[i];
	}
}