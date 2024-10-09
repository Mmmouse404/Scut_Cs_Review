#include "Schoolmate.h"

Schoolmate::Schoolmate(string n, string t, string e, string s, int y, int m, int d, int ty):Person(n, t, e, y, m, d, ty),school(s)
{
}

string Schoolmate::GetSchool() const
{
	return string(school);
}

string Schoolmate::GetAddition() const
{
	return string(school);
}

void Schoolmate::SetAddtion(char s[])
{
	school = s;
}
