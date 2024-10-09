#include "Relative.h"

string Relative::GetRelation() const
{
    return string(relation);
}

Relative::Relative(string n, string t, string e, string r, int y, int m, int d, int ty):Person(n,t,e,y,m,d,ty),relation(r)
{
}

string Relative::GetAddition() const
{
    return string(relation);
}

void Relative::SetAddtion(char s[])
{
    relation = s;
}
