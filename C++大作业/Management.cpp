#include "Management.h"
#include<iostream>
#include<fstream>
#include<conio.h>
#include<locale>
#include<string>
#include<graphics.h>
#include<ctime>
#include<filesystem>
SYSTEMTIME t;
using namespace std;
Management::Management()
{
	//加载背景图片
	::loadimage(&bk, "D:/新学习/个人通讯录管理系统/image/bk.jpg", Window::Width(), Window::Height());
	//界面按钮初始化
	m_button.push_back(new Button("显示所有"));
	m_button.push_back(new Button("增加"));
	m_button.push_back(new Button("删除"));
	m_button.push_back(new Button("修改"));
	m_button.push_back(new Button("查找"));
	m_button.push_back(new Button("排序"));
	m_button.push_back(new Button("退出"));
	s_button.push_back(new Button("显示同学分组"));
	s_button.push_back(new Button("显示同事分组"));
	s_button.push_back(new Button("显示朋友分组"));
	s_button.push_back(new Button("显示亲戚分组"));
	s_button.push_back(new Button("显示给定月份出生人数"));
	s_button.push_back(new Button("给五天内生日联系人祝福"));
	for (int i = 0; i < m_button.size(); i++) 
	{
		m_button[i]->SetSize(250, 50);
		int bx = (Window::Width() - m_button[i]->Getw()-700) / 2;
		int by = (Window::Height() - m_button.size()*m_button[i]->Geth()) / 2 + i * m_button[i]->Geth();
		m_button[i]->Move(bx, by);
	}
	for (int i = 0; i < s_button.size(); i++)
	{
		s_button[i]->SetSize(250, 50);
		int bx = (Window::Width() - s_button[i]->Getw()+700) / 2;
		int by = (Window::Height() - s_button.size() * s_button[i]->Geth()) / 2 + i * s_button[i]->Geth();
		s_button[i]->Move(bx, by);
	}//调整按钮位置
	mannum = 0;
}

bool Management::Log()
{
	char Adname[20] = { 0 }; char choice[2] = { 0 }; char Adcode[20] = { 0 };
	if (InputBox(choice, 2, "输入1/2/3选择：登录/注册/修改密码(输入其他为退出)", NULL, NULL, 0, 0, false) == false)
	{
		exit(0);
	}
	switch (choice[0] - '0')
	{
	case 1: {
		if (InputBox(Adname, 20, "请输入用户名", NULL, NULL, 0, 0, false) == false)
		{
			return false;
		}
		Admin = string(Adname) + "的通讯录"; string PasswordFile = "D:/新学习/个人通讯录管理系统/" + Admin + "/密码.txt"; string AdminFile = "D:/新学习/个人通讯录管理系统/" + Admin;
		if (!filesystem::is_directory(AdminFile))
		{
			MessageBox(NULL, "用户名不存在！", "False", MB_SYSTEMMODAL);
		}
		else {
			ifstream file; file.open(PasswordFile, ios::in);
			if (!file.is_open())
			{
				MessageBox(NULL, "无法打开密码文件！", "False", MB_SYSTEMMODAL); return false;
			}
			else file >> Password;
			InputBox(Adcode, 20, "请输入密码", NULL, NULL, 0, 0, false);
			if (Mystrcmp(Password.c_str(), Adcode)) {
				MessageBox(NULL, "登陆成功！", "OK！", MB_SYSTEMMODAL); Adminname = Adname;
				return true;
			}
			else {
				MessageBox(NULL, "密码与用户名不匹配！", "False！", MB_SYSTEMMODAL); 
				return false;
			}
			break;
		}
	}
	case 2: {
		if (InputBox(Adname, 20, "请输入用户名", NULL, NULL, 0, 0, false) == false)
		{
			return false;
		}
		else
		{
			Admin = string(Adname) + "的通讯录"; ofstream file; string PasswordFile = "D:/新学习/个人通讯录管理系统/" + Admin + "/密码.txt"; string AdminFile = "D:/新学习/个人通讯录管理系统/" + Admin;
			if (filesystem::is_directory(AdminFile)) {
				MessageBox(NULL, "用户已被注册！", "False", MB_SYSTEMMODAL);
				return false;
			}
			else {
				InputBox(Adcode, 20, "请输入密码(请勿输入中文)", NULL, NULL, 0, 0, false);
				if (strlen(Adcode) == 0 || strlen(Adname) == 0)
				{
					MessageBox(NULL, "错误的空输入", "False", MB_SYSTEMMODAL);//弹出一个窗口提醒用户
					return false;
				}
				else;
				{
					string AdminFile = "D:/新学习/个人通讯录管理系统/" + Admin;
						if(!filesystem::is_directory(AdminFile))filesystem::create_directory(AdminFile);
						file.open(PasswordFile, ios::out | ios::ate);//指定文件夹里清空并重新输入
						file << string(Adcode) << endl;
						file.close();
						string book1 = "D:/新学习/个人通讯录管理系统/" + Admin + "/AddressBook1.txt"; file.open(book1, ios::out); file << 0 << endl; file.close();
						string book2 = "D:/新学习/个人通讯录管理系统/" + Admin + "/AddressBook2.txt"; file.open(book2, ios::out); file << 0 << endl; file.close();
						string book3 = "D:/新学习/个人通讯录管理系统/" + Admin + "/AddressBook3.txt"; file.open(book3, ios::out); file << 0 << endl; file.close();
						string book4 = "D:/新学习/个人通讯录管理系统/" + Admin + "/AddressBook4.txt"; file.open(book4, ios::out); file << 0 << endl; file.close();
						MessageBox(NULL, "注册成功", "OK!", MB_SYSTEMMODAL);
						return false;	
				}
			}
		}
		break;
	}
	case 3: {
		if (InputBox(Adname, 20, "请输入用户名", NULL, NULL, 0, 0, false) == false)
		{
			return false;
		}
		else
		{
			Admin = string(Adname) + "的通讯录"; fstream file; string PasswordFile = "D:/新学习/个人通讯录管理系统/"  + Admin + "/密码.txt"; char Recode[20] = { 1 }; char Incode[20] = {0}; 
			string AdminFile = "D:/新学习/个人通讯录管理系统/" + Admin;
			file.open(PasswordFile, ios::in); file >> Password; file.close();
			if (filesystem::is_directory(AdminFile)) 
			{
				if (InputBox(Adcode, 20, "请输入原来的密码(请勿输入中文)", NULL, NULL, 0, 0, false)) {
					if (Mystrcmp(Password.c_str(), Adcode))
					{
						InputBox(Recode, 20, "请输入新的密码(请勿输入中文)", NULL, NULL, 0, 0, false);
						InputBox(Incode, 20, "请再次确认密码(请勿输入中文)", NULL, NULL, 0, 0, false);

						if (Mystrcmp(Incode, Recode))
						{
							file.open(PasswordFile, ios::out | ios::ate);//指定文件夹里清空并重新输入
							file << string(Recode) << endl;
							file.close();
							MessageBox(NULL, "修改成功！", "OK", MB_SYSTEMMODAL);
						}
						else { MessageBox(NULL, "两次密码不一致！", "OK", MB_SYSTEMMODAL); }
					}
					else { MessageBox(NULL, "密码错误！", "False!", MB_SYSTEMMODAL); }
				}
			}
			else
			{
				MessageBox(NULL, "用户名不存在！", "False", MB_SYSTEMMODAL); 
			} 
		}
		return false;
		break;
	}
	default:
		exit(0);
	}
}

