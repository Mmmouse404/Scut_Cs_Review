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
	//���ر���ͼƬ
	::loadimage(&bk, "D:/��ѧϰ/����ͨѶ¼����ϵͳ/image/bk.jpg", Window::Width(), Window::Height());
	//���水ť��ʼ��
	m_button.push_back(new Button("��ʾ����"));
	m_button.push_back(new Button("����"));
	m_button.push_back(new Button("ɾ��"));
	m_button.push_back(new Button("�޸�"));
	m_button.push_back(new Button("����"));
	m_button.push_back(new Button("����"));
	m_button.push_back(new Button("�˳�"));
	s_button.push_back(new Button("��ʾͬѧ����"));
	s_button.push_back(new Button("��ʾͬ�·���"));
	s_button.push_back(new Button("��ʾ���ѷ���"));
	s_button.push_back(new Button("��ʾ���ݷ���"));
	s_button.push_back(new Button("��ʾ�����·ݳ�������"));
	s_button.push_back(new Button("��������������ϵ��ף��"));
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
	}//������ťλ��
	mannum = 0;
}

bool Management::Log()
{
	char Adname[20] = { 0 }; char choice[2] = { 0 }; char Adcode[20] = { 0 };
	if (InputBox(choice, 2, "����1/2/3ѡ�񣺵�¼/ע��/�޸�����(��������Ϊ�˳�)", NULL, NULL, 0, 0, false) == false)
	{
		exit(0);
	}
	switch (choice[0] - '0')
	{
	case 1: {
		if (InputBox(Adname, 20, "�������û���", NULL, NULL, 0, 0, false) == false)
		{
			return false;
		}
		Admin = string(Adname) + "��ͨѶ¼"; string PasswordFile = "D:/��ѧϰ/����ͨѶ¼����ϵͳ/" + Admin + "/����.txt"; string AdminFile = "D:/��ѧϰ/����ͨѶ¼����ϵͳ/" + Admin;
		if (!filesystem::is_directory(AdminFile))
		{
			MessageBox(NULL, "�û��������ڣ�", "False", MB_SYSTEMMODAL);
		}
		else {
			ifstream file; file.open(PasswordFile, ios::in);
			if (!file.is_open())
			{
				MessageBox(NULL, "�޷��������ļ���", "False", MB_SYSTEMMODAL); return false;
			}
			else file >> Password;
			InputBox(Adcode, 20, "����������", NULL, NULL, 0, 0, false);
			if (Mystrcmp(Password.c_str(), Adcode)) {
				MessageBox(NULL, "��½�ɹ���", "OK��", MB_SYSTEMMODAL); Adminname = Adname;
				return true;
			}
			else {
				MessageBox(NULL, "�������û�����ƥ�䣡", "False��", MB_SYSTEMMODAL); 
				return false;
			}
			break;
		}
	}
	case 2: {
		if (InputBox(Adname, 20, "�������û���", NULL, NULL, 0, 0, false) == false)
		{
			return false;
		}
		else
		{
			Admin = string(Adname) + "��ͨѶ¼"; ofstream file; string PasswordFile = "D:/��ѧϰ/����ͨѶ¼����ϵͳ/" + Admin + "/����.txt"; string AdminFile = "D:/��ѧϰ/����ͨѶ¼����ϵͳ/" + Admin;
			if (filesystem::is_directory(AdminFile)) {
				MessageBox(NULL, "�û��ѱ�ע�ᣡ", "False", MB_SYSTEMMODAL);
				return false;
			}
			else {
				InputBox(Adcode, 20, "����������(������������)", NULL, NULL, 0, 0, false);
				if (strlen(Adcode) == 0 || strlen(Adname) == 0)
				{
					MessageBox(NULL, "����Ŀ�����", "False", MB_SYSTEMMODAL);//����һ�����������û�
					return false;
				}
				else;
				{
					string AdminFile = "D:/��ѧϰ/����ͨѶ¼����ϵͳ/" + Admin;
						if(!filesystem::is_directory(AdminFile))filesystem::create_directory(AdminFile);
						file.open(PasswordFile, ios::out | ios::ate);//ָ���ļ�������ղ���������
						file << string(Adcode) << endl;
						file.close();
						string book1 = "D:/��ѧϰ/����ͨѶ¼����ϵͳ/" + Admin + "/AddressBook1.txt"; file.open(book1, ios::out); file << 0 << endl; file.close();
						string book2 = "D:/��ѧϰ/����ͨѶ¼����ϵͳ/" + Admin + "/AddressBook2.txt"; file.open(book2, ios::out); file << 0 << endl; file.close();
						string book3 = "D:/��ѧϰ/����ͨѶ¼����ϵͳ/" + Admin + "/AddressBook3.txt"; file.open(book3, ios::out); file << 0 << endl; file.close();
						string book4 = "D:/��ѧϰ/����ͨѶ¼����ϵͳ/" + Admin + "/AddressBook4.txt"; file.open(book4, ios::out); file << 0 << endl; file.close();
						MessageBox(NULL, "ע��ɹ�", "OK!", MB_SYSTEMMODAL);
						return false;	
				}
			}
		}
		break;
	}
	case 3: {
		if (InputBox(Adname, 20, "�������û���", NULL, NULL, 0, 0, false) == false)
		{
			return false;
		}
		else
		{
			Admin = string(Adname) + "��ͨѶ¼"; fstream file; string PasswordFile = "D:/��ѧϰ/����ͨѶ¼����ϵͳ/"  + Admin + "/����.txt"; char Recode[20] = { 1 }; char Incode[20] = {0}; 
			string AdminFile = "D:/��ѧϰ/����ͨѶ¼����ϵͳ/" + Admin;
			file.open(PasswordFile, ios::in); file >> Password; file.close();
			if (filesystem::is_directory(AdminFile)) 
			{
				if (InputBox(Adcode, 20, "������ԭ��������(������������)", NULL, NULL, 0, 0, false)) {
					if (Mystrcmp(Password.c_str(), Adcode))
					{
						InputBox(Recode, 20, "�������µ�����(������������)", NULL, NULL, 0, 0, false);
						InputBox(Incode, 20, "���ٴ�ȷ������(������������)", NULL, NULL, 0, 0, false);

						if (Mystrcmp(Incode, Recode))
						{
							file.open(PasswordFile, ios::out | ios::ate);//ָ���ļ�������ղ���������
							file << string(Recode) << endl;
							file.close();
							MessageBox(NULL, "�޸ĳɹ���", "OK", MB_SYSTEMMODAL);
						}
						else { MessageBox(NULL, "�������벻һ�£�", "OK", MB_SYSTEMMODAL); }
					}
					else { MessageBox(NULL, "�������", "False!", MB_SYSTEMMODAL); }
				}
			}
			else
			{
				MessageBox(NULL, "�û��������ڣ�", "False", MB_SYSTEMMODAL); 
			} 
		}
		return false;
		break;
	}
	default:
		exit(0);
	}
}

