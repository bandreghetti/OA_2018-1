#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct reg {
    char matric[7];
    char name[42];
    char op[3];
    char course[3];
    char class[2];
    char primkey[31];
    unsigned short rrn;
} Reg;

typedef struct secidx {
    char course[3];
    short head;
} Secidx;

typedef struct secinvlist {
    char primkey[31];
    short next;
} SecInvList;

Reg parsereg(char* regbytes)
{
    Reg tmpreg;
    strncpy(tmpreg.matric, &regbytes[0], 6);
    tmpreg.matric[6] = '\0';
    strncpy(tmpreg.name, &regbytes[7], 41);
    tmpreg.name[41] = '\0';
    strncpy(tmpreg.op, &regbytes[48], 2);
    tmpreg.op[2] = '\0';
    strncpy(tmpreg.course, &regbytes[52], 2);
    tmpreg.course[2] = '\0';
    strncpy(tmpreg.class, &regbytes[61], 1);
    tmpreg.class[1] = '\0';
    strcpy(tmpreg.primkey, tmpreg.matric);
    char* srcptr = tmpreg.name;
    int dstidx = 6;
    while(*srcptr && dstidx < 30)
    {
        if(*srcptr != ' ')
        {
            tmpreg.primkey[dstidx] = toupper(*srcptr);
            ++dstidx;
        }
        ++srcptr;
    }
    while(dstidx < 30)
    {
        tmpreg.primkey[dstidx] = ' ';
        ++dstidx;
    }
    tmpreg.primkey[dstidx] = '\0';

    return tmpreg;
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

void gen_output_fname(char* outputfname, char* inputfname)
{
    strcat(outputfname, inputfname);
    char* dotptr = strrchr(outputfname, '.');
    *dotptr = '\0';
    strcat(outputfname, ".ind");
}

int main(int argc, char* argv[])
{
    char input_file[30];
    char output_prim_file[36];
    char output_sec_file[39];
    FILE *inputfp, *outputprimfp, *outputsecfp;
    char inputbuffer[64];
    Reg *reglist;

    strcpy(output_prim_file, "indice");
    strcpy(output_sec_file, "indicesec");

    if(argc > 1)
    {
        strcpy(input_file, argv[1]);
    } else {
        printf("ERROR: Missing argument.\n");
        return -1;
    }

    gen_output_fname(output_prim_file, input_file);
    gen_output_fname(output_sec_file, input_file);

    int nregs = fsize(input_file)/64;

    inputfp = fopen(input_file, "r");
    outputprimfp = fopen(output_prim_file, "w");
    outputsecfp = fopen(output_sec_file, "w");

    reglist = (Reg*)malloc(nregs*sizeof(Reg));

    int i = 0;
    while(i < nregs)
    {
        fread(inputbuffer, sizeof(char), 64, inputfp);
        reglist[i] = parsereg(inputbuffer);
        reglist[i].rrn = i;
        ++i;
    }
    i = 0;
    while(i < nregs)
    {
        printf("Primary Key: %s | RRN: %d\n", reglist[i].primkey, reglist[i].rrn);
        fprintf(outputprimfp, "%s", reglist[i].primkey);
        fwrite(&reglist[i].rrn, sizeof(unsigned short), 1, outputprimfp);
        ++i;
    }

    fclose(inputfp);
    fclose(outputprimfp);
    fclose(outputsecfp);

    return 0;
}