void Management::Run()//运行管理系统
{
	int op = menu;//循环开始，保证一开始用（菜单）功能，显示菜单
	Window::BeginDraw();//开始绘制图形，不显示
	ReadFile();//初始化进行文件读取
	while (1) 
	{
		Window::Clear();//清屏
		DrawBackground();//绘制背景图片
		GetLocalTime(&t); //获取当前时间 
		DrawDigitalClock(t.wYear,t.wMonth,t.wDay,t.wDayOfWeek,t.wHour, t.wMinute, t.wSecond);//显示计时器
		if (Window::HasMsg())
		{
			menu_msg = Window::GetMsg();
		}//获取鼠标键盘信息，实现窗口交互
		if (menu_msg.vkcode == VK_ESCAPE) {
			op = menu;
		}//获取键盘ESC信息退出
		switch (op)//根据按钮返回的信息进行功能实现
		{
		case menu: 
		{
			page = 1; flag = 1;
			op = Menu();//Menu接受按钮信息
			break; 
		}
		case show: 
		{
			Show();//显示所有联系人
			break; 
		}
		case add: 
		{
			Add();//增加联系人
			SaveFile();//保存文件
			op = menu;//实现后回到菜单，后面同理
			break; 
		}
		case erase: 
		{
			Erase();//删除联系人
			SaveFile();
			op = menu;
			break;
		}
		case modify:
		{
			Modify();//修改联系人
			SaveFile();
			op = menu;
			break; 
		}
		case search: 
		{
			Search();//查找联系人
			op = menu;
			break; 
		}
		case sort:
		{
			Sort();//重排联系人
			op = menu;
			break;
		}
		case show1+7:
		{
			Sshow(1);//显示同学分组联系人,显示函数不能只显示一次就返回，所以没有op=menu
			break;
		}
		case show2+7:
		{
			Sshow(2);//显示同事分组联系人
			break;
		}
		case show3+7:
		{
			Sshow(3);//显示朋友分组联系人
			break;
		}
		case show4+7:
		{
			Sshow(4);//显示亲戚分组联系人
			break;
		}
		case showbirth+7:
		{
			ShowBirthNnum();//指定月份，显示该月份出生的联系人
			op = menu;
			break;
		}
		case givebirth + 7:
		{
			MerryBirthday();//给最近五天内生日的联系人祝福
			break;
		}
		case Exit:
			exit(0); break;//退出系统
		default:
			break;
		}
		Window::FlushDraw();//上述Switch实现后，开始绘制（绘制菜单的背景）
	}
	Window::EndDraw();//结束绘制
}

int Management::Menu()
{
	settextstyle(30, 0, "黑体");//设置标题字体
	::outtextxy(430,100, Admin.c_str());//显示标题
	settextstyle(18, 0, "等线");//设置其他字体
	int i = 0;
	for(i=0;i<m_button.size();i++)
	{
		m_button[i]->Loop(menu_msg);//事件循环，不断获取menu的信息（来自键盘鼠标）
		m_button[i]->Show();//显示按钮
		if (m_button[i]->IsClicked()) {
			return i;//如果按钮被点击，则返回相应的按钮值，实现不同功能
		}
	}
	for (i = 0; i < s_button.size(); i++)
	{
		s_button[i]->Loop(menu_msg);
		s_button[i]->Show();
		if (s_button[i]->IsClicked()) {
			return i+7;//同上，因为只用了一个switch所以要略去
		}
	}
	return menu;
}

