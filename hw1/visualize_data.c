#include <stdio.h>
#include <string.h>

typedef struct reg {
    char matric[7];
    char name[41];
    char op[3];
    char course[3];
    char class[2];
    char primkey[31];
} Reg;

unsigned long int fsize(char* fname)
{
    FILE* fp;
    fp = fopen(fname, "r");
    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);
    fclose(fp);
    return size;
}

char input_file[31];
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

    int filesize = fsize(input_file);

    fp = fopen(input_file, "r");

    int n_regs = filesize/sizeof(Reg);

    Reg reg; 
    int i;
    printf("RRN|MATRIC|NOME                                    |OP|CURSO   |TURMA\n");
    for(i = 0; i < n_regs; ++i)
    {
        fread(&reg, sizeof(Reg), 1, fp);
        printf("%03d|%s|%s|%s|%-8s|%s\n", i, reg.matric, reg.name, reg.op, reg.course, reg.class);
    }

    return 0;
}