void Management::Run()//���й���ϵͳ
{
	int op = menu;//ѭ����ʼ����֤һ��ʼ�ã��˵������ܣ���ʾ�˵�
	Window::BeginDraw();//��ʼ����ͼ�Σ�����ʾ
	ReadFile();//��ʼ�������ļ���ȡ
	while (1) 
	{
		Window::Clear();//����
		DrawBackground();//���Ʊ���ͼƬ
		GetLocalTime(&t); //��ȡ��ǰʱ�� 
		DrawDigitalClock(t.wYear,t.wMonth,t.wDay,t.wDayOfWeek,t.wHour, t.wMinute, t.wSecond);//��ʾ��ʱ��
		if (Window::HasMsg())
		{
			menu_msg = Window::GetMsg();
		}//��ȡ��������Ϣ��ʵ�ִ��ڽ���
		if (menu_msg.vkcode == VK_ESCAPE) {
			op = menu;
		}//��ȡ����ESC��Ϣ�˳�
		switch (op)//���ݰ�ť���ص���Ϣ���й���ʵ��
		{
		case menu: 
		{
			page = 1; flag = 1;
			op = Menu();//Menu���ܰ�ť��Ϣ
			break; 
		}
		case show: 
		{
			Show();//��ʾ������ϵ��
			break; 
		}
		case add: 
		{
			Add();//������ϵ��
			SaveFile();//�����ļ�
			op = menu;//ʵ�ֺ�ص��˵�������ͬ��
			break; 
		}
		case erase: 
		{
			Erase();//ɾ����ϵ��
			SaveFile();
			op = menu;
			break;
		}
		case modify:
		{
			Modify();//�޸���ϵ��
			SaveFile();
			op = menu;
			break; 
		}
		case search: 
		{
			Search();//������ϵ��
			op = menu;
			break; 
		}
		case sort:
		{
			Sort();//������ϵ��
			op = menu;
			break;
		}
		case show1+7:
		{
			Sshow(1);//��ʾͬѧ������ϵ��,��ʾ��������ֻ��ʾһ�ξͷ��أ�����û��op=menu
			break;
		}
		case show2+7:
		{
			Sshow(2);//��ʾͬ�·�����ϵ��
			break;
		}
		case show3+7:
		{
			Sshow(3);//��ʾ���ѷ�����ϵ��
			break;
		}
		case show4+7:
		{
			Sshow(4);//��ʾ���ݷ�����ϵ��
			break;
		}
		case showbirth+7:
		{
			ShowBirthNnum();//ָ���·ݣ���ʾ���·ݳ�������ϵ��
			op = menu;
			break;
		}
		case givebirth + 7:
		{
			MerryBirthday();//��������������յ���ϵ��ף��
			break;
		}
		case Exit:
			exit(0); break;//�˳�ϵͳ
		default:
			break;
		}
		Window::FlushDraw();//����Switchʵ�ֺ󣬿�ʼ���ƣ����Ʋ˵��ı�����
	}
	Window::EndDraw();//��������
}