void Management::Show()//显示全部联系人
{
	if (flag == 0)return;
	IMAGE Showphoto;  pagenum = mannum / 15 + 1; string spage = "第" + to_string(page) + "页";//string保存页码
	loadimage(&Showphoto, "D:/新学习/个人通讯录管理系统/image/Showphoto.jpg", Window::Width(), Window::Height());//Showphoto作为载体加载新的背景图片
	putimage(0, 0, &Showphoto);//绘制图片在窗口上
	settextcolor(BLACK);//字体颜色设置为黑色
	setbkmode(TRANSPARENT);//背景设置为渐变
	outtextxy(8, 8, "|姓名|      |分组|      |备注|               |电话|              |邮箱|                          |生日|   ");//显示表头
	outtextxy(750, 400, "按下ESC键回到主菜单");
	outtextxy(750, 440, "按下Space键翻页");
	outtextxy(430, 600, spage.c_str());//显示页码
	if (Window::HasMsg())
	{
		menu_msg= Window::GetMsg();
		if (menu_msg.message == WM_KEYDOWN) {
			if (menu_msg.vkcode == VK_SPACE) {
				if (page < pagenum) page++;//如果按下SPACE可以页码增加
				else page = 1;//页码达到上限回到第一页
			}
			if (menu_msg.vkcode == VK_ESCAPE) {
				flag = 0;//按下ESC后表示不在输出，直接return
			}
		}
	}
	int i = 10, j = 50; 
	for (int t = 0 + (page - 1) * 15; (t < 15 * page) && (t < mannum)&&flag; t++)
	{
		string birthday = to_string(man[t]->GetYear()) + "." + to_string(man[t]->GetMonth()) + "." + to_string(man[t]->GetDay());//将生日连起来
		outtextxy(i, j, man[t]->GetName().c_str());//显示姓名
		outtextxy(i + 70, j, man[t]->GetType().c_str());//显示分组
		outtextxy(i + 150, j, man[t]->GetAddition().c_str());//显示备注
		outtextxy(i + 260, j, man[t]->GetTel().c_str());//显示电话
		outtextxy(i + 370, j, man[t]->GetEmail().c_str());//显示邮箱
		outtextxy(i + 540, j, birthday.c_str());//显示生日
		j = j + 30;
	}
	DrawDigitalClock(t.wYear,t.wMonth,t.wDay,t.wDayOfWeek,t.wHour, t.wMinute, t.wSecond);//显示时钟
}

void Management::Add()
{
	DrawDigitalClock(t.wYear,t.wMonth,t.wDay,t.wDayOfWeek,t.wHour, t.wMinute, t.wSecond);
	char Rname[20] = { 0 }; char addition[20] = {0}; char Rtel[12] = { 0 }; char Remail[30] = { 0 };
	char cRyear[5]; char cRmonth[3]; char cRday[3]; char cRtype[2];
	InputBox(Rname, 20, "请输入联系人姓名", NULL, NULL, 0, 0, false);
	if (Rname[0] != 0) {
		InputBox(Rtel, 12, "请输入联系人电话", NULL, NULL, 0, 0, true);
		InputBox(Remail, 30, "请输入联系人邮箱", NULL, NULL, 0, 0, true);
		InputBox(cRyear, 5, "请输入联系人出生年份", NULL, NULL, 0, 0, true);
		InputBox(cRmonth, 3, "请输入联系人出生月份", NULL, NULL, 0, 0, true);
		InputBox(cRday, 3, "请输入联系人出生日期", NULL, NULL, 0, 0, true);
	}
	//1.输出空
	if (strlen(Rname) == 0 || strlen(Rtel) == 0 || strlen(Remail) == 0 || strlen(cRyear) == 0 || strlen(cRmonth) == 0 || strlen(cRday) == 0)
	{
		MessageBox(NULL, "错误的空输入", "False", MB_SYSTEMMODAL);//弹出一个窗口提醒用户
		return;                                          
	}
	//2.输入太长
	if (strlen(Rname) > 20 || strlen(Rtel) >20 || strlen(Remail) > 30 || strlen(cRyear) > 4 || strlen(cRmonth) > 2 || strlen(cRday) >2)
	{
		MessageBox(NULL, "输入的字符超出范围！", "False", MB_SYSTEMMODAL);
		return ;
	}
	int Ryear = atoi(cRyear); int Rmonth = atoi(cRmonth); int Rday = atoi(cRday); 
	//3.日期限制
	if ( Ryear-'0' >2023 || Rmonth-'0' > 12 || Rday-'0' > 31)
	{
		MessageBox(NULL, "输入日期超出范围！", "False", MB_SYSTEMMODAL);
		return;
	}
	//4.超出联系人最大人数
	if (mannum>200)
	{
		MessageBox(NULL, "联系人过多！", "False", MB_SYSTEMMODAL);//弹出一个窗口提醒用户
		return;
	}
	InputBox(cRtype, 3, "请输入联系人分组 (1:同学 2:同事 3:朋友 4:亲戚)", NULL, NULL, 0, 0, true); int Rtype = atoi(cRtype);
	switch (Rtype) 
	{
	case 1: {
		InputBox(addition, 20, "请输入联系人学校", NULL, NULL, 0, 0, true); 
		Insert(Rname, Rtel, Remail, addition, Ryear, Rmonth, Rday, Rtype);
		break;
	}
	case 2: {
		InputBox(addition, 20, "请输入联系人单位", NULL, NULL, 0, 0, true); 
		Insert(Rname, Rtel, Remail, addition, Ryear, Rmonth, Rday, Rtype);
		break;
	}
	case 3: {
		InputBox(addition, 20, "请输入联系人认识地点", NULL, NULL, 0, 0, true); 
		Insert(Rname, Rtel, Remail, addition, Ryear, Rmonth, Rday, Rtype);
		break;
	}
	case 4: {
		InputBox(addition, 20, "请输入联系人称呼", NULL, NULL, 0, 0, true); 
		Insert(Rname, Rtel, Remail, addition, Ryear, Rmonth, Rday, Rtype);
		break;
	}
	default:
		MessageBox(NULL, "输入错误！", "False", MB_SYSTEMMODAL);
		return; 
		break;
	}
	cout << man[mannum - 1]->GetName() << man[mannum - 1]->GetTel() << man[mannum - 1]->GetEmail() << endl;
	MessageBox(NULL, "添加成功！", "OK", MB_SYSTEMMODAL);
}

