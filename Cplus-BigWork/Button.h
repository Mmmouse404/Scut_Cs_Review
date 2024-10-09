#pragma once
#include "BasicX.h"
#include"Configure.h"
class Button : public BasicX
{
public:
	Button(const std::string& text,int x=0, int y=0, int w = 100, int h = 30);//���캯��
	void Show();//��ť��ʾ
	bool IsOnButton();//�ж��Ƿ��ڰ�ť��
	bool IsClicked();//�жϰ�ť�Ƿ������
	void Loop(const ExMessage& msg);//ѭ�������������������л�ȡ��Ϣ
	void SetNormalColor(COLORREF c);//������ͨ��ɫ
	void SetOnColor(COLORREF c);//���ø���ʱ����ɫ
protected:
	std::string m_text;//��ť�ϵ�����
	ExMessage m_msg;//��Ϣ
	COLORREF normal_color = RGB(232, 232, 236);//��ͨ��ɫ
	COLORREF on_color = RGB(138, 255, 158);//���������ʱ����ɫ
	COLORREF color = RGB(232, 232, 236);
};