int Management::Menu()
{
	settextstyle(30, 0, "����");//���ñ�������
	::outtextxy(430,100, Admin.c_str());//��ʾ����
	settextstyle(18, 0, "����");//������������
	int i = 0;
	for(i=0;i<m_button.size();i++)
	{
		m_button[i]->Loop(menu_msg);//�¼�ѭ�������ϻ�ȡmenu����Ϣ�����Լ�����꣩
		m_button[i]->Show();//��ʾ��ť
		if (m_button[i]->IsClicked()) {
			return i;//�����ť��������򷵻���Ӧ�İ�ťֵ��ʵ�ֲ�ͬ����
		}
	}
	for (i = 0; i < s_button.size(); i++)
	{
		s_button[i]->Loop(menu_msg);
		s_button[i]->Show();
		if (s_button[i]->IsClicked()) {
			return i+7;//ͬ�ϣ���Ϊֻ����һ��switch����Ҫ��ȥ
		}
	}
	return menu;
}

void Management::Show()//��ʾȫ����ϵ��
{
	if (flag == 0)return;
	IMAGE Showphoto;  pagenum = mannum / 15 + 1; string spage = "��" + to_string(page) + "ҳ";//string����ҳ��
	loadimage(&Showphoto, "D:/��ѧϰ/����ͨѶ¼����ϵͳ/image/Showphoto.jpg", Window::Width(), Window::Height());//Showphoto��Ϊ��������µı���ͼƬ
	putimage(0, 0, &Showphoto);//����ͼƬ�ڴ�����
	settextcolor(BLACK);//������ɫ����Ϊ��ɫ
	setbkmode(TRANSPARENT);//��������Ϊ����
	outtextxy(8, 8, "|����|      |����|      |��ע|               |�绰|              |����|                          |����|   ");//��ʾ��ͷ
	outtextxy(750, 400, "����ESC���ص����˵�");
	outtextxy(750, 440, "����Space����ҳ");
	outtextxy(430, 600, spage.c_str());//��ʾҳ��
	if (Window::HasMsg())
	{
		menu_msg= Window::GetMsg();
		if (menu_msg.message == WM_KEYDOWN) {
			if (menu_msg.vkcode == VK_SPACE) {
				if (page < pagenum) page++;//�������SPACE����ҳ������
				else page = 1;//ҳ��ﵽ���޻ص���һҳ
			}
			if (menu_msg.vkcode == VK_ESCAPE) {
				flag = 0;//����ESC���ʾ���������ֱ��return
			}
		}
	}
	int i = 10, j = 50; 
	for (int t = 0 + (page - 1) * 15; (t < 15 * page) && (t < mannum)&&flag; t++)
	{
		string birthday = to_string(man[t]->GetYear()) + "." + to_string(man[t]->GetMonth()) + "." + to_string(man[t]->GetDay());//������������
		outtextxy(i, j, man[t]->GetName().c_str());//��ʾ����
		outtextxy(i + 70, j, man[t]->GetType().c_str());//��ʾ����
		outtextxy(i + 150, j, man[t]->GetAddition().c_str());//��ʾ��ע
		outtextxy(i + 260, j, man[t]->GetTel().c_str());//��ʾ�绰
		outtextxy(i + 370, j, man[t]->GetEmail().c_str());//��ʾ����
		outtextxy(i + 540, j, birthday.c_str());//��ʾ����
		j = j + 30;
	}
	DrawDigitalClock(t.wYear,t.wMonth,t.wDay,t.wDayOfWeek,t.wHour, t.wMinute, t.wSecond);//��ʾʱ��
}

