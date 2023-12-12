#include <stdio.h>
#include <Windows.h>
#include <process.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#pragma comment(lib,"Ws2_32.lib")
using namespace std;
#define MAXSIZE 65507 //�������ݱ��ĵ���󳤶�
#define HTTP_PORT 80 //http �������˿�
#define DATELENGTH 50 //ʱ���ֽ���
#define CACHE_NUM 50  //������󻺴�����
//Http ��Ҫͷ������
struct HttpHeader {
	char method[4]; // POST ���� GET��ע����ЩΪ CONNECT����ʵ���ݲ�����
	char url[1024]; // ����� url
	char host[1024]; // Ŀ������
	char cookie[1024 * 10]; //cookie
	HttpHeader() {
		ZeroMemory(this, sizeof(HttpHeader));
	}
};

//Cache��Ϣ�ֱ𴢴��������ļ��У�����һ������ͷ����Ϣ��Header������һ�����������ı�����Ϣ
struct cacheHttpHead {
	int index;
	char method[4]; // POST ���� GET��ע����ЩΪ CONNECT����ʵ���ݲ�����
	char url[1024]; // ����� url
	char host[1024]; // Ŀ������
	char date[DATELENGTH];//�������ݵ�����޸�ʱ��
	cacheHttpHead() {
		ZeroMemory(this, sizeof(cacheHttpHead));
		index = -1;
	}
	// ͨ�� id ��� Cache ͷ���ͱ��ġ�ͷ���ļ���
	// ��Ч�ļ��� id = -1
	cacheHttpHead(int id, string& msgFileName, string& headerFileName) {
		ZeroMemory(this, sizeof(cacheHttpHead));
		index = id;
		ifstream ifsMsg(msgFileName.c_str());
		ifstream ifs(headerFileName.c_str());
		//�ļ�������
		if (ifsMsg.good() && ifs.good()) {
			char buf[1024] = { 0 };
			char* p;
			char* ptr;
			const char* delim = "\r\n";
			bool isValid = 1;
			//�ӱ��Ϊid���ļ��ж�ȡ��Ч��ͷ����Ϣ
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
			//�ļ������ڼ��򿪣���������Ӧ���Ƶ����ļ�
			index = -1;
			ofstream ofs;
			ofs.open(msgFileName, ios::out);
			ofs.close();
			ofs.open(headerFileName, ios::out);
			ofs.close();
		}
	}
	// ͨ�� id ��ñ����ļ���������
	// ��Ч�ļ����� 0
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

//������������漼��
struct CACHE {
	int cache_index;//��¼��ǰӦ�ý���������ĸ�λ��(ָ��)
	CACHE() {
		cache_index = 0;
	}
	// ͨ�� id ��ñ��ġ�ͷ���ļ���
	void getCacheFileName(int id, string& msgFileName, string& headerFileName) {
		char fir = id / 10 + '0', sec = id % 10 + '0';
		msgFileName = (string)"store\\msg" + fir + sec + ".txt";
		headerFileName = (string)"store\\header" + fir + sec + ".txt";
	}
	// �����±�Ϊ id �ı��ĺ�ͷ���ļ�
	// �� id Ϊ -1����ʾ��������µ�ͷ���������ָ��ĩβ���ļ�������ѭ�����н��и��ǣ�
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
int isInCache(HttpHeader* httpHeader, string& fileName, string& fileDate);//Ѱ�һ������Ƿ���ڣ�������ڷ���index�������ڷ���-1
void insertIMS(char* buffer, char* date);//�����޸�HTTP���ģ�If-Modified-Since�Σ�
//������ز���
SOCKET ProxyServer;
sockaddr_in ProxyServerAddr;
const int ProxyPort = 10240;
//�����µ����Ӷ�ʹ�����߳̽��д������̵߳�Ƶ���Ĵ����������ر��˷���Դ
//����ʹ���̳߳ؼ�����߷�����Ч��
//const int ProxyThreadMaxNum = 20;
//HANDLE ProxyThreadHandle[ProxyThreadMaxNum] = {0};
//DWORD ProxyThreadDW[ProxyThreadMaxNum] = {0};
struct ProxyParam {
	SOCKET clientSocket;
	SOCKET serverSocket;
};
//ѡ�����ܲ�������
bool button1 = false;
bool button2_1 = false;
bool button2_2 = false;
bool button2_3 = false;//ȡtrue��ʱ���ʾ��ʼ����ѡ������
//��ֹ������վ
char hitedu[] = "http://www.hit.edu.cn";
char* invalid_website[10] = { hitedu };
const int invalid_website_num = 1;//�ж��ٸ���ֹ��վ
//������վ
char today_hit[] = "http://today.hit.edu.cn";
char* fishing_src = today_hit;//������վԭ��ַ
char jwes[] = "http://jwts.hit.edu.cn";
char* fishing_dest = jwes;//������վĿ����ַ
char jwts[] = "jwts.hit.edu.cn";
char* fishing_dest_host = jwts;//����Ŀ�ĵ�ַ������
//���Ʒ����û�
char host[] = "127.0.0.1";
char* restrict_host[10] = { host };
int restrict_host_num = 1;

int main(int argc, char* argv[])
{
	printf("�����������������\n");
	printf("��ʼ��...\n");
	if (!InitSocket()) {
		printf("socket ��ʼ��ʧ��\n");
		return -1;
	}
	printf("����������������У������˿� %d\n", ProxyPort);
	printf("�Ƿ�����cache(y or n)\n");
	if (getchar() == 'y')
	{
		button1 = true;
	}
	getchar();
	printf("�Ƿ�������վ����(y or n)\n");
	if (getchar() == 'y')
	{
		button2_1 = true;
	}
	getchar();
	printf("�Ƿ������û�����(y or n)\n");
	if (getchar() == 'y')
	{
		button2_2 = true;
	}
	getchar();
	printf("�Ƿ�������վ����(y or n)\n");
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
	//������������ϼ���
	while (true) {
		acceptSocket = accept(ProxyServer, (SOCKADDR*)&addr_in, &(addr_len));
		lpProxyParam = new ProxyParam;
		if (lpProxyParam == NULL) {
			continue;
		}
		//�����û�,���б���ƥ�䲻�ϵĶ��޷�����
		//inet_ntoaת���������ַ
		bool isRestrict = 0;
		for (int i = 0; i < restrict_host_num; i++) {
			if (strcmp(restrict_host[i], inet_ntoa(addr_in.sin_addr)) == 0 && button2_2)
			{
				printf("���û���������\n");
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
// Qualifier: ��ʼ���׽���
//************************************
BOOL InitSocket() {
	//�����׽��ֿ⣨���룩
	WORD wVersionRequested;
	WSADATA wsaData;
	//�׽��ּ���ʱ������ʾ
	int err;
	//�汾 2.2
	wVersionRequested = MAKEWORD(2, 2);
	//���� dll �ļ� Scoket ��
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		//�Ҳ��� winsock.dll
		printf("���� winsock ʧ�ܣ��������Ϊ: %d\n", WSAGetLastError());
		return FALSE;
	}
	//if�е������Ҫ���ڱȶ��Ƿ���2.2�汾
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		printf("�����ҵ���ȷ�� winsock �汾\n");
		WSACleanup();
		return FALSE;
	}
	//������socket�ļ�����������IPV4��TCP
	ProxyServer = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == ProxyServer) {
		printf("�����׽���ʧ�ܣ��������Ϊ�� %d\n", WSAGetLastError());
		return FALSE;
	}
	ProxyServerAddr.sin_family = AF_INET;
	ProxyServerAddr.sin_port = htons(ProxyPort);//���ͱ����������ֽ�˳��ת��������ֽ�˳��,ת��Ϊ��˷�
	ProxyServerAddr.sin_addr.S_un.S_addr = INADDR_ANY;//��ָ����Ҳ���Ǳ�ʾ����������IP��������������£�����ͱ�ʾ��������ip��ַ����˼
	if (bind(ProxyServer, (SOCKADDR*)&ProxyServerAddr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		printf("���׽���ʧ��\n");
		return FALSE;
	}
	if (listen(ProxyServer, SOMAXCONN) == SOCKET_ERROR) {
		printf("�����˿�%d ʧ��", ProxyPort);
		return FALSE;
	}
	return TRUE;
}
//************************************
// Method: ProxyThread
// FullName: ProxyThread
// Access: public
// Returns: unsigned int __stdcall
// Qualifier: �߳�ִ�к���
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
		*)lpParameter)->clientSocket, Buffer, MAXSIZE, 0);//���յ�����
	if (recvSize <= 0) {
		printf("�ر��׽���\n");
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
	//����HTTPͷ��
	ParseHttpHead(CacheBuffer, httpHeader);
	//�����ֹ������վ
	for (int i = 0; i < invalid_website_num; i++) {
		if (strstr(httpHeader->url, invalid_website[i]) != NULL && button2_1)
		{
			printf("\n=====================\n");
			printf("--------����վ�ѱ�����!----------\n");
			printf("�ر��׽���\n");
			Sleep(200);
			closesocket(((ProxyParam*)lpParameter)->clientSocket);
			closesocket(((ProxyParam*)lpParameter)->serverSocket);
			delete lpParameter;
			_endthreadex(0);
			return 0;
		}
	}
	//���������վ
	if (strstr(httpHeader->url, fishing_src) != NULL && button2_3)
	{
		printf("\n=====================\n");
		printf("-------------�Ѵ�Դ��ַ��%s ת�� Ŀ����ַ ��%s ----------------\n", fishing_src, fishing_dest);
		//�޸�HTTP����
		memcpy(httpHeader->host, fishing_dest_host, strlen(fishing_dest_host) + 1);
		memcpy(httpHeader->url, fishing_dest, strlen(fishing_dest));
	}
	delete CacheBuffer;
	//����Ŀ������
	if (!ConnectToServer(&((ProxyParam
		*)lpParameter)->serverSocket, httpHeader->host)) {
		printf("�ر��׽���\n");
		Sleep(200);
		closesocket(((ProxyParam*)lpParameter)->clientSocket);
		closesocket(((ProxyParam*)lpParameter)->serverSocket);
		delete lpParameter;
		_endthreadex(0);
		return 0;
	}
	printf("������������ %s �ɹ�\n", httpHeader->host);

	int index = isInCache(httpHeader,filename,filedate);
	//����ڻ����д���
	if (index > -1)
	{
		char* cacheBuffer;
		char Buf[MAXSIZE];
		ZeroMemory(Buf, MAXSIZE);
		memcpy(Buf, Buffer, recvSize);
		char NowDate[DATELENGTH];
		ZeroMemory(NowDate, DATELENGTH);
		strcpy(NowDate, filedate.c_str());
		//����"If-Modified-Since: "
		insertIMS(Buf, NowDate);
		printf("-------------------������------------------------\n%s\n", Buf);
		ret = send(((ProxyParam
			*)lpParameter)->serverSocket, Buf, strlen(Buf) + 1, 0);
		recvSize = recv(((ProxyParam
			*)lpParameter)->serverSocket, Buf, MAXSIZE, 0);
		printf("------------------Server���ر���-------------------\n%s\n", Buf);
		if (recvSize <= 0) {
			printf("�ر��׽���\n");
			Sleep(200);
			closesocket(((ProxyParam*)lpParameter)->clientSocket);
			closesocket(((ProxyParam*)lpParameter)->serverSocket);
			delete lpParameter;
			_endthreadex(0);
			return 0;
		}
		char no_mo[] = "304";
		char* No_Modified = no_mo;

		//û�иı䣬ֱ�ӷ���cache�е�����
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
			printf("��cache�еĻ��淵�ؿͻ���\n");
			printf("============================\n");
			printf("�ر��׽���\n");
			Sleep(200);
			closesocket(((ProxyParam*)lpParameter)->clientSocket);
			closesocket(((ProxyParam*)lpParameter)->serverSocket);
			delete lpParameter;
			_endthreadex(0);
			return 0;
		}
	}

