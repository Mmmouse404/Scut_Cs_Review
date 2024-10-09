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
	}; //ö�����ͣ���������жϣ�ֱ��
	enum Operator2 {
		show1,
		show2,
		show3,
		show4,
		showbirth,
		givebirth
	};//ö�����ͣ��Ҳఴť�Ķ�Ӧ����

    bool Log();//��½����
	void Run();//����������
	int Menu();//�˵�
	void Show();//1.��ʾ������ϵ����Ϣ
	void Add();//2.������ϵ��
	void Erase();//3.ɾ����ϵ��
	void Modify();//4.�޸���Ϣ
	void Search();//5.������Ϣ
	void Sort();//6.����
	void Sshow(int i);//7.���շ�����ʾ��ϵ��
	void MerryBirthday();//8.��ʾ�������������ϵ�˲�����ף��

	void DrawBackground()//���Ʊ���
	{
		::putimage(0, 0, &bk);
	}
	void DrawDigitalClock(int year,int month,int day,int dayofweek,int hour, int minute, int second);//ʱ����ʾ��
	//��Person������ӣ�
	Person* man[200];
	int mannum=0;//��ϵ��������
	void Insert( string Name, string Phone, string Email, string Addition,int Year, int Month, int Day ,int Type);//������Ϣ
	void ReadFile();//���ļ����������
	void SaveFile();//д�ļ�����
	bool Mystrcmp(const char a[], char b[]);//�Զ�����ַ����Ƚ�
	bool Delete(char d[]);//ɾ������ָ���ַ������û�
    void DeleteAll();//�����ϵ��
	void ShowBirthNnum();//��ʾ�����ڸ����·��ڵ���ϵ��
	void WriteWish(Person*m);//��ָ������ϵ��д����ף��
	int CalculateWeekDay(int y, int m, int d);//���������ڼ�
	int DayDiff( int smonth, int sday,  int emonth, int eday);//�����������ڣ���������ݣ��Ĳ��
	//��Ϣͳ��
	int birthmonth[12];//�ֱ�洢ÿ�������յ�����
	int group[4];//�ֱ�洢����������
	int flag = 1;//����Menu���ж���ʾ�����Ƿ������ʾ
	int page = 1; int pagenum=0;//���ڷ�ҳ����ǰҳ������ҳ��
	string Admin; string Password; string Adminname;//�û�����xx��ͨѶ¼�������룬�û��ļ��еľ���·��
private:
	IMAGE bk;//����ͼƬ����
	ExMessage menu_msg;//�˵�����Ϣ����ȡ����������Ϣ���ж�
	std::vector<Button*>m_button;//�˵���ť1�������
	std::vector<Button*>s_button;//�˵���ť2�����Ҳ�
};