void Management::Add()
{
	DrawDigitalClock(t.wYear,t.wMonth,t.wDay,t.wDayOfWeek,t.wHour, t.wMinute, t.wSecond);
	char Rname[20] = { 0 }; char addition[20] = {0}; char Rtel[12] = { 0 }; char Remail[30] = { 0 };
	char cRyear[5]; char cRmonth[3]; char cRday[3]; char cRtype[2];
	InputBox(Rname, 20, "��������ϵ������", NULL, NULL, 0, 0, false);
	if (Rname[0] != 0) {
		InputBox(Rtel, 12, "��������ϵ�˵绰", NULL, NULL, 0, 0, true);
		InputBox(Remail, 30, "��������ϵ������", NULL, NULL, 0, 0, true);
		InputBox(cRyear, 5, "��������ϵ�˳������", NULL, NULL, 0, 0, true);
		InputBox(cRmonth, 3, "��������ϵ�˳����·�", NULL, NULL, 0, 0, true);
		InputBox(cRday, 3, "��������ϵ�˳�������", NULL, NULL, 0, 0, true);
	}
	//1.�����
	if (strlen(Rname) == 0 || strlen(Rtel) == 0 || strlen(Remail) == 0 || strlen(cRyear) == 0 || strlen(cRmonth) == 0 || strlen(cRday) == 0)
	{
		MessageBox(NULL, "����Ŀ�����", "False", MB_SYSTEMMODAL);//����һ�����������û�
		return;                                          
	}
	//2.����̫��
	if (strlen(Rname) > 20 || strlen(Rtel) >20 || strlen(Remail) > 30 || strlen(cRyear) > 4 || strlen(cRmonth) > 2 || strlen(cRday) >2)
	{
		MessageBox(NULL, "������ַ�������Χ��", "False", MB_SYSTEMMODAL);
		return ;
	}
	int Ryear = atoi(cRyear); int Rmonth = atoi(cRmonth); int Rday = atoi(cRday); 
	//3.��������
	if ( Ryear-'0' >2023 || Rmonth-'0' > 12 || Rday-'0' > 31)
	{
		MessageBox(NULL, "�������ڳ�����Χ��", "False", MB_SYSTEMMODAL);
		return;
	}
	//4.������ϵ���������
	if (mannum>200)
	{
		MessageBox(NULL, "��ϵ�˹��࣡", "False", MB_SYSTEMMODAL);//����һ�����������û�
		return;
	}
	InputBox(cRtype, 3, "��������ϵ�˷��� (1:ͬѧ 2:ͬ�� 3:���� 4:����)", NULL, NULL, 0, 0, true); int Rtype = atoi(cRtype);
	switch (Rtype) 
	{
	case 1: {
		InputBox(addition, 20, "��������ϵ��ѧУ", NULL, NULL, 0, 0, true); 
		Insert(Rname, Rtel, Remail, addition, Ryear, Rmonth, Rday, Rtype);
		break;
	}
	case 2: {
		InputBox(addition, 20, "��������ϵ�˵�λ", NULL, NULL, 0, 0, true); 
		Insert(Rname, Rtel, Remail, addition, Ryear, Rmonth, Rday, Rtype);
		break;
	}
	case 3: {
		InputBox(addition, 20, "��������ϵ����ʶ�ص�", NULL, NULL, 0, 0, true); 
		Insert(Rname, Rtel, Remail, addition, Ryear, Rmonth, Rday, Rtype);
		break;
	}
	case 4: {
		InputBox(addition, 20, "��������ϵ�˳ƺ�", NULL, NULL, 0, 0, true); 
		Insert(Rname, Rtel, Remail, addition, Ryear, Rmonth, Rday, Rtype);
		break;
	}
	default:
		MessageBox(NULL, "�������", "False", MB_SYSTEMMODAL);
		return; 
		break;
	}
	cout << man[mannum - 1]->GetName() << man[mannum - 1]->GetTel() << man[mannum - 1]->GetEmail() << endl;
	MessageBox(NULL, "��ӳɹ���", "OK", MB_SYSTEMMODAL);
}

void Management::Erase()//ɾ������ʵ��
{
	DrawDigitalClock(t.wYear,t.wMonth,t.wDay,t.wDayOfWeek,t.wHour, t.wMinute, t.wSecond);
	char Dname[20] = { 0 }; char Dchoice[20] = {0};
	InputBox(Dchoice, 2, "����1/2ѡ�����������ϵ����Ϣ/ɾ��������ϵ��",NULL,NULL,0,0,false);
	if (Dchoice[0] - '0' == 1) {
		DeleteAll();
		MessageBox(NULL, "ɾ���ɹ���", "OK", MB_SYSTEMMODAL);
	}
	if (Dchoice[0] - '0' == 2) {
		InputBox(Dname, 20, "������Ҫɾ��������", NULL, NULL, 0, 0, false);
		if (Delete(Dname)) {
			MessageBox(NULL, "ɾ���ɹ���", "OK", MB_SYSTEMMODAL);
		}
		else { MessageBox(NULL, "ɾ��ʧ�ܣ�", "����", MB_SYSTEMMODAL); }
	}
	SaveFile();
}

