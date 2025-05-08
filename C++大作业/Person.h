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
	Person(string n, string t, string e, int y, int m, int d, int ty);//构造函数
	Person() {};//默认构造函数
	//常量：姓名、出生日期（星期）    变量：电话号码，邮箱，关系类别
	string GetName()const;//获取名字
	string GetTel()const;//获取电话
	string GetEmail()const;//获取邮箱
	virtual string GetAddition()const = 0;//获取备注，纯虚函数实现多态
	int GetYear()const;
	int GetMonth()const;
	int GetDay()const;//获取生日日期
	string GetType()const;//获取关系（string形式）
	int GetTypeNum()const;//获取关系（int形式）
	void SetName(char s[]);//设置名字
	void SetTel(char s[]);//设置电话
	void SetEmail(char s[]);//设置邮箱
	void SetType(int s);//设置分组
	virtual void SetAddtion(char s[]) = 0;//设置备注，纯虚函数实现多态
	void SetBirthDay(int a, int b, int c);//设置生日
	int GetBirthTime();//获取生日日期，转化为总日期用于日期距离运算
private:
	string name;//名字
	int year; int month; int day;//生日
	int type;//关系（1同学2同事3朋友4亲戚）
	string tel;//电话
	string email;//邮箱
};