void Management::Erase()//删除功能实现
{
	DrawDigitalClock(t.wYear,t.wMonth,t.wDay,t.wDayOfWeek,t.wHour, t.wMinute, t.wSecond);
	char Dname[20] = { 0 }; char Dchoice[20] = {0};
	InputBox(Dchoice, 2, "输入1/2选择：清空所有联系人信息/删除单个联系人",NULL,NULL,0,0,false);
	if (Dchoice[0] - '0' == 1) {
		DeleteAll();
		MessageBox(NULL, "删除成功！", "OK", MB_SYSTEMMODAL);
	}
	if (Dchoice[0] - '0' == 2) {
		InputBox(Dname, 20, "请输入要删除的名字", NULL, NULL, 0, 0, false);
		if (Delete(Dname)) {
			MessageBox(NULL, "删除成功！", "OK", MB_SYSTEMMODAL);
		}
		else { MessageBox(NULL, "删除失败！", "错误！", MB_SYSTEMMODAL); }
	}
	SaveFile();
}

void Management::Modify()
{
	DrawDigitalClock(t.wYear,t.wMonth,t.wDay,t.wDayOfWeek,t.wHour, t.wMinute, t.wSecond);
	char Sname[20] = { 0 };  char Rtel[12] = { 0 }; char Remail[30] = { 0 }; char cRtype[2]; char addition[20]; char mstr[20] = { 0 }; int flag = 0;//预输入的字符串，置空便于取消后返回菜单
	InputBox(Sname, 20, "请输入要修改的联系人姓名/电话", NULL, NULL, 0, 0, false);
	if (mannum)
	{
		for (int t = 0; t < mannum; t++)
		{
			if (Mystrcmp(man[t]->GetName().c_str(), Sname) || Mystrcmp(man[t]->GetTel().c_str(), Sname))
			{
				string Inf = man[t]->GetName() + " 电话：" + man[t]->GetTel() + " 邮箱：" + man[t]->GetEmail() + " 备注：" + man[t]->GetAddition() 
					+ "\n生日：" + to_string(man[t]->GetYear()) + "." + to_string(man[t]->GetMonth()) + "." + to_string(man[t]->GetDay());
				if (InputBox(mstr, 20, Inf.c_str(), "请确认是否修改该联系人(点取消或确定)", NULL, 0, 0, false)) 
				{
					InputBox(Rtel, 12, "请修改联系人电话", NULL, NULL, 0, 0, true);
					InputBox(Remail, 30, "请修改联系人邮箱", NULL, NULL, 0, 0, true);
					InputBox(cRtype, 3, "请修改联系人分组 (1:同学 2:同事 3:朋友 4:亲戚)", NULL, NULL, 0, 0, true); int Rtype = atoi(cRtype);
					//1.输出空
					if (strlen(Rtel) == 0 || strlen(Remail) == 0){
						MessageBox(NULL, "错误的空输入", "False", MB_SYSTEMMODAL);//弹出一个窗口提醒用户
						t--; continue;
					}
					//2.输入太长
					if (strlen(Rtel) > 20 || strlen(Remail) > 30){
						MessageBox(NULL, "输入的字符超出范围！", "False", MB_SYSTEMMODAL);
						t--; continue;
					}
					man[t]->SetTel(Rtel); man[t]->SetEmail(Remail);
					switch (Rtype)
					{
					case 1: {
									InputBox(addition, 20, "请输入联系人学校", NULL, NULL, 0, 0, true);
									man[t]->SetType(Rtype); man[t]->SetAddtion(addition); break;
							}
					case 2: {
									InputBox(addition, 20, "请输入联系人单位", NULL, NULL, 0, 0, true);
									man[t]->SetType(Rtype); man[t]->SetAddtion(addition); break;
							}
					case 3: {
									InputBox(addition, 20, "请输入联系人认识地点", NULL, NULL, 0, 0, true);
									man[t]->SetType(Rtype); man[t]->SetAddtion(addition); break;
							}
					case 4: {
									InputBox(addition, 20, "请输入联系人称呼", NULL, NULL, 0, 0, true);
									man[t]->SetType(Rtype); man[t]->SetAddtion(addition); break;
							}
					}
					MessageBox(NULL, "修改成功！", "OK", MB_SYSTEMMODAL);flag = 1;
				}
			}
		}
		if (flag == 0)MessageBox(NULL, "修改失败！", "错误！", MB_SYSTEMMODAL); return;
	}
	else { MessageBox(NULL, "无联系人", "错误！", MB_SYSTEMMODAL); }
}

