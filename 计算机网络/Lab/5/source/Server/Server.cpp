#include "Winsock.h"
#include "windows.h"
#include <iostream>
#include <string>
using namespace std;

#define RECV_PORT 3312	//接收端口
#define SEND_PORT 4302	//发送端口
#pragma comment(lib, "wsock32.lib")

SOCKET sockClient, sockServer;
sockaddr_in severAddr;//服务器地址
sockaddr_in ClientAddr;//客户端地址 

int addrLen;		//地址长度
char fileName[20];	//文件名
char order[20];		//命令
char rbuff[1024];	//接收缓冲区
char sbuff[1024];	//发送缓冲区

char namePassword[1024] = "hhz 123456";	//用户名和密码

//***************函数声明***************

DWORD startSock();
DWORD createSocket();
int sendFileRecord(SOCKET datatcps, WIN32_FIND_DATA* pfd);
int sendFileList(SOCKET datatcps);
int sendFile(SOCKET datatcps, FILE* file);
DWORD connectProcess();

//***************函数声明***************
DWORD startSock() {//初始化winsock
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
		cout << "初始化失败" << endl;
		return -1;
	}
	return 1;
}
DWORD createSocket() {
	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (sockClient == SOCKET_ERROR) {
		cout << "创建失败" << endl;
		WSACleanup();
		return -1;
	}
	severAddr.sin_family = AF_INET;
	severAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	severAddr.sin_port = htons(RECV_PORT);
	if (bind(sockClient, (struct sockaddr FAR*) & severAddr, sizeof(severAddr)) == SOCKET_ERROR) {
		//bind函数用于将socket和地址结构绑定
		cout << "绑定失败" << endl;
		return -1;
	}
	return 1;
}
DWORD connectProcess() {
	addrLen = sizeof(ClientAddr);//addrLen是对象地址的长度 
	if (listen(sockClient, 10) < 0) {//让套接字进入被动监听状态，参数2为请求队列的最大长度
		cout << "监听失败" << endl;
		return -1;
	}
	cout << "服务器正在监听中…" << endl;
	while (1) {
		//accept取出队列头部的连接请求
		//sockclient是处于监听的套接字
		//ClientAddr 是监听的对象地址
		sockServer = accept(sockClient, (struct sockaddr FAR*) & ClientAddr, &addrLen);
		while (1) {
			memset(rbuff, 0, sizeof(rbuff));
			memset(sbuff, 0, sizeof(sbuff));
			if (recv(sockServer, rbuff, sizeof(rbuff), 0) <= 0) {
				break;
			}
			cout << endl << "获取并执行的命令：" << rbuff << endl;
			if (strncmp(rbuff, "get", 3) == 0) {
				strcpy(fileName, rbuff + 4);
				FILE* file;//定义一个文件访问指针
				//处理下载文件请求
				file = fopen(fileName, "r");//二进制打开文件，只允许读
				if (file) {
					sprintf(sbuff, "get %s", fileName);
					if (!send(sockServer, sbuff, sizeof(sbuff), 0)) {
						fclose(file);
						return 0;
					}
					else {//创建额外数据连接传送数据
						if (!sendFile(sockServer, file)) {
							return 0;
						}
						fclose(file);
					}
				}
				else {
					strcpy(sbuff, "无法打开文件\n");
					if (send(sockServer, sbuff, sizeof(sbuff), 0)) {
						return 0;
					}
				}
			}//get
			else if (strncmp(rbuff, "put", 3) == 0) {
				FILE* fd;
				int cnt;
				strcpy(fileName, rbuff + 4);
				fd = fopen(fileName, "w+");
				if (fd == NULL) {
					cout << "无法打开文件" << fileName << endl;
					return 0;
				}
				sprintf(sbuff, "put %s", fileName);
				if (!send(sockServer, sbuff, sizeof(sbuff), 0)) {
					fclose(fd);
					return 0;
				}
				memset(sbuff, '\0', sizeof(rbuff));
				while ((cnt = recv(sockServer, rbuff, sizeof(rbuff), 0)) > 0) {
					fwrite(rbuff, sizeof(char), cnt, fd);//把cnt个数据长度为char的数据从rbuff输入到fd指向的文件
				}
				cout << "成功获得文件" << fileName << endl;
				fclose(fd);
			}//put
			else if (strncmp(rbuff, "pwd", 3) == 0) {
				char path[1000];
				GetCurrentDirectory(sizeof(path), path);//找到当前进程的当前目录
				strcpy(sbuff, path);
				send(sockServer, sbuff, sizeof(sbuff), 0);
			}//pwd
			else if (strncmp(rbuff, "dir", 3) == 0) {
				strcpy(sbuff, rbuff);
				send(sockServer, sbuff, sizeof(sbuff), 0);
				sendFileList(sockServer);
			}//dir
			else if (strncmp(rbuff, "cd", 2) == 0) {
				strcpy(fileName, rbuff + 3);
				strcpy(sbuff, rbuff);
				send(sockServer, sbuff, sizeof(sbuff), 0);
				SetCurrentDirectory(fileName);//设置当前目录 
			}//cd
			else if (strncmp(rbuff, "user", 4) == 0) {
				char tbuff[1024];
				strcpy(tbuff, rbuff + 5);
				strcat(tbuff, " ");
				memset(rbuff, '\0', sizeof(rbuff));
				strcpy(sbuff, "成功获取用户名\0");
				send(sockServer, sbuff, sizeof(sbuff), 0);

				recv(sockServer, rbuff, sizeof(rbuff), 0);
				cout << endl << "获取并执行的命令：" << rbuff << endl;
				strcat(tbuff, rbuff + 5);
				if (strcmp(tbuff, namePassword) == 0) {//验证是否正确并返回数据给客户端
					send(sockServer, "right\0", sizeof(sbuff), 0);
				}
				else {
					send(sockServer, "wrong\0", sizeof(sbuff), 0);
				}
			}//user pass
			closesocket(sockServer);
		}
	}
}
int sendFile(SOCKET datatcps, FILE* file) {
	cout << "正在发送文件…" << endl;
	memset(sbuff, '\0', sizeof(sbuff));
	while (1) {//从文件中循环读取数据并发送至客户端
		int len = fread(sbuff, 1, sizeof(sbuff), file);//把file指针指向的文件中的内容读取到sbuff中
		if (send(datatcps, sbuff, len, 0) == SOCKET_ERROR) {
			cout << "连接失败" << endl;
			closesocket(datatcps);
			return 0;
		}
		if (len < sizeof(sbuff)) {//文件传送结束
			break;
		}
	}
	closesocket(datatcps);
	cout << "发送成功" << endl;
	return 1;
}
int sendFileList(SOCKET datatcps) {
	HANDLE hff;								//建立一个线程
	WIN32_FIND_DATA fd;						//搜索文件
	hff = FindFirstFile("*", &fd);			//查找文件来把待操作文件的相关属性读取到WIN32_FIND_DATA结构中去 
	if (hff == INVALID_HANDLE_VALUE) {		//发生错误
		const char* errStr = "列出文件列表时发生错误\n";
		cout << *errStr << endl;
		if (send(datatcps, errStr, strlen(errStr), 0) == SOCKET_ERROR) {
			cout << "发送失败" << endl;
		}
		closesocket(datatcps);
		return 0;
	}
	BOOL flag = TRUE;
	while (flag) {//发送文件信息
		if (!sendFileRecord(datatcps, &fd)) {
			closesocket(datatcps);
			return 0;
		}
		flag = FindNextFile(hff, &fd);//查找下一个文件
	}
	closesocket(datatcps);
	return 1;
}
int sendFileRecord(SOCKET datatcps, WIN32_FIND_DATA* pfd) {//发送当前的文件记录
	char fileRecord[MAX_PATH + 32];

	FILETIME ft;						//文件的建立时间
	FileTimeToLocalFileTime(&pfd->ftLastWriteTime, &ft);//Converts a file time to a local file time.

	SYSTEMTIME lastWriteTime;
	FileTimeToSystemTime(&ft, &lastWriteTime);

	const char* dir = pfd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? "<DIR>" : " ";
	sprintf(fileRecord, "%04d-%02d-%02d %02d:%02d %5s %10d   %-20s\n",
		lastWriteTime.wYear,
		lastWriteTime.wMonth,
		lastWriteTime.wDay,
		lastWriteTime.wHour,
		lastWriteTime.wMinute,
		dir,
		pfd->nFileSizeLow,
		pfd->cFileName
	);
	if (send(datatcps, fileRecord, strlen(fileRecord), 0) == SOCKET_ERROR) {
		//通过datatcps接口发送fileRecord数据，成功返回发送的字节数   
		cout << "发送失败" << endl;
		return 0;
	}
	return 1;
}
int main() {
	if (startSock() == -1 || createSocket() == -1 || connectProcess() == -1) {
		return -1;
	}
	return 1;
}
