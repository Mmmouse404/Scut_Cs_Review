#pragma once
class BasicX
{
public:
	BasicX(int x, int y, int w, int h);
	int Getw();
	int Geth();
	int Getx();
	int Gety();
	void SetSize(int w,int h);
	void Move(int x,int y);

	virtual void Show()=0;
private:
	int m_x;
	int m_y;
	int m_w;
	int m_h;
};