void Management::Modify()
{
	DrawDigitalClock(t.wYear,t.wMonth,t.wDay,t.wDayOfWeek,t.wHour, t.wMinute, t.wSecond);
	char Sname[20] = { 0 };  char Rtel[12] = { 0 }; char Remail[30] = { 0 }; char cRtype[2]; char addition[20]; char mstr[20] = { 0 }; int flag = 0;//Ԥ������ַ������ÿձ���ȡ���󷵻ز˵�
	InputBox(Sname, 20, "������Ҫ�޸ĵ���ϵ������/�绰", NULL, NULL, 0, 0, false);
	if (mannum)
	{
		for (int t = 0; t < mannum; t++)
		{
			if (Mystrcmp(man[t]->GetName().c_str(), Sname) || Mystrcmp(man[t]->GetTel().c_str(), Sname))
			{
				string Inf = man[t]->GetName() + " �绰��" + man[t]->GetTel() + " ���䣺" + man[t]->GetEmail() + " ��ע��" + man[t]->GetAddition() 
					+ "\n���գ�" + to_string(man[t]->GetYear()) + "." + to_string(man[t]->GetMonth()) + "." + to_string(man[t]->GetDay());
				if (InputBox(mstr, 20, Inf.c_str(), "��ȷ���Ƿ��޸ĸ���ϵ��(��ȡ����ȷ��)", NULL, 0, 0, false)) 
				{
					InputBox(Rtel, 12, "���޸���ϵ�˵绰", NULL, NULL, 0, 0, true);
					InputBox(Remail, 30, "���޸���ϵ������", NULL, NULL, 0, 0, true);
					InputBox(cRtype, 3, "���޸���ϵ�˷��� (1:ͬѧ 2:ͬ�� 3:���� 4:����)", NULL, NULL, 0, 0, true); int Rtype = atoi(cRtype);
					//1.�����
					if (strlen(Rtel) == 0 || strlen(Remail) == 0){
						MessageBox(NULL, "����Ŀ�����", "False", MB_SYSTEMMODAL);//����һ�����������û�
						t--; continue;
					}
					//2.����̫��
					if (strlen(Rtel) > 20 || strlen(Remail) > 30){
						MessageBox(NULL, "������ַ�������Χ��", "False", MB_SYSTEMMODAL);
						t--; continue;
					}
					man[t]->SetTel(Rtel); man[t]->SetEmail(Remail);
					switch (Rtype)
					{
					case 1: {
									InputBox(addition, 20, "��������ϵ��ѧУ", NULL, NULL, 0, 0, true);
									man[t]->SetType(Rtype); man[t]->SetAddtion(addition); break;
							}
					case 2: {
									InputBox(addition, 20, "��������ϵ�˵�λ", NULL, NULL, 0, 0, true);
									man[t]->SetType(Rtype); man[t]->SetAddtion(addition); break;
							}
					case 3: {
									InputBox(addition, 20, "��������ϵ����ʶ�ص�", NULL, NULL, 0, 0, true);
									man[t]->SetType(Rtype); man[t]->SetAddtion(addition); break;
							}
					case 4: {
									InputBox(addition, 20, "��������ϵ�˳ƺ�", NULL, NULL, 0, 0, true);
									man[t]->SetType(Rtype); man[t]->SetAddtion(addition); break;
							}
					}
					MessageBox(NULL, "�޸ĳɹ���", "OK", MB_SYSTEMMODAL);flag = 1;
				}
			}
		}
		if (flag == 0)MessageBox(NULL, "�޸�ʧ�ܣ�", "����", MB_SYSTEMMODAL); return;
	}
	else { MessageBox(NULL, "����ϵ��", "����", MB_SYSTEMMODAL); }
}

void Management::Search()//������ϵ����Ҫ����
{
	DrawDigitalClock(t.wYear,t.wMonth,t.wDay,t.wDayOfWeek,t.wHour, t.wMinute, t.wSecond);
	::outtextxy(0, 600, "����ESC���˳�");
	char Sname[20] = {0};
	InputBox(Sname, 20, "������Ҫ���ҵ���ϵ������/�绰���绰����������", NULL, NULL, 0, 0, false);//InputBox���û����
	int t; flag = 0;
	if (mannum)//��ϵ��������Ϊ0
	{
		for (t = 0; t < mannum; t++)
		{
			if (Mystrcmp(man[t]->GetName().c_str(), Sname) || Mystrcmp(man[t]->GetTel().c_str(), Sname))//�ַ����Ƚϲ�����==������ʹ��Mystrcmp����
			{
				MessageBox(NULL, "���ҳɹ���", "OK", MB_SYSTEMMODAL);
				string Inf = man[t]->GetName() + " �绰��" + man[t]->GetTel() + " ���䣺" + man[t]->GetEmail() + " ��ע��" + man[t]->GetAddition()
					+ "\n���գ�" + to_string(man[t]->GetYear()) + "." + to_string(man[t]->GetMonth()) + "." + to_string(man[t]->GetDay());
				MessageBox(NULL, Inf.c_str(), "��ϵ����Ϣ��", MB_SYSTEMMODAL); flag = 1;
			}
		}
	  if(flag==0)MessageBox(NULL, "�޴���ϵ��", "����", MB_SYSTEMMODAL); 
	}
	else {MessageBox(NULL, "����ϵ��", "����", MB_SYSTEMMODAL);}
}

