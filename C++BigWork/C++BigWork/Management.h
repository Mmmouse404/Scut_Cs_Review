#pragma once
#include"Window.h"
#include<vector>
#include"Button.h"
#include"Person.h"
#include"Relative.h"
#include"Friend.h"
#include"Schoolmate.h"
#include"Workmate.h"
#include"Configure.h"

class Management
{
public:
	Management();
	enum Operator {
		show,
		add,
		erase,
		modify,
		search,
		sort,
		Exit,
		menu=666,
	}; //枚举类型，方便进行判断，直观
	enum Operator2 {
		show1,
		show2,
		show3,
		show4,
		showbirth,
		givebirth
	};//枚举类型，右侧按钮的对应数字

    bool Log();//登陆函数
	void Run();//启动管理类
	int Menu();//菜单
	void Show();//1.显示所有联系人信息
	void Add();//2.增加联系人
	void Erase();//3.删除联系人
	void Modify();//4.修改信息
	void Search();//5.查找信息
	void Sort();//6.排序
	void Sshow(int i);//7.按照分组显示联系人
	void MerryBirthday();//8.显示最近五天生日联系人并给出祝福

	void DrawBackground()//绘制背景
	{
		::putimage(0, 0, &bk);
	}
	void DrawDigitalClock(int year,int month,int day,int dayofweek,int hour, int minute, int second);//时间显示：
	//与Person类的连接：
	Person* man[200];
	int mannum=0;//联系人总人数
	void Insert( string Name, string Phone, string Email, string Addition,int Year, int Month, int Day ,int Type);//插入信息
	void ReadFile();//读文件到类对象里
	void SaveFile();//写文件保存
	bool Mystrcmp(const char a[], char b[]);//自定义的字符串比较
	bool Delete(char d[]);//删除含有指定字符串的用户
    void DeleteAll();//清空联系人
	void ShowBirthNnum();//显示生日在给定月份内的联系人
	void WriteWish(Person*m);//对指定的联系人写生日祝福
	int CalculateWeekDay(int y, int m, int d);//计算是星期几
	int DayDiff( int smonth, int sday,  int emonth, int eday);//计算两个日期（不考虑年份）的差距
	//信息统计
	int birthmonth[12];//分别存储每个月生日的人数
	int group[4];//分别存储分组内人数
	int flag = 1;//用于Menu内判断显示函数是否继续显示
	int page = 1; int pagenum=0;//用于翻页，当前页码和最大页码
	string Admin; string Password; string Adminname;//用户名（xx的通讯录），密码，用户文件夹的绝对路径
private:
	IMAGE bk;//背景图片变量
	ExMessage menu_msg;//菜单的信息，获取键盘鼠标的信息并判断
	std::vector<Button*>m_button;//菜单按钮1，在左侧
	std::vector<Button*>s_button;//菜单按钮2，在右侧
};

