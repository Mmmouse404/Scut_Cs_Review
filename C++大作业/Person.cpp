#include "Person.h"

Person::Person(string n, string t, string e, int y, int m, int d, int ty):name(n),tel(t),email(e),year(y),month(m),day(d),type(ty)
{
}

string Person::GetName() const
{
	return string(name);
}

string Person::GetTel() const
{
	return string(tel);
}

string Person::GetEmail() const
{
	return string(email);
}


int Person::GetYear() const
{
	return year;
}

int Person::GetMonth() const
{
	return month;
}

int Person::GetDay() const
{
	return day;
}

string Person::GetType() const
{
	switch (type) {
	case Type_Relative:return "亲戚";
	case Type_Friend:return "朋友";
	case Type_Workmate:return "同事";
	case Type_Schoolmate:return "同学";
	}
}

int Person::GetTypeNum() const
{
	return type;
}

void Person::SetName(char s[])
{
	name = s;
}

void Person::SetTel(char s[])
{
	tel = s;
}

void Person::SetEmail(char s[])
{
	email = s;
}

void Person::SetType(int s)
{
	type = s;
}


void Person::SetBirthDay(int a, int b, int c)
{
	year = a; month = b; day = c;
}

int Person::GetBirthTime()
{
	return 372 * year + 31 * month + day;
}
