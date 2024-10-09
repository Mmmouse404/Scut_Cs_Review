#pragma once
#include "BasicX.h"
#include"Configure.h"
class Button : public BasicX
{
public:
	Button(const std::string& text,int x=0, int y=0, int w = 100, int h = 30);//构造函数
	void Show();//按钮显示
	bool IsOnButton();//判定是否在按钮上
	bool IsClicked();//判断按钮是否被鼠标点击
	void Loop(const ExMessage& msg);//循环，用于在整个过程中获取信息
	void SetNormalColor(COLORREF c);//设置普通颜色
	void SetOnColor(COLORREF c);//设置覆盖时的颜色
protected:
	std::string m_text;//按钮上的文字
	ExMessage m_msg;//信息
	COLORREF normal_color = RGB(232, 232, 236);//普通颜色
	COLORREF on_color = RGB(138, 255, 158);//鼠标在上面时的颜色
	COLORREF color = RGB(232, 232, 236);
};

