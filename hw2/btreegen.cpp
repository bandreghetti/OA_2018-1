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

    BTree btree(5);
    
    Student* student;

    int i;
    for(i = 0; i < 4; ++i)
    {
        printf("Inserting student #%d\n", i);
        line = src.getline();
        student = new Student(line);   
        btree.insert(student->primkey(), i);
    }

    btree.show();

    src.close();

    return 0;
}