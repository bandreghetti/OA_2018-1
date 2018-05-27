#include <stdio.h>
#include <string.h>
#include "datatypes.c"

unsigned long int fsize(char* fname)
{
    FILE* fp;
    fp = fopen(fname, "r");
    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);
    fclose(fp);
    return size;
}

char input_file[36];
FILE *fp;

int main(int argc, char* argv[])
{    
    if(argc > 1)
    {
        strcpy(input_file, argv[1]);
    } else {
        printf("ERROR: Missing argument.\n");
        return -1;
    }

    int idxsize = fsize(input_file);

    fp = fopen(input_file, "r");

    int n_idx = idxsize/sizeof(Primidx);

    Primidx primidx; 
    int i;
    printf("Primary Key                   |RRN\n");
    for(i = 0; i < n_idx; ++i)
    {
        fread(&primidx, sizeof(Primidx), 1, fp);
        printf("%s|%03d\n", primidx.primkey, primidx.rrn);
    }

    return 0;
}