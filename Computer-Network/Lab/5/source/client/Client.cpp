#include "Winsock.h"
#include "windows.h"
#include "time.h"
#include "stdio.h"
#include <iostream>
using namespace std;

#define RECV_PORT 3312	//���ն˿�
#define SEND_PORT 4302	//���Ͷ˿�
#pragma comment(lib, "wsock32.lib")	//����ws2_32.dll������Windows SocketsӦ�ó���ӿڣ� ����֧��Internet������Ӧ�ó���

SOCKET sockClient;		//�ͻ��˶���
sockaddr_in serverAddr;	//��������ַ
char inputIP[20];		//�洢����ķ�����IP
char fileName[20];		//�ļ���
char rbuff[1024];		//���ջ�����
char sbuff[1024];		//���ͻ�����
bool checkFlag = false;			//��־�Ƿ�ͨ����½

DWORD startSock();							//����winsock����ʼ��
DWORD createSocket();						//����socket
DWORD callServer();							//������������

void help();								//�˵�
void list(SOCKET sockfd);					//�г�Զ����ǰĿ¼
DWORD sendTCP(char data[]);					//����Ҫִ�е������������
int user();									//�ϴ��û���
int pass();									//�ϴ�����
int sendFile(SOCKET datatcps, FILE* file);	//put ���͸�Զ��һ���ļ�

DWORD startSock() { //����winsock����ʼ��
	WSADATA WSAData;
	char a[20];
	memset(a, 0, sizeof(a));
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) { //����winsock�汾
		cout << "sock��ʼ��ʧ��" << endl;
		return -1;
	}
	if (strncmp(inputIP, a, sizeof(a)) == 0) {
		cout << "������Ҫ���ӵķ�����IP��";
		cin >> inputIP;
	}
	//���õ�ַ�ṹ
	serverAddr.sin_family = AF_INET;					//�����ײ���ʹ�õ�����ͨ��Э�����ݽ����ݵģ�AF_INET��ʾʹ�� TCP/IPv4 ��ַ�����ͨ��
	serverAddr.sin_addr.s_addr = inet_addr(inputIP);	//ָ��������IP��ʮ����ת���ɶ�����IPV4��ַ
	serverAddr.sin_port = htons(RECV_PORT);				//���ö˿ںţ�htons���ڽ������ֽ����Ϊ�����ֽ���
	return 1;
}
DWORD createSocket() { //����socket
	//Ҫʹ���׽��֣����ȱ������socket()��������һ���׽���������������ͬ�����ļ�ʱ�����ȵõ���fopen()������һ���ļ���
	sockClient = socket(AF_INET, SOCK_STREAM, 0);//��scoket�����ɹ�����ʱ����һ���µ�SOCKET(Socket Descriptor) 
	//SOCK_STREAM����ʽ�׽��֣�:Tcp���ӣ��ṩ���л��ġ��ɿ��ġ�˫�����ӵ��ֽ�����֧�ִ������ݴ���
	if (sockClient == SOCKET_ERROR) {
		cout << "����socketʧ��" << endl;
		WSACleanup();//��ֹWs2_32.dll ��ʹ��
		return -1;
	}
	return 1;
}
DWORD callServer() { //������������
	createSocket();
	if (connect(sockClient, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {//connect()������ָ���ⲿ�˿ڵ�����
		cout << "����ʧ��" << endl;
		memset(inputIP, 0, sizeof(inputIP));
		return -1;
	}
	return 1;
}
void help() { //�����˵�
	cout << "        ___________________________________________  " << endl
		<< "       |                FTP�����˵�                |   " << endl
		<< "       | 1��get �����ļ� [�����ʽ: get �ļ��� ]   |   " << endl
		<< "       | 2��put �ϴ��ļ� [�����ʽ��put �ļ���]    |   " << endl
		<< "       | 3��pwd ��ʾ��ǰ�ļ��еľ���·��           |   " << endl
		<< "       | 4��dir ��ʾԶ����ǰĿ¼���ļ�             |   " << endl
		<< "       | 5��cd  �ı�Զ����ǰĿ¼��·��             |   " << endl
		<< "       |         �����¼�Ŀ¼: cd ·����           |   " << endl
		<< "       |         �����ϼ�Ŀ¼: cd ..               |   " << endl
		<< "       | 6��? ���� help ��������˵�               |   " << endl
		<< "       | 7��quit �˳�FTP                           |   " << endl
		<< "       |___________________________________________|    " << endl;
}
DWORD sendTCP(char data[]) { //����Ҫִ�е������������
	int length = send(sockClient, data, strlen(data), 0);
	if (length <= 0) {
		cout << "���������������ʧ��" << endl;
		closesocket(sockClient);//����ʹ��socket()�������׽���ʱ��Ӧ�õ���closesocket()���������رգ�����ͬ����fclose()�����ر�һ���ļ������������׽�����Դ���ͷš�
		WSACleanup();
		return -1;
	}
	return 1;
}
int sendFile(SOCKET datatcps, FILE* file) { //put ���͸�Զ��һ���ļ�
	cout << "���ڴ����ļ���" << endl;
	memset(sbuff, '\0', sizeof(sbuff));
	while (1) { //���ļ���ѭ����ȡ���ݲ�����
		int len = fread(sbuff, 1, sizeof(sbuff), file); //fread��file�ļ���ȡsizeof(sbuff)���ȵ����ݵ�sbuff�����سɹ���ȡ�����ݸ���
		if (send(datatcps, sbuff, len, 0) == SOCKET_ERROR) {
			cout << "��ͻ��˵������ж�" << endl;
			closesocket(datatcps);
			return 0;
		}
		if (len < sizeof(sbuff)) {
			break;
		}
	}
	closesocket(datatcps);
	cout << "�������" << endl;
	return 1;
}
void list(SOCKET sockfd) { //�г�Զ����ǰĿ¼
	int nRead;
	memset(sbuff, '\0', sizeof(sbuff));
	while (1) {
		nRead = recv(sockClient, rbuff, sizeof(rbuff), 0);
		//recvͨ��sockClient�׽ӿڽ������ݴ���rbuff�����������ؽ��յ����ֽ���
		if (nRead == SOCKET_ERROR) {
			cout << "��ȡʱ��������" << endl;
			exit(1);
		}
		if (nRead == 0) { //���ݶ�ȡ����
			break;
		}
		//��ʾ����
		rbuff[nRead] = '\0';
		cout << rbuff << endl;
	}
}
int  user() {
	char operation[10], name[20];		//�������ļ���
	char order[30] = "\0";				//���������
	char buff[80];						//�����洢�����ַ�����ʽ����order
	cout << "�������û���ָ�user �û�������";
	cin >> operation;
	cin >> name;
	strcat(order, operation), strcat(order, " "), strcat(order, name);
	sprintf(buff, order);
	sendTCP(buff);									//����ָ��
	recv(sockClient, rbuff, sizeof(rbuff), 0);		//������Ϣ 
	cout << rbuff << endl;
	return 1;
}
int pass() {
	char operation[10], name[20];		//�������ļ���
	char order[30] = "\0";				//���������
	char buff[80];						//�����洢�����ַ�����ʽ����order
	cout << "����������ָ�pass ���룩��";
	cin >> operation;
	cin >> name;
	strcat(order, operation), strcat(order, " "), strcat(order, name);
	sprintf(buff, order);
	sendTCP(buff);									//����ָ��
	recv(sockClient, rbuff, sizeof(rbuff), 0);		//������Ϣ 
	cout << rbuff << endl;
	if (strcmp(rbuff, "wrong") == 0) {
		return 0;
	}
	return 1;
}

int main() {
	while (1) {
		char operation[10], name[20];		//�������ļ���
		char order[30] = "\0";				//���������
		char buff[80];						//�����洢�����ַ�����ʽ����order
		FILE* fd1, * fd2;					//FileЭ����Ҫ���ڷ��ʱ��ؼ�����е��ļ���fdָ��ָ��Ҫ���ʵ�Ŀ���ļ� 
		int cnt;

		startSock();				//����winsock����ʼ��
		if (callServer() == -1) {	//������������ʧ��
			continue;
		}

		//������������ɹ�����ʼ������
		memset(operation, 0, sizeof(operation));
		memset(name, 0, sizeof(name));
		memset(order, 0, sizeof(order));
		memset(buff, 0, sizeof(buff));
		memset(rbuff, 0, sizeof(rbuff));
		memset(sbuff, 0, sizeof(sbuff));

		if (checkFlag == false) {//��½
			if (user() && pass()) {
				checkFlag = true;
				continue;
			}
			else {
				continue;
			}
		}

		cout << endl << "������Ҫִ�е�ָ��(���� ? �� help ���Դ򿪰����˵�) : ";
		cin >> operation;

		if (strncmp(operation, "get", 3) == 0 || strncmp(operation, "put", 3) == 0 || strncmp(operation, "cd", 2) == 0) { ///��Ҫ�����ļ����Ĺ���
			cin >> name;
		}
		else if (strncmp(operation, "quit", 4) == 0) { ///�˳�����
			cout << "�˳���" << endl;
			closesocket(sockClient);
			WSACleanup();
			break;
		}
		else if (strncmp(operation, "?", 1) == 0 || strncmp(operation, "help", 4) == 0) { ///�����˵�����
			help();
		}

		//��ָ�����Ͻ�order������Ž�buff
		strcat(order, operation), strcat(order, " "), strcat(order, name);
		sprintf(buff, order);
		sendTCP(buff);									//����ָ��
		recv(sockClient, rbuff, sizeof(rbuff), 0);		//������Ϣ 
		cout << rbuff << endl;							//pwd�����������Ѿ�ʵ��
		if (strncmp(rbuff, "get", 3) == 0) {			///���ع���
			fd1 = fopen(name, "wb");					//�ö����Ƶķ�ʽ���ļ���wb��ʾ�򿪻��½�һ���������ļ���ֻ����д���ݣ�  
			if (fd1 == NULL) {
				cout << "�򿪻����½� " << name << "�ļ�ʧ��" << endl;
				continue;
			}
			memset(rbuff, '\0', sizeof(rbuff));
			while ((cnt = recv(sockClient, rbuff, sizeof(rbuff), 0)) > 0) {
				fwrite(rbuff, sizeof(rbuff), cnt, fd1);	//C �⺯�� size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) �� ptr ��ָ��������е�����д�뵽������ stream �С�
			}
			fclose(fd1);								//�ر��ļ�
		}//get
		else if (strncmp(rbuff, "put", 3) == 0) { ///�ϴ�����
			strcpy(fileName, rbuff + 4);
			fd2 = fopen(fileName, "rb");				//��һ���������ļ����ļ�������ڣ�ֻ������
			if (fd2) { //�ɹ���
				if (!sendFile(sockClient, fd2)) {
					cout << "����ʧ��" << endl;
					return 0;
				}
				fclose(fd2);
			}
			else {
				strcpy(sbuff, "�޷����ļ�\n");
				if (send(sockClient, sbuff, sizeof(sbuff), 0)) {
					return 0;
				}
			}
		}//put
		else if (strncmp(rbuff, "dir", 3) == 0) { ///dir����
			list(sockClient);
		}//dir
		closesocket(sockClient);	//�ر�����
		WSACleanup();				//�ͷ�Winsock
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