void Management::DrawDigitalClock(int year,int month,int day,int dayofweek,int hour, int minute, int second)
{
	string str; string week[7] = { "������","����һ","���ڶ�","������","������","������","������" };
	string ssecond = to_string(second);              string sminute = to_string(minute);                      string shour = to_string(hour);
	if(second<10) ssecond="0"+to_string(second);     if (minute < 10) sminute = "0" + to_string(minute);      if (hour < 10) shour = "0" + to_string(hour);
    //Сʱ������ת��Ϊstring
	str = to_string(year)+"��"+to_string(month)+"��"+to_string(day)+"�� "+week[dayofweek]+" "+shour + ":" + sminute + ":" + ssecond;
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

void Management::Sort()//������ϵ����Ϣ
{
	char cop[2] = { 0 };
	InputBox(cop, 2, "����������ʽ��1:����������  2:���绰���� 3:���������� (Ĭ��Ϊ0)", NULL, NULL, 0, 0, false);//false�ṩȡ���������ڷ���
	int i, j; Person* temp;
	if (mannum) 
	{
		for (i = 0; i < mannum; i++) 
		{
			for (j = mannum - 1; j > i; j--) //���������ȶ��ҿ�
			{
				if (cop[0]-'0' == 1)
				{
					if (man[j - 1]->GetName() > man[j]->GetName()) //����������
					{
						temp = man[j - 1];
						man[j - 1] = man[j];
						man[j] = temp;
					}
				}
				if (cop[0]-'0' ==2)
				{
					if (man[j - 1]->GetTel() > man[j]->GetTel())//���绰��������
					{
						temp = man[j - 1];
						man[j - 1] = man[j];
						man[j] = temp;
					}
				}
				if (cop[0] - '0' == 3)
				{
					if (man[j - 1]->GetBirthTime() > man[j]->GetBirthTime())//����������
					{
						temp = man[j - 1];
						man[j - 1] = man[j];
						man[j] = temp;
					}
				}
			}
		}
	}
	MessageBox(NULL, "�޸ĳɹ���", "OK", MB_SYSTEMMODAL);//MessageBox���Ե�����Ϣ����
}

void Management::ReadFile()//���ļ�
{
	setlocale(LC_ALL, "chs");
	locale::global(locale("chs"));//��ֹ���������������
	ifstream file[4]; int i = 0; string Rname; string addition; int Ryear; int Rmonth; int Rday; int Rtype; string Rtel; string Remail;
	string book1 = "D:/��ѧϰ/����ͨѶ¼����ϵͳ/" + Admin + "/AddressBook1.txt"; string book2 = "D:/��ѧϰ/����ͨѶ¼����ϵͳ/" + Admin + "/AddressBook2.txt"; 
	string book3 = "D:/��ѧϰ/����ͨѶ¼����ϵͳ/" + Admin + "/AddressBook3.txt"; string book4 = "D:/��ѧϰ/����ͨѶ¼����ϵͳ/" + Admin + "/AddressBook4.txt";
	file[0].open(book1, ios::in);file[1].open(book2, ios::in);file[2].open(book3, ios::in);file[3].open(book4, ios::in);//�ֱ���û��ļ����е��ļ�
	for (int k = 0; k < 4; k++)
	{
		if (!file[k].is_open())
		{
			cout << "�ļ��޷��򿪣�" << endl;//debug�ã��鿴�Ƿ�ɹ����ļ���
			return;
		}
		i = 0; file[k] >> i;group[k] = i;//����ͷ�����ݣ������ж�����
		cout << k << "���У�" << i << "��" << endl;
		while (file[k] >> Rname >> Rtel >> Remail >> addition >> Ryear >> Rmonth >> Rday >> Rtype)
		{
			Insert(Rname, Rtel, Remail, addition, Ryear, Rmonth, Rday, Rtype);//��һ�����ݾ��½�һ����ϵ��
		}
		file[k].close();//�ر��ļ�
	}
}

/*file.open("ͨѶ¼\\AddressBook2.txt", ios::in);
	if (!file.is_open())
	{
		return;
	}
	i = 0;file >> i;
	group[Person::Type_Workmate] = i;
	cout << "Workmate���У�" << i << "��" << endl;
	while (file >> Rname >> Rtel >> Remail >> addition >> Ryear >> Rmonth >> Rday >> Rtype)
	{
		Insert(Rname, Rtel, Remail, addition, Ryear, Rmonth, Rday, Rtype);
	}
	file.close();*/

void Management::SaveFile()//�����ļ�
{
	setlocale(LC_ALL, "chs");	locale::global(locale("chs"));//��ֹ���������������
	ofstream file[4]; 
	string book1 = "D:/��ѧϰ/����ͨѶ¼����ϵͳ/" + Admin + "/AddressBook1.txt"; string book2 = "D:/��ѧϰ/����ͨѶ¼����ϵͳ/" + Admin + "/AddressBook2.txt";
	string book3 = "D:/��ѧϰ/����ͨѶ¼����ϵͳ/" + Admin + "/AddressBook3.txt"; string book4 = "D:/��ѧϰ/����ͨѶ¼����ϵͳ/" + Admin + "/AddressBook4.txt";
	file[0].open(book1, ios::out || ios::ate );file[1].open(book2, ios::out || ios::ate);file[2].open(book3, ios::out || ios::ate);file[3].open(book4, ios::out || ios::ate);//�ֱ���ļ����������д��
	int groupcount[4] = {0};
		for (int i = 0; i < mannum; i++)
		{
			if (man[i]->GetTypeNum() == Person::Type_Schoolmate)groupcount[0]++;//�����������ж�����
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
				file[0] << man[i]->GetYear() << " " << man[i]->GetMonth() << " " << man[i]->GetDay() << " " << man[i]->GetTypeNum() << endl;//��ȡһ������
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
		file[i].close();//�ر��ļ�
	}
}

bool Management::Mystrcmp(const char a[], char b[])//�ַ���������==�Ƚϣ���д�ȽϺ���
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

bool Management::Delete(char d[])//ɾ��ָ��Ԫ��
{
	int t; char Dname[20]= {0}; int flag = 0;
	
	for (t = 0; t < mannum; t++)
	{   if (!mannum) return false;
		if (Mystrcmp(man[t]->GetName().c_str(), d))
		{
			string Inf = man[t]->GetName() + " �绰��" + man[t]->GetTel() + " ���䣺" + man[t]->GetEmail() + " ��ע��" + man[t]->GetAddition()
			+ "\n���գ�" + to_string(man[t]->GetYear()) + "." + to_string(man[t]->GetMonth()) + "." + to_string(man[t]->GetDay());//��ָ����ϵ����Ϣ��string������ʾ
			if (InputBox(Dname, 20, Inf.c_str(), "������ȷ��ɾ������ϵ��(��ȡ����ȷ��)", NULL, 0, 0, false)==true)//InputBox���ȷ������true
			{
				birthmonth[man[t]->GetMonth() - 1]--;
				group[man[t]->GetTypeNum() - 1]--;
				delete man[t];
				man[t] = NULL;
				for (int i = t; i < mannum-1; i++)
				{
					man[i] = man[i + 1];//ɾ��Ԫ�غ󣬺���Ԫ�ظı�
				} 
				man[mannum-1] = NULL;//β��Ԫ���ÿ�
				flag++; t --; mannum--;//t--ʹ�������´ӱ�ɾ��Ԫ�ؿ�ʼ��������ʵ�ֲ�ͬ�����������ɾ��ָ����ϵ�˵Ĺ���
			}
		}
	}

	if (flag) return true; //���ɾ��������true
	return false;//δɾ������ʾ���޴���
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

void Management::Sshow(int k)//ѡ����ʾ������ϵ�ˣ�k��ʾ�������
{
	if(flag==0)return;
	IMAGE Showphoto; pagenum = group[k-1] / 15 + 1; string spage = "��" + to_string(page) + "ҳ"; 
	loadimage(&Showphoto, "D:/��ѧϰ/����ͨѶ¼����ϵͳ/image/Showphoto.jpg", Window::Width(), Window::Height());
	putimage(0, 0, &Showphoto);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	outtextxy(8, 8, "|����|      |����|      |��ע|               |�绰|              |����|                          |����|   ");
	outtextxy(750, 400, "����ESC���˳�");
	outtextxy(750, 440, "����Space����ҳ");
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
	int i = 10, j = 50; int t = 0; int datanum = 1;//��Ϊman[t]�޷���ʾ�����ڵ����򣬷�ֹҳ����ʾ���ң�ʹ��datanum��¼�����ڸ�����ϵ�˵����
	switch (k)
	{
	case 1://ѡ����ʾͬѧ����
	{
		for (t = 0 ; (t < mannum)&&flag; t++)
		{
			if ((man[t]->GetTypeNum() == 1)) {
				if ((datanum <= page * 15) && (datanum > (page - 1) * 15)) {//��������ϵ������ڹ涨��ҳ����Χ��
					string birthday = to_string(man[t]->GetYear()) + "." + to_string(man[t]->GetMonth()) + "." + to_string(man[t]->GetDay());
					outtextxy(i, j, man[t]->GetName().c_str());
					outtextxy(i + 70, j, man[t]->GetType().c_str());
					outtextxy(i + 150, j, man[t]->GetAddition().c_str());
					outtextxy(i + 260, j, man[t]->GetTel().c_str());
					outtextxy(i + 370, j, man[t]->GetEmail().c_str());
					outtextxy(i + 540, j, birthday.c_str());
					j = j + 30;
				}
				datanum++;//����Ƿ�������ż�1
			}
		}
		break;
	}
	case 2://ѡ����ʾͬ�·���
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
	case 3://ѡ����ʾ���ѷ���
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
	case 4://ѡ����ʾ���ݷ���
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
		InputBox(cmonth, 3, "������ָ���·�", NULL, NULL, 0, 0, true);
		int month = atoi(cmonth); string p = to_string(birthmonth[month - 1]);
		MessageBox(NULL, p.c_str(), "����������", MB_SYSTEMMODAL);
		return;
}

void Management::MerryBirthday()//��ʾ5����������ϵ�˲�����ף��
{
	string spage = "��" + to_string(page) + "ҳ"; 
	SYSTEMTIME nt;IMAGE Showphoto;//SYSTEMTIME�������ڻ�ȡ���ڵ�ʱ�� 
	::loadimage(&Showphoto, "D:/��ѧϰ/����ͨѶ¼����ϵͳ/image/Showphoto.jpg", Window::Width(), Window::Height());
	putimage(0, 0, &Showphoto);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);//���ñ�����ɫ����ʽ
	outtextxy(8, 8, "|����|      |����|      |��ע|               |�绰|              |����|                          |����|   ");
	outtextxy(750, 400, "����ESC���ص����˵�");
	outtextxy(750, 440, "����Space����ҳ");
	outtextxy(430, 600, spage.c_str());//��ʾҳ��
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
	GetLocalTime(&nt);//��ȡ��ǰʱ��
	int i = 10, j = 50, datanum = 0; int nyear = nt.wYear; int nmonth = nt.wMonth; int nday = nt.wDay; pagenum = 0;
	for (int k = 0; k < mannum; k++) {
		int dyear = man[k]->GetYear(); int dmonth = man[k]->GetMonth(); int dday = man[k]->GetDay(); int diff = DayDiff( nmonth, nday,dmonth, dday);//DayDiff�������ڻ�ȡĿ���·�����������ʱ�����
		if (diff <= 5 && diff >= 0) //������������
		{
			datanum++;
			if (datanum % 15 == 1)pagenum++;
			string week[7] = {"������","������", "����һ","���ڶ�","������","������","������"};//��string���鴢�����ڣ��������
			if ((datanum <= page * 15) && (datanum > (page - 1) * 15)) 
			{
				string birthday = to_string(man[k]->GetMonth()) + "��" + to_string(man[k]->GetDay()) + "��" 
				+ "(" + week[CalculateWeekDay(dyear, dmonth, dday) - 1] + ")";//CalculateWeekDay�������ڼ������ڼ�
				outtextxy(i, j, man[k]->GetName().c_str());
				outtextxy(i + 70, j, man[k]->GetType().c_str());
				outtextxy(i + 150, j, man[k]->GetAddition().c_str());
				outtextxy(i + 260, j, man[k]->GetTel().c_str());
				outtextxy(i + 370, j, man[k]->GetEmail().c_str());
				outtextxy(i + 540, j, birthday.c_str());
				j = j + 30;
				WriteWish(man[k]);//д���˵�ף��
			}
		}
	}
}

void Management::WriteWish(Person*m)
{
	ofstream file; string filebrowse = "D:/��ѧϰ/����ͨѶ¼����ϵͳ/" + Admin + "/����ף��"; string filename = "D:/��ѧϰ/����ͨѶ¼����ϵͳ/" + Admin + "/����ף��/��" + (m->GetName()) + ".txt";//ָ���ļ�������ļ���
	if (!filesystem::is_directory(filebrowse)) {
		filesystem::create_directory(filebrowse);
	}
	file.open(filename, ios::out | ios::ate);//ָ���ļ�������ղ��������루��ʾ��ѭ��������׷�ӣ�
	if (!file.is_open()) {
		return;
	}
	string end = "                                   " + Adminname;
	file << m->GetName() << ":" << endl;
	file << "           ף���տ��֣������Ҹ���" << endl;
	file << end << endl;
	file.close();
	return;
}

int Management::CalculateWeekDay(int y, int m, int d)
{
	if (m == 1 || m == 2) //��һ�ºͶ��»������һ���ʮ���º�ʮ����  
	{
		m += 12;
		y--;
	}
	int Week = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7;//��ķ����ɭ���㹫ʽ
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
