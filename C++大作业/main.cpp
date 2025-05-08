#include"Management.h"
#include"Window.h"
int main() {
	Window w(960,640,EX_SHOWCONSOLE);//显示窗口
	w.SetWindowTitle("个人通讯录管理系统");
	Management m;
	while (1) {
		if (m.Log() ) {
			m.Run();
			w.exec();//保持运行窗口
		}
	}
}