#include <string>
#include <iostream>
#include "classdefs.cpp"

int main()
{
    Student student("Carlos Dias Takase                       62364  EM  A\n");

    cout << student.primkey() << endl;
    cout << student.name << endl;
    cout << student.id << endl;
    cout << student.course << endl;
    cout << student.classroom << endl;

    return 0;
}