#pragma once
#include"Configure.h"
#include<easyx.h>
#include<string>
class Window
{
public:
	Window(int w, int h, int flag);
	void SetWindowTitle(const std::string& title);//���ô��ڱ���
	int exec();//����ʱ���ֹ��
	static int Width();//��ȡ���
	static int Height();//��ȡ�߶�
	static void Clear();//�����Ļ
	static void BeginDraw();//��ʼ����
	static void FlushDraw();//������ͼ��չ�֣���BeginDraw���÷�ֹ����
	static void EndDraw();//��������
	inline static bool HasMsg() { return ::peekmessage(&m_msg, EX_MOUSE | EX_KEY); }//�õ���������Ϣ�������ô���
	inline static const ExMessage& GetMsg() { return m_msg; }//�����Ϣ

private:
	HWND m_handle;//���ھ��
    static ExMessage m_msg;//��Ϣ
};