void Management::Search()//查找联系人主要函数
{
	DrawDigitalClock(t.wYear,t.wMonth,t.wDay,t.wDayOfWeek,t.wHour, t.wMinute, t.wSecond);
	::outtextxy(0, 600, "按下ESC键退出");
	char Sname[20] = {0};
	InputBox(Sname, 20, "请输入要查找的联系人姓名/电话（电话不会重名）", NULL, NULL, 0, 0, false);//InputBox让用户填空
	int t; flag = 0;
	if (mannum)//联系人数量不为0
	{
		for (t = 0; t < mannum; t++)
		{
			if (Mystrcmp(man[t]->GetName().c_str(), Sname) || Mystrcmp(man[t]->GetTel().c_str(), Sname))//字符串比较不能用==，这里使用Mystrcmp函数
			{
				MessageBox(NULL, "查找成功！", "OK", MB_SYSTEMMODAL);
				string Inf = man[t]->GetName() + " 电话：" + man[t]->GetTel() + " 邮箱：" + man[t]->GetEmail() + " 备注：" + man[t]->GetAddition()
					+ "\n生日：" + to_string(man[t]->GetYear()) + "." + to_string(man[t]->GetMonth()) + "." + to_string(man[t]->GetDay());
				MessageBox(NULL, Inf.c_str(), "联系人信息：", MB_SYSTEMMODAL); flag = 1;
			}
		}
	  if(flag==0)MessageBox(NULL, "无此联系人", "错误！", MB_SYSTEMMODAL); 
	}
	else {MessageBox(NULL, "无联系人", "错误！", MB_SYSTEMMODAL);}
}

void Management::DrawDigitalClock(int year,int month,int day,int dayofweek,int hour, int minute, int second)
{
	string str; string week[7] = { "星期日","星期一","星期二","星期三","星期四","星期五","星期六" };
	string ssecond = to_string(second);              string sminute = to_string(minute);                      string shour = to_string(hour);
	if(second<10) ssecond="0"+to_string(second);     if (minute < 10) sminute = "0" + to_string(minute);      if (hour < 10) shour = "0" + to_string(hour);
    //小时分钟秒转化为string
	str = to_string(year)+"年"+to_string(month)+"月"+to_string(day)+"日 "+week[dayofweek]+" "+shour + ":" + sminute + ":" + ssecond;
	::settextcolor(BLACK);
	::outtextxy(700, 10, str.c_str());
}

int Management::DayDiff(int smonth, int sday,  int emonth, int eday)
{
		int  m2, d2;int  m1, d1;
		m1 = (smonth + 9) % 12;
		d1 = (m1 * 306 + 5) / 10 + (sday - 1);
		m2 = (emonth + 9) % 12;
		d2 = (m2 * 306 + 5) / 10 + (eday - 1);
		return (d2 - d1);
}

void Management::Insert(string Name, string Phone, string Email, string Addition, int Year, int Month, int Day,int Type )
{
	switch (Type) 
	{
	case Person::Type_Relative: 
	{
		man[mannum] = NULL;
		man[mannum] = new Relative(Name,Phone,Email,Addition,Year,Month,Day,Type);
		break;
	}
	case Person::Type_Friend:
	{
		man[mannum] = NULL;
		man[mannum] = new Friend(Name, Phone, Email, Addition, Year, Month, Day, Type);
		break;
	}
	case Person::Type_Workmate:
	{
		man[mannum] = NULL;
		man[mannum] = new Workmate(Name, Phone, Email, Addition, Year, Month, Day, Type);
		break;
	}
	case Person::Type_Schoolmate:
	{
		man[mannum] = NULL;
		man[mannum] = new Schoolmate(Name, Phone, Email, Addition, Year, Month, Day, Type);
		break;
	}
	default:
		return;
		break;
	}
	birthmonth[Month-1]++;
	mannum++;
	return;
}

void Management::Sort()//重排联系人信息
{
	char cop[2] = { 0 };
	InputBox(cop, 2, "请输入排序方式：1:按姓名排序  2:按电话排序 3:按生日排序 (默认为0)", NULL, NULL, 0, 0, false);//false提供取消键，便于返回
	int i, j; Person* temp;
	if (mannum) 
	{
		for (i = 0; i < mannum; i++) 
		{
			for (j = mannum - 1; j > i; j--) //插入排序稳定且快
			{
				if (cop[0]-'0' == 1)
				{
					if (man[j - 1]->GetName() > man[j]->GetName()) //按姓名排序
					{
						temp = man[j - 1];
						man[j - 1] = man[j];
						man[j] = temp;
					}
				}
				if (cop[0]-'0' ==2)
				{
					if (man[j - 1]->GetTel() > man[j]->GetTel())//按电话号码排序
					{
						temp = man[j - 1];
						man[j - 1] = man[j];
						man[j] = temp;
					}
				}
				if (cop[0] - '0' == 3)
				{
					if (man[j - 1]->GetBirthTime() > man[j]->GetBirthTime())//按生日排序
					{
						temp = man[j - 1];
						man[j - 1] = man[j];
						man[j] = temp;
					}
				}
			}
		}
	}
	MessageBox(NULL, "修改成功！", "OK", MB_SYSTEMMODAL);//MessageBox可以弹出信息窗口
}

