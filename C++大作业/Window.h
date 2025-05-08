#pragma once
#include"Configure.h"
#include<easyx.h>
#include<string>
class Window
{
public:
	Window(int w, int h, int flag);
	void SetWindowTitle(const std::string& title);//设置窗口标题
	int exec();//运行时候防止卡
	static int Width();//获取宽度
	static int Height();//获取高度
	static void Clear();//清除屏幕
	static void BeginDraw();//开始绘制
	static void FlushDraw();//将绘制图形展现，与BeginDraw连用防止闪屏
	static void EndDraw();//结束绘制
	inline static bool HasMsg() { return ::peekmessage(&m_msg, EX_MOUSE | EX_KEY); }//得到鼠标键盘信息，传给该窗口
	inline static const ExMessage& GetMsg() { return m_msg; }//获得信息

private:
	HWND m_handle;//窗口句柄
    static ExMessage m_msg;//消息
};

