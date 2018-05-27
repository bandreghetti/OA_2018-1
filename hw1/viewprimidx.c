#include <stdio.h>
#include <string.h>
#include "datatypes.c"

void gen_output_fname(char* outputfname, char* inputfname, char ext[])
{
    strcat(outputfname, inputfname);
    char* dotptr = strrchr(outputfname, '.');
    *dotptr = '\0';
    strcat(outputfname, ext);
}

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
char data_file[31];
FILE *fp;

int main(int argc, char* argv[])
{    
    if(argc > 1)
    {
        strcpy(data_file, argv[1]);
    } else {
        printf("ERROR: Missing argument.\n");
        return -1;
    }

    strcpy(input_file, "indice");
    gen_output_fname(input_file, data_file, ".ind");

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