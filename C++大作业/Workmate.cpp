#include "Workmate.h"

Workmate::Workmate(string n, string t, string e, string c, int y, int m, int d, int ty):Person(n,t,e,y,m,d,ty),company(c)
{
}

string Workmate::GetCompany() const
{
    return string(company);
}

string Workmate::GetAddition() const
{
    return string(company);
}

void Workmate::SetAddtion(char s[])
{
    company = s;
}
