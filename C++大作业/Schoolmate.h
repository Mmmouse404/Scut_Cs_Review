#pragma once
#include "Person.h"
class Schoolmate :public Person  //�̳�Person��
{
public:
    Schoolmate() {};//Ĭ�Ϲ��캯��
    Schoolmate(string n, string t, string e, string s, int y, int m, int d, int ty);//���캯��
    string GetSchool()const;//��ȡ��ע��һͬ��ѧ��ѧУ
    string GetAddition()const;//������𣬻�ȡ��ͬ��ע������ΪѧУ��
    void SetAddtion(char s[]);//����ѧУ
private:
    string school;
};