	//���ͻ��˷��͵� HTTP ���ݱ���ֱ��ת����Ŀ�������
	ret = send(((ProxyParam*)lpParameter)->serverSocket, Buffer, strlen(Buffer)
		+ 1, 0);
	//�ȴ�Ŀ���������������
	recvSize = recv(((ProxyParam
		*)lpParameter)->serverSocket, Buffer, MAXSIZE, 0);
	if (recvSize <= 0) {
		printf("�ر��׽���\n");
		Sleep(200);
		closesocket(((ProxyParam*)lpParameter)->clientSocket);
		closesocket(((ProxyParam*)lpParameter)->serverSocket);
		delete lpParameter;
		_endthreadex(0);
		return 0;
	}
	//���²��ֽ����ر��ļ��뻺��
	//�ӷ��������ر����н���ʱ��
	char* cacheBuffer2 = new char[MAXSIZE];
	ZeroMemory(cacheBuffer2, MAXSIZE);
	memcpy(cacheBuffer2, Buffer, MAXSIZE);
	char a_1[] = "\r\n";
	char* delim = a_1;
	char date[DATELENGTH];
	char* nextStr;
	ZeroMemory(date, DATELENGTH);
	char* p = strtok_s(cacheBuffer2, delim, &nextStr);
	bool flag = false;//��ʾ�Ƿ����޸�ʱ�䱨��
	//���Ϸ��У�ֱ���ֳ������޸�ʱ�����һ��
	while (p)
	{
		if (p[0] == 'L')//�ҵ�Last-Modified:��һ��
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
	//��Ŀ����������ص�����ֱ��ת�����ͻ���
	ret = send(((ProxyParam
		*)lpParameter)->clientSocket, Buffer, sizeof(Buffer), 0);
	//������
error:
	printf("�ر��׽���\n");
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
// Qualifier: ���� TCP �����е� HTTP ͷ��
// Parameter: char * buffer
// Parameter: HttpHeader * httpHeader
//************************************
void ParseHttpHead(char* buffer, HttpHeader* httpHeader) {
	char* p;
	char* ptr;
	const char* delim = "\r\n";
	p = strtok_s(buffer, delim, &ptr);//��ȡ��һ��
	printf("%s\n", p);
	if (p[0] == 'G') {//GET ��ʽ
		memcpy(httpHeader->method, "GET", 3);
		memcpy(httpHeader->url, &p[4], strlen(p) - 13);
	}
	else if (p[0] == 'P') {//POST ��ʽ
		memcpy(httpHeader->method, "POST", 4);
		memcpy(httpHeader->url, &p[5], strlen(p) - 14);
	}
	else if (p[0] == 'C') {//CONNECT ��ʽ
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
// Qualifier: ������������Ŀ��������׽��֣�������
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
	serverAddr.sin_addr.s_addr = inet_addr(inet_ntoa(Inaddr));//��һ���������ַת����һ������������
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
	//�������л����ļ����ж����Ƿ����У����з��������ļ���ţ����򷵻�-1
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
	//�˺�����HTTP�м����"If-Modified-Since: "
	const char* strHost = "Host";
	const char* inputStr = "If-Modified-Since: ";
	char temp[MAXSIZE];
	ZeroMemory(temp, MAXSIZE);
	char* pos = strstr(buffer, strHost);//�ҵ�Hostλ��
	int i = 0;
	//��host��֮��Ĳ���д��temp
	for (i = 0; i < strlen(pos); i++) {
		temp[i] = pos[i];
	}
	*pos = '\0';
	while (*inputStr != '\0') {  //����If-Modified-Since�ֶ�
		*pos++ = *inputStr++;
	}
	while (*date != '\0') {
		*pos++ = *date++;
	}
	*pos++ = '\r';
	*pos++ = '\n';
	//��host֮����ֶθ��Ƶ�buffer��
	for (i = 0; i < strlen(temp); i++) {
		*pos++ = temp[i];
	}
}