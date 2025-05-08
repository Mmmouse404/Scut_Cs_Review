#include "BasicX.h"

BasicX::BasicX(int x, int y, int w, int h)
{
	m_x = x; m_y = y; m_w = w; m_h = h;
}

int BasicX::Getw()
{
	return m_w;
}

int BasicX::Geth()
{
	return m_h;
}

int BasicX::Getx()
{
	return m_x;
}

int BasicX::Gety()
{
	return m_y;
}

void BasicX::SetSize(int w, int h)
{
	m_w = w; m_h = h;
}

void BasicX::Move(int x, int y)
{
	m_x = x; m_y = y;
}
