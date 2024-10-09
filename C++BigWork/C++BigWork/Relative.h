#pragma once
#include "Person.h"
class Relative :
    public Person
{
public:
    string GetRelation()const;
    Relative(string n, string t, string e, string r, int y, int m, int d, int ty);
    Relative() {};
    string GetAddition()const;
    void SetAddtion(char s[]);
private:
    string relation;
};