void Management::ReadFile()//读文件
{
	setlocale(LC_ALL, "chs");
	locale::global(locale("chs"));//防止中文输入呈现乱码
	ifstream file[4]; int i = 0; string Rname; string addition; int Ryear; int Rmonth; int Rday; int Rtype; string Rtel; string Remail;
	string book1 = "D:/新学习/个人通讯录管理系统/" + Admin + "/AddressBook1.txt"; string book2 = "D:/新学习/个人通讯录管理系统/" + Admin + "/AddressBook2.txt"; 
	string book3 = "D:/新学习/个人通讯录管理系统/" + Admin + "/AddressBook3.txt"; string book4 = "D:/新学习/个人通讯录管理系统/" + Admin + "/AddressBook4.txt";
	file[0].open(book1, ios::in);file[1].open(book2, ios::in);file[2].open(book3, ios::in);file[3].open(book4, ios::in);//分别打开用户文件夹中的文件
	for (int k = 0; k < 4; k++)
	{
		if (!file[k].is_open())
		{
			cout << "文件无法打开！" << endl;//debug用，查看是否成功打开文件夹
			return;
		}
		i = 0; file[k] >> i;group[k] = i;//读表头的数据，看出有多少人
		cout << k << "类有：" << i << "人" << endl;
		while (file[k] >> Rname >> Rtel >> Remail >> addition >> Ryear >> Rmonth >> Rday >> Rtype)
		{
			Insert(Rname, Rtel, Remail, addition, Ryear, Rmonth, Rday, Rtype);//读一条数据就新建一个联系人
		}
		file[k].close();//关闭文件
	}
}

/*file.open("通讯录\\AddressBook2.txt", ios::in);
	if (!file.is_open())
	{
		return;
	}
	i = 0;file >> i;
	group[Person::Type_Workmate] = i;
	cout << "Workmate类有：" << i << "人" << endl;
	while (file >> Rname >> Rtel >> Remail >> addition >> Ryear >> Rmonth >> Rday >> Rtype)
	{
		Insert(Rname, Rtel, Remail, addition, Ryear, Rmonth, Rday, Rtype);
	}
	file.close();*/

void Management::SaveFile()//保存文件
{
	setlocale(LC_ALL, "chs");	locale::global(locale("chs"));//防止中文输出呈现乱码
	ofstream file[4]; 
	string book1 = "D:/新学习/个人通讯录管理系统/" + Admin + "/AddressBook1.txt"; string book2 = "D:/新学习/个人通讯录管理系统/" + Admin + "/AddressBook2.txt";
	string book3 = "D:/新学习/个人通讯录管理系统/" + Admin + "/AddressBook3.txt"; string book4 = "D:/新学习/个人通讯录管理系统/" + Admin + "/AddressBook4.txt";
	file[0].open(book1, ios::out || ios::ate );file[1].open(book2, ios::out || ios::ate);file[2].open(book3, ios::out || ios::ate);file[3].open(book4, ios::out || ios::ate);//分别打开文件，清空重新写入
	int groupcount[4] = {0};
		for (int i = 0; i < mannum; i++)
		{
			if (man[i]->GetTypeNum() == Person::Type_Schoolmate)groupcount[0]++;//计数分组内有多少人
			if (man[i]->GetTypeNum() == Person::Type_Workmate)groupcount[1]++;
			if (man[i]->GetTypeNum() == Person::Type_Friend)groupcount[2]++;
			if (man[i]->GetTypeNum() == Person::Type_Relative)groupcount[3]++;
		}
		for (int i = 0; i < 4; i++)
		{
			file[i] << groupcount[i] << endl;
		}
		for (int i = 0; i < mannum; i++)
		{
			if (man[i]->GetTypeNum() == Person::Type_Schoolmate)
			{
				file[0] << man[i]->GetName() << " " << man[i]->GetTel() << " " << man[i]->GetEmail() << " " << man[i]->GetAddition() << " ";
				file[0] << man[i]->GetYear() << " " << man[i]->GetMonth() << " " << man[i]->GetDay() << " " << man[i]->GetTypeNum() << endl;//读取一条数据
			}
			if (man[i]->GetTypeNum() == Person::Type_Workmate)
			{
				file[1] << man[i]->GetName() << " " << man[i]->GetTel() << " " << man[i]->GetEmail() << " " << man[i]->GetAddition() << " ";
				file[1] << man[i]->GetYear() << " " << man[i]->GetMonth() << " " << man[i]->GetDay() << " " << man[i]->GetTypeNum() << endl;
			}
			if (man[i]->GetTypeNum() == Person::Type_Friend)
			{
				file[2] << man[i]->GetName() << " " << man[i]->GetTel() << " " << man[i]->GetEmail() << " " << man[i]->GetAddition() << " ";
				file[2] << man[i]->GetYear() << " " << man[i]->GetMonth() << " " << man[i]->GetDay() << " " << man[i]->GetTypeNum() << endl;
			}
			if (man[i]->GetTypeNum() == Person::Type_Relative)
			{
				file[3] << man[i]->GetName() << " " << man[i]->GetTel() << " " << man[i]->GetEmail() << " " << man[i]->GetAddition() << " ";
				file[3] << man[i]->GetYear() << " " << man[i]->GetMonth() << " " << man[i]->GetDay() << " " << man[i]->GetTypeNum() << endl;
			}
		}
	for (int i = 0; i < 4; i++)
	{
		file[i].close();//关闭文件
	}
}

bool Management::Mystrcmp(const char a[], char b[])//字符串不能用==比较，重写比较函数
{
	if (strlen(a) == strlen(b)) {
		for (int i = 0; i < strlen(a); i++) {
			if (a[i] != b[i]) {
				return false;
			}
		}
		return true;
	}
	else return false;
}

