#include "Winsock.h"
#include "windows.h"
#include "time.h"
#include "stdio.h"
#include <iostream>
using namespace std;

#define RECV_PORT 3312	//接收端口
#define SEND_PORT 4302	//发送端口
#pragma comment(lib, "wsock32.lib")	//加载ws2_32.dll，它是Windows Sockets应用程序接口， 用于支持Internet和网络应用程序。

SOCKET sockClient;		//客户端对象
sockaddr_in serverAddr;	//服务器地址
char inputIP[20];		//存储输入的服务器IP
char fileName[20];		//文件名
char rbuff[1024];		//接收缓冲区
char sbuff[1024];		//发送缓冲区
bool checkFlag = false;			//标志是否通过登陆

DWORD startSock();							//启动winsock并初始化
DWORD createSocket();						//创建socket
DWORD callServer();							//发送连接请求

void help();								//菜单
void list(SOCKET sockfd);					//列出远方当前目录
DWORD sendTCP(char data[]);					//发送要执行的命令至服务端
int user();									//上传用户名
int pass();									//上传密码
int sendFile(SOCKET datatcps, FILE* file);	//put 传送给远方一个文件

DWORD startSock() { //启动winsock并初始化
	WSADATA WSAData;
	char a[20];
	memset(a, 0, sizeof(a));
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) { //加载winsock版本
		cout << "sock初始化失败" << endl;
		return -1;
	}
	if (strncmp(inputIP, a, sizeof(a)) == 0) {
		cout << "请输入要连接的服务器IP：";
		cin >> inputIP;
	}
	//设置地址结构
	serverAddr.sin_family = AF_INET;					//表明底层是使用的哪种通信协议来递交数据的，AF_INET表示使用 TCP/IPv4 地址族进行通信
	serverAddr.sin_addr.s_addr = inet_addr(inputIP);	//指定服务器IP，十进制转化成二进制IPV4地址
	serverAddr.sin_port = htons(RECV_PORT);				//设置端口号，htons用于将主机字节序改为网络字节序
	return 1;
}
DWORD createSocket() { //创建socket
	//要使用套接字，首先必须调用socket()函数创建一个套接字描述符，就如同操作文件时，首先得调用fopen()函数打开一个文件。
	sockClient = socket(AF_INET, SOCK_STREAM, 0);//当scoket函数成功调用时返回一个新的SOCKET(Socket Descriptor) 
	//SOCK_STREAM（流式套接字）:Tcp连接，提供序列化的、可靠的、双向连接的字节流。支持带外数据传输
	if (sockClient == SOCKET_ERROR) {
		cout << "创建socket失败" << endl;
		WSACleanup();//终止Ws2_32.dll 的使用
		return -1;
	}
	return 1;
}
DWORD callServer() { //发送连接请求
	createSocket();
	if (connect(sockClient, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {//connect()创建与指定外部端口的连接
		cout << "连接失败" << endl;
		memset(inputIP, 0, sizeof(inputIP));
		return -1;
	}
	return 1;
}
void help() { //帮助菜单
	cout << "        ___________________________________________  " << endl
		<< "       |                FTP帮助菜单                |   " << endl
		<< "       | 1、get 下载文件 [输入格式: get 文件名 ]   |   " << endl
		<< "       | 2、put 上传文件 [输入格式：put 文件名]    |   " << endl
		<< "       | 3、pwd 显示当前文件夹的绝对路径           |   " << endl
		<< "       | 4、dir 显示远方当前目录的文件             |   " << endl
		<< "       | 5、cd  改变远方当前目录和路径             |   " << endl
		<< "       |         进入下级目录: cd 路径名           |   " << endl
		<< "       |         进入上级目录: cd ..               |   " << endl
		<< "       | 6、? 或者 help 进入帮助菜单               |   " << endl
		<< "       | 7、quit 退出FTP                           |   " << endl
		<< "       |___________________________________________|    " << endl;
}
DWORD sendTCP(char data[]) { //发送要执行的命令至服务端
	int length = send(sockClient, data, strlen(data), 0);
	if (length <= 0) {
		cout << "发送命令至服务端失败" << endl;
		closesocket(sockClient);//当不使用socket()创建的套接字时，应该调用closesocket()函数将它关闭，就如同调用fclose()函数关闭一个文件，用来进行套接字资源的释放。
		WSACleanup();
		return -1;
	}
	return 1;
}
int sendFile(SOCKET datatcps, FILE* file) { //put 传送给远方一个文件
	cout << "正在传输文件…" << endl;
	memset(sbuff, '\0', sizeof(sbuff));
	while (1) { //从文件中循环读取数据并发送
		int len = fread(sbuff, 1, sizeof(sbuff), file); //fread从file文件读取sizeof(sbuff)长度的数据到sbuff，返回成功读取的数据个数
		if (send(datatcps, sbuff, len, 0) == SOCKET_ERROR) {
			cout << "与客户端的连接中断" << endl;
			closesocket(datatcps);
			return 0;
		}
		if (len < sizeof(sbuff)) {
			break;
		}
	}
	closesocket(datatcps);
	cout << "传输完成" << endl;
	return 1;
}
void list(SOCKET sockfd) { //列出远方当前目录
	int nRead;
	memset(sbuff, '\0', sizeof(sbuff));
	while (1) {
		nRead = recv(sockClient, rbuff, sizeof(rbuff), 0);
		//recv通过sockClient套接口接受数据存入rbuff缓冲区，返回接收到的字节数
		if (nRead == SOCKET_ERROR) {
			cout << "读取时发生错误" << endl;
			exit(1);
		}
		if (nRead == 0) { //数据读取结束
			break;
		}
		//显示数据
		rbuff[nRead] = '\0';
		cout << rbuff << endl;
	}
}
int  user() {
	char operation[10], name[20];		//操作与文件名
	char order[30] = "\0";				//输入的命令
	char buff[80];						//用来存储经过字符串格式化的order
	cout << "请输入用户名指令（user 用户名）：";
	cin >> operation;
	cin >> name;
	strcat(order, operation), strcat(order, " "), strcat(order, name);
	sprintf(buff, order);
	sendTCP(buff);									//发送指令
	recv(sockClient, rbuff, sizeof(rbuff), 0);		//接收信息 
	cout << rbuff << endl;
	return 1;
}
int pass() {
	char operation[10], name[20];		//操作与文件名
	char order[30] = "\0";				//输入的命令
	char buff[80];						//用来存储经过字符串格式化的order
	cout << "请输入密码指令（pass 密码）：";
	cin >> operation;
	cin >> name;
	strcat(order, operation), strcat(order, " "), strcat(order, name);
	sprintf(buff, order);
	sendTCP(buff);									//发送指令
	recv(sockClient, rbuff, sizeof(rbuff), 0);		//接收信息 
	cout << rbuff << endl;
	if (strcmp(rbuff, "wrong") == 0) {
		return 0;
	}
	return 1;
}

int main() {
	while (1) {
		char operation[10], name[20];		//操作与文件名
		char order[30] = "\0";				//输入的命令
		char buff[80];						//用来存储经过字符串格式化的order
		FILE* fd1, * fd2;					//File协议主要用于访问本地计算机中的文件，fd指针指向要访问的目标文件 
		int cnt;

		startSock();				//启动winsock并初始化
		if (callServer() == -1) {	//发送连接请求失败
			continue;
		}

		//发送连接请求成功，初始化数据
		memset(operation, 0, sizeof(operation));
		memset(name, 0, sizeof(name));
		memset(order, 0, sizeof(order));
		memset(buff, 0, sizeof(buff));
		memset(rbuff, 0, sizeof(rbuff));
		memset(sbuff, 0, sizeof(sbuff));

		if (checkFlag == false) {//登陆
			if (user() && pass()) {
				checkFlag = true;
				continue;
			}
			else {
				continue;
			}
		}

		cout << endl << "请输入要执行的指令(输入 ? 或 help 可以打开帮助菜单) : ";
		cin >> operation;

		if (strncmp(operation, "get", 3) == 0 || strncmp(operation, "put", 3) == 0 || strncmp(operation, "cd", 2) == 0) { ///需要输入文件名的功能
			cin >> name;
		}
		else if (strncmp(operation, "quit", 4) == 0) { ///退出功能
			cout << "退出！" << endl;
			closesocket(sockClient);
			WSACleanup();
			break;
		}
		else if (strncmp(operation, "?", 1) == 0 || strncmp(operation, "help", 4) == 0) { ///帮助菜单功能
			help();
		}

		//将指令整合进order，并存放进buff
		strcat(order, operation), strcat(order, " "), strcat(order, name);
		sprintf(buff, order);
		sendTCP(buff);									//发送指令
		recv(sockClient, rbuff, sizeof(rbuff), 0);		//接收信息 
		cout << rbuff << endl;							//pwd功能在这里已经实现
		if (strncmp(rbuff, "get", 3) == 0) {			///下载功能
			fd1 = fopen(name, "wb");					//用二进制的方式打开文件，wb表示打开或新建一个二进制文件（只允许写数据）  
			if (fd1 == NULL) {
				cout << "打开或者新建 " << name << "文件失败" << endl;
				continue;
			}
			memset(rbuff, '\0', sizeof(rbuff));
			while ((cnt = recv(sockClient, rbuff, sizeof(rbuff), 0)) > 0) {
				fwrite(rbuff, sizeof(rbuff), cnt, fd1);	//C 库函数 size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) 把 ptr 所指向的数组中的数据写入到给定流 stream 中。
			}
			fclose(fd1);								//关闭文件
		}//get
		else if (strncmp(rbuff, "put", 3) == 0) { ///上传功能
			strcpy(fileName, rbuff + 4);
			fd2 = fopen(fileName, "rb");				//打开一个二进制文件，文件必须存在，只允许读
			if (fd2) { //成功打开
				if (!sendFile(sockClient, fd2)) {
					cout << "发送失败" << endl;
					return 0;
				}
				fclose(fd2);
			}
			else {
				strcpy(sbuff, "无法打开文件\n");
				if (send(sockClient, sbuff, sizeof(sbuff), 0)) {
					return 0;
				}
			}
		}//put
		else if (strncmp(rbuff, "dir", 3) == 0) { ///dir功能
			list(sockClient);
		}//dir
		closesocket(sockClient);	//关闭连接
		WSACleanup();				//释放Winsock
	}
	return 0;
}
/*
192.168.0.100
user gyc
pass 123456
pwd
cd Debug
get 110.txt
*/
