#pragma once
#include "Person.h"
class Workmate :
    public Person
{
public:
    Workmate() {};
    Workmate(string n, string t, string e, string c, int y, int m, int d, int ty);
    string GetCompany()const;
    string GetAddition()const;
    void SetAddtion(char s[]);
private:
    string company;
};

