#include "Friend.h"

Friend::Friend(string n, string t, string e, string mp, int y, int m, int d, int ty) :Person(n, t, e, y, m, d, ty), meetplace(mp)
{
}

string Friend::GetMeetPlace() const
{
    return string(meetplace);
}
string Friend::GetAddition() const {
    return meetplace;
}

void Friend::SetAddtion(char s[])
{
    meetplace = s;
}
