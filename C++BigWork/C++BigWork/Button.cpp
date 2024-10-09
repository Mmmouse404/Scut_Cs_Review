#include "Button.h"

Button::Button(const std::string& text, int x, int y, int w, int h):BasicX(x,y,w,h),m_text(text)
{
}
void Button::Show()
{
	if (IsOnButton()) 
	{
		::setfillcolor(on_color);//设置填充颜色
	}
	else 
	{
		::setfillcolor(normal_color);//设置填充颜色
	}
	::settextcolor(BLACK);//设置文字颜色
	::fillroundrect(Getx(), Gety(), Getx()+Getw(), Gety() + Geth(), 10, 10);//设置按钮形状大小
	int ty = Gety() + (Geth() - textheight(m_text.c_str())) / 2;
	int tx = Getx() + (Getw() - textwidth(m_text.c_str())) / 2;
	::outtextxy(tx, ty, m_text.c_str());//显示文字于指定位置
	
}
bool Button::IsOnButton()
{
	if (m_msg.x >= Getx() && m_msg.x < Getx() + Getw() && m_msg.y >= Gety() && m_msg.y < Gety() + Geth()) 
	{
		return true;
	}
	return false;
}
bool Button::IsClicked()
{
	if (IsOnButton())
	{
		if (m_msg.message == WM_LBUTTONDOWN) 
		{
			return true;
		}
	}
	return false;
}
void Button::Loop(const ExMessage&msg)
{
	m_msg = msg;
}
void Button::SetNormalColor(COLORREF c)
{
	normal_color = c;
}
void Button::SetOnColor(COLORREF c)
{
	on_color = c;
}
