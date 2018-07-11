#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "classdefs.cpp"

int main()
{
    SourceFile src;
    string line;
    char key[9];

    printf("Key to be searched: ");
    scanf("%8s", key);
    while(getchar() != '\n');
    string keyobj(key);
    
    BTree btree;
    
    int RRN = btree.search(keyobj);
    // printf("RRN: %d\n", RRN);

    if(RRN >= 0)
    {
        src.open();
        line = src.getline(RRN);

        Student stu(line);

        stu.print();

        src.close();
    } else {
        printf("Student with key %s not found.\n", key);
    }

    return 0;
}