bool Management::Delete(char d[])//删除指定元素
{
	int t; char Dname[20]= {0}; int flag = 0;
	
	for (t = 0; t < mannum; t++)
	{   if (!mannum) return false;
		if (Mystrcmp(man[t]->GetName().c_str(), d))
		{
			string Inf = man[t]->GetName() + " 电话：" + man[t]->GetTel() + " 邮箱：" + man[t]->GetEmail() + " 备注：" + man[t]->GetAddition()
			+ "\n生日：" + to_string(man[t]->GetYear()) + "." + to_string(man[t]->GetMonth()) + "." + to_string(man[t]->GetDay());//将指定联系人信息用string链接显示
			if (InputBox(Dname, 20, Inf.c_str(), "请输入确认删除的联系人(点取消或确定)", NULL, 0, 0, false)==true)//InputBox点击确定返回true
			{
				birthmonth[man[t]->GetMonth() - 1]--;
				group[man[t]->GetTypeNum() - 1]--;
				delete man[t];
				man[t] = NULL;
				for (int i = t; i < mannum-1; i++)
				{
					man[i] = man[i + 1];//删除元素后，后续元素改变
				} 
				man[mannum-1] = NULL;//尾部元素置空
				flag++; t --; mannum--;//t--使函数重新从被删除元素开始遍历，以实现不同人重名情况下删除指定联系人的功能
			}
		}
	}

	if (flag) return true; //如果删除过返回true
	return false;//未删除过表示查无此人
}

void Management::DeleteAll()
{
	if (!mannum)return;
	for (int t = 0; t < mannum; t++) 
	{
		if (man[t] != NULL) {
			birthmonth[man[t]->GetMonth() - 1]--;
			group[man[t]->GetTypeNum() - 1]--;
			man[t] = { 0 };
			delete man[t];
		}
		mannum = 0;
	}
}

void Management::Sshow(int k)//选择显示分组联系人，k表示分组组别
{
	if(flag==0)return;
	IMAGE Showphoto; pagenum = group[k-1] / 15 + 1; string spage = "第" + to_string(page) + "页"; 
	loadimage(&Showphoto, "D:/新学习/个人通讯录管理系统/image/Showphoto.jpg", Window::Width(), Window::Height());
	putimage(0, 0, &Showphoto);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	outtextxy(8, 8, "|姓名|      |分组|      |备注|               |电话|              |邮箱|                          |生日|   ");
	outtextxy(750, 400, "按下ESC键退出");
	outtextxy(750, 440, "按下Space键翻页");
	outtextxy(430, 600,spage.c_str());
	if (Window::HasMsg())
	{
		menu_msg = Window::GetMsg();
		if (menu_msg.message == WM_KEYDOWN) {
			if (menu_msg.vkcode == VK_SPACE) {
				if (page < pagenum) page++;
				else page = 1;
			}
			if (menu_msg.vkcode == VK_ESCAPE) {
				flag = 0;
			}
		}
	}
	DrawDigitalClock(t.wYear, t.wMonth, t.wDay, t.wDayOfWeek, t.wHour, t.wMinute, t.wSecond);
	int i = 10, j = 50; int t = 0; int datanum = 1;//因为man[t]无法表示分组内的排序，防止页码显示混乱，使用datanum记录分组内各个联系人的序号
	switch (k)
	{
	case 1://选择显示同学分组
	{
		for (t = 0 ; (t < mannum)&&flag; t++)
		{
			if ((man[t]->GetTypeNum() == 1)) {
				if ((datanum <= page * 15) && (datanum > (page - 1) * 15)) {//如果这个联系人序号在规定的页数范围内
					string birthday = to_string(man[t]->GetYear()) + "." + to_string(man[t]->GetMonth()) + "." + to_string(man[t]->GetDay());
					outtextxy(i, j, man[t]->GetName().c_str());
					outtextxy(i + 70, j, man[t]->GetType().c_str());
					outtextxy(i + 150, j, man[t]->GetAddition().c_str());
					outtextxy(i + 260, j, man[t]->GetTel().c_str());
					outtextxy(i + 370, j, man[t]->GetEmail().c_str());
					outtextxy(i + 540, j, birthday.c_str());
					j = j + 30;
				}
				datanum++;//如果是分组内序号加1
			}
		}
		break;
	}
	case 2://选择显示同事分组
	{
		for (t = 0; (t < mannum) && flag; t++)
		{
			if ((man[t]->GetTypeNum() == 2)) {
				if ((datanum <= page * 15) && (datanum > (page - 1) * 15)) {
					string birthday = to_string(man[t]->GetYear()) + "." + to_string(man[t]->GetMonth()) + "." + to_string(man[t]->GetDay());
					outtextxy(i, j, man[t]->GetName().c_str());
					outtextxy(i + 70, j, man[t]->GetType().c_str());
					outtextxy(i + 150, j, man[t]->GetAddition().c_str());
					outtextxy(i + 260, j, man[t]->GetTel().c_str());
					outtextxy(i + 370, j, man[t]->GetEmail().c_str());
					outtextxy(i + 540, j, birthday.c_str());
					j = j + 30;
				}
				datanum++;
			}
		}
		break;
	}
	case 3://选择显示朋友分组
	{
		for (t = 0; (t < mannum) && flag; t++)
		{
			if ((man[t]->GetTypeNum() == 3)) {
				if ((datanum <= page * 15) && (datanum > (page - 1) * 15)) {
					string birthday = to_string(man[t]->GetYear()) + "." + to_string(man[t]->GetMonth()) + "." + to_string(man[t]->GetDay());
					outtextxy(i, j, man[t]->GetName().c_str());
					outtextxy(i + 70, j, man[t]->GetType().c_str());
					outtextxy(i + 150, j, man[t]->GetAddition().c_str());
					outtextxy(i + 260, j, man[t]->GetTel().c_str());
					outtextxy(i + 370, j, man[t]->GetEmail().c_str());
					outtextxy(i + 540, j, birthday.c_str());
					j = j + 30;
				}
				datanum++;
			}
		}
		break;
	}
	case 4://选择显示亲戚分组
	{
		for (t = 0; (t < mannum) && flag; t++)
		{
			if ((man[t]->GetTypeNum() == 4)) {
				if ((datanum <= page * 15) && (datanum > (page - 1) * 15)) {
					string birthday = to_string(man[t]->GetYear()) + "." + to_string(man[t]->GetMonth()) + "." + to_string(man[t]->GetDay());
					outtextxy(i, j, man[t]->GetName().c_str());
					outtextxy(i + 70, j, man[t]->GetType().c_str());
					outtextxy(i + 150, j, man[t]->GetAddition().c_str());
					outtextxy(i + 260, j, man[t]->GetTel().c_str());
					outtextxy(i + 370, j, man[t]->GetEmail().c_str());
					outtextxy(i + 540, j, birthday.c_str());
					j = j + 30;
				}
				datanum++;
			}
		}
		break;
	}
    dafault:break;
	}
}

