#pragma once
#include "Person.h"
class Schoolmate :public Person  //继承Person类
{
public:
    Schoolmate() {};//默认构造函数
    Schoolmate(string n, string t, string e, string s, int y, int m, int d, int ty);//构造函数
    string GetSchool()const;//获取备注：一同上学的学校
    string GetAddition()const;//根据类别，获取不同备注（这里为学校）
    void SetAddtion(char s[]);//设置学校
private:
    string school;
};
