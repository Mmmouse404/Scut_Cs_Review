#pragma once
#include "Person.h"
class Friend :
    public Person
{
public:
    Friend() {}
    Friend(string n, string t, string e, string mp,int y, int m, int d, int ty);
    string GetMeetPlace()const;
    string GetAddition()const;
    void SetAddtion(char s[]);
private:
    string meetplace;
};

