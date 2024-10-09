#include "Window.h"
#include<iostream>
ExMessage Window::m_msg;
Window::Window(int w, int h, int flag)
{
	m_handle=::initgraph(w, h, flag);//³õÊ¼»¯º¯Êý
	::setbkmode(TRANSPARENT);
}

void Window::SetWindowTitle(const std::string& title)
{
	::SetWindowText(m_handle, title.c_str());
}

int Window::exec()
{
	return getchar();
}

int Window::Width()
{
	return ::getwidth();
}

int Window::Height()
{
	return ::getheight();
}

void Window::Clear()
{
	::cleardevice();
}

void Window::BeginDraw()
{
	::BeginBatchDraw();
}

void Window::FlushDraw()
{
	::FlushBatchDraw();
}

void Window::EndDraw()
{
	::EndBatchDraw();
}

