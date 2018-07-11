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
    for(i = 0; i < 15; ++i)
    {
        line = src.getline(i);
        student = new Student(line);   
        // printf("Inserting student #%d with name %s key %s\n", i, student->name.c_str(), student->primkey().c_str());
        btree.insert(student->primkey(), i);
    }

    btree.show();

    src.close();

    return 0;
}