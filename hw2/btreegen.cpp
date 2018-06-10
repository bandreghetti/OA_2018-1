#include <string>
#include <iostream>
#include "classdefs.cpp"

int main()
{
    Student student("Bruno Dantas", "12345", "EM", 'A');

    cout << student.primkey() << endl;
    cout << student.name << endl;
    cout << student.id << endl;
    cout << student.course << endl;
    cout << student.classroom << endl;

    return 0;
}