void Management::ShowBirthNnum()
{
		char cmonth[3] = { 0 };
		InputBox(cmonth, 3, "请输入指定月份", NULL, NULL, 0, 0, true);
		int month = atoi(cmonth); string p = to_string(birthmonth[month - 1]);
		MessageBox(NULL, p.c_str(), "出生人数：", MB_SYSTEMMODAL);
		return;
}

void Management::MerryBirthday()//显示5天内生日联系人并给出祝福
{
	string spage = "第" + to_string(page) + "页"; 
	SYSTEMTIME nt;IMAGE Showphoto;//SYSTEMTIME变量用于获取现在的时间 
	::loadimage(&Showphoto, "D:/新学习/个人通讯录管理系统/image/Showphoto.jpg", Window::Width(), Window::Height());
	putimage(0, 0, &Showphoto);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);//设置背景颜色，格式
	outtextxy(8, 8, "|姓名|      |分组|      |备注|               |电话|              |邮箱|                          |生日|   ");
	outtextxy(750, 400, "按下ESC键回到主菜单");
	outtextxy(750, 440, "按下Space键翻页");
	outtextxy(430, 600, spage.c_str());//显示页码
	if (Window::HasMsg())
	{   menu_msg = Window::GetMsg();
		if (menu_msg.message == WM_KEYDOWN) {
			if (menu_msg.vkcode == VK_SPACE) {
				if (page < pagenum) page++;
				else page = 1;
			}
			if (menu_msg.vkcode == VK_ESCAPE) {flag = 0;}
		}
	}
	DrawDigitalClock(t.wYear, t.wMonth, t.wDay, t.wDayOfWeek, t.wHour, t.wMinute, t.wSecond);
	GetLocalTime(&nt);//获取当前时间
	int i = 10, j = 50, datanum = 0; int nyear = nt.wYear; int nmonth = nt.wMonth; int nday = nt.wDay; pagenum = 0;
	for (int k = 0; k < mannum; k++) {
		int dyear = man[k]->GetYear(); int dmonth = man[k]->GetMonth(); int dday = man[k]->GetDay(); int diff = DayDiff( nmonth, nday,dmonth, dday);//DayDiff函数用于获取目标月份日期与今天的时间距离
		if (diff <= 5 && diff >= 0) //生日在五天内
		{
			datanum++;
			if (datanum % 15 == 1)pagenum++;
			string week[7] = {"星期六","星期日", "星期一","星期二","星期三","星期四","星期五"};//用string数组储存星期，便于输出
			if ((datanum <= page * 15) && (datanum > (page - 1) * 15)) 
			{
				string birthday = to_string(man[k]->GetMonth()) + "月" + to_string(man[k]->GetDay()) + "日" 
				+ "(" + week[CalculateWeekDay(dyear, dmonth, dday) - 1] + ")";//CalculateWeekDay函数用于计算星期几
				outtextxy(i, j, man[k]->GetName().c_str());
				outtextxy(i + 70, j, man[k]->GetType().c_str());
				outtextxy(i + 150, j, man[k]->GetAddition().c_str());
				outtextxy(i + 260, j, man[k]->GetTel().c_str());
				outtextxy(i + 370, j, man[k]->GetEmail().c_str());
				outtextxy(i + 540, j, birthday.c_str());
				j = j + 30;
				WriteWish(man[k]);//写该人的祝福
			}
		}
	}
}

void Management::WriteWish(Person*m)
{
	ofstream file; string filebrowse = "D:/新学习/个人通讯录管理系统/" + Admin + "/生日祝福"; string filename = "D:/新学习/个人通讯录管理系统/" + Admin + "/生日祝福/给" + (m->GetName()) + ".txt";//指定文件夹里的文件名
	if (!filesystem::is_directory(filebrowse)) {
		filesystem::create_directory(filebrowse);
	}
	file.open(filename, ios::out | ios::ate);//指定文件夹里清空并重新输入（显示有循环，不能追加）
	if (!file.is_open()) {
		return;
	}
	string end = "                                   " + Adminname;
	file << m->GetName() << ":" << endl;
	file << "           祝生日快乐，健康幸福。" << endl;
	file << end << endl;
	file.close();
	return;
}

int Management::CalculateWeekDay(int y, int m, int d)
{
	if (m == 1 || m == 2) //把一月和二月换算成上一年的十三月和十四月  
	{
		m += 12;
		y--;
	}
	int Week = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7;//基姆拉尔森计算公式
	switch (Week)
	{
	case 0: return 1; break;
	case 1: return 2; break;
	case 2: return 3; break;
	case 3: return 4; break;
	case 4: return 5; break;
	case 5: return 6; break;
	case 6: return 7; break;
	}
}
