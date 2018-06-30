#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "classdefs.cpp"

int main()
{
    SourceFile src;
    string line;
    src.open();

    // while(!src.eof())
    // {
    //     line = src.getline();
    //     Student student(line);
    //     cout << student.primkey() << endl;
    //     cout << student.name << endl;
    //     cout << student.id << endl;
    //     cout << student.course << endl;
    //     cout << student.classroom << endl;
    // }

    src.close();

    return 0;
}