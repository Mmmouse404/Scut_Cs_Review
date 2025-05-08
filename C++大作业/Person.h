#pragma once
#include<iostream>

using namespace std;
class Person
{
public:
	enum Type {
		Type_Schoolmate = 1,
		Type_Workmate = 2,
		Type_Friend = 3,
		Type_Relative = 4
	};
	Person(string n, string t, string e, int y, int m, int d, int ty);//���캯��
	Person() {};//Ĭ�Ϲ��캯��
	//�������������������ڣ����ڣ�    �������绰���룬���䣬��ϵ���
	string GetName()const;//��ȡ����
	string GetTel()const;//��ȡ�绰
	string GetEmail()const;//��ȡ����
	virtual string GetAddition()const = 0;//��ȡ��ע�����麯��ʵ�ֶ�̬
	int GetYear()const;
	int GetMonth()const;
	int GetDay()const;//��ȡ��������
	string GetType()const;//��ȡ��ϵ��string��ʽ��
	int GetTypeNum()const;//��ȡ��ϵ��int��ʽ��
	void SetName(char s[]);//��������
	void SetTel(char s[]);//���õ绰
	void SetEmail(char s[]);//��������
	void SetType(int s);//���÷���
	virtual void SetAddtion(char s[]) = 0;//���ñ�ע�����麯��ʵ�ֶ�̬
	void SetBirthDay(int a, int b, int c);//��������
	int GetBirthTime();//��ȡ�������ڣ�ת��Ϊ�������������ھ�������
private:
	string name;//����
	int year; int month; int day;//����
	int type;//��ϵ��1ͬѧ2ͬ��3����4���ݣ�
	string tel;//�绰
	string email;//����
};

