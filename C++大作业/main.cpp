#include"Management.h"
#include"Window.h"
int main() {
	Window w(960,640,EX_SHOWCONSOLE);//��ʾ����
	w.SetWindowTitle("����ͨѶ¼����ϵͳ");
	Management m;
	while (1) {
		if (m.Log() ) {
			m.Run();
			w.exec();//�������д���
		}
	}
}