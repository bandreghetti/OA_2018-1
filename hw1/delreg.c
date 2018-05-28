#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "datatypes.c"

char datafile[31];
char primidxfile[40];
char secidxfile[40];
char secinvlistfile[40];

FILE *datafp, *primidxfp, *secidxfp, *secinvlistfp;

void gen_idx_fname(char* outputfname, char* inputfname, char ext[])
{
    strcat(outputfname, inputfname);
    char* dotptr = strrchr(outputfname, '.');
    *dotptr = '\0';
    strcat(outputfname, ext);
}

void gen_primkey(char primkey[], Reg reg)
{
    strcpy(primkey, reg.matric);
    char* srcptr = reg.name;
    int dstidx = 6;
    while(*srcptr && dstidx < 30)
    {
        if(*srcptr != ' ')
        {
            primkey[dstidx] = toupper(*srcptr);
            ++dstidx;
        }
        ++srcptr;
    }
    while(dstidx < 30)
    {
        primkey[dstidx] = ' ';
        ++dstidx;
    }
    primkey[dstidx] = '\0';
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

int main(int argc, char* argv[])
{
    if(argc > 1)
    {
        strcpy(datafile, argv[1]);
    } else {
        printf("ERROR: Missing argument.\n");
        return -1;
    }

    strcpy(primidxfile, "indice");
    strcpy(secidxfile, "indicesec");
    strcpy(secinvlistfile, "indicesecinv");
    gen_idx_fname(primidxfile, datafile, ".ind");
    gen_idx_fname(secidxfile, datafile, ".ind");
    gen_idx_fname(secinvlistfile, datafile, ".ind");

    Reg reg;
    printf("Adding student to file %s\n\n", datafile);
    printf("MATRIC: ");
    scanf("%6s", reg.matric);
    while(getchar() != '\n');
    printf("NAME: ");
    scanf("%40[^\n]s", reg.name);
    int i = 0;
    while(i < 40)
    {
        if(reg.name[i] == '\0')
        {
            reg.name[i] = ' ';
            reg.name[i+1] = '\0';
        }
        ++i;
    }
    while(getchar() != '\n');

    char remprimkey[31];
    unsigned short remrrn;
    gen_primkey(remprimkey, reg);

    /*Removing register from primary index*/
    unsigned long int primidxsize = fsize(primidxfile)/sizeof(Primidx);
    Primidx primidx;
    
    primidxfp = fopen(primidxfile, "r+");
    i = 0;
    while(i < primidxsize)
    {
        fread(&primidx, sizeof(Primidx), 1, primidxfp);
        if(primidx.primkey[0] != '*')
        {
            if(strcmp(primidx.primkey, remprimkey) == 0)
            {
                remrrn = primidx.rrn;
                fseek(primidxfp, -1*sizeof(Primidx), SEEK_CUR);
                fputc('*', primidxfp);
                break;
            }
        }
        ++i;
    }
    fclose(primidxfp);
    if(i == primidxsize)
    {
        printf("Register not found.\n");
        return -1;
    } else {
        printf("Removed register from primary index\n");
    }

    /*Removing register from data file*/
    Reg remreg;
    datafp = fopen(datafile, "r+");
    fseek(datafp, remrrn*sizeof(Reg), SEEK_SET);
    fread(&remreg, sizeof(Reg), 1, datafp);
    char remcourse[3];
    strcpy(remcourse, remreg.course);
    fseek(datafp, remrrn*sizeof(Reg), SEEK_SET);
    fputc('*', datafp);
    fclose(datafp);
    printf("Removed register from data file\n");

    /*Removing register from secondary index*/
    unsigned long secidxsize = fsize(secidxfile)/sizeof(Secidx);
    unsigned long secinvlistsize = fsize(secinvlistfile)/sizeof(SecInvList);
    Secidx* secidxvec = malloc(secidxsize*sizeof(Secidx));
    SecInvList* secinvlistvec = malloc(secinvlistsize*sizeof(SecInvList));
    secidxfp = fopen(secidxfile, "r+");
    secinvlistfp = fopen(secinvlistfile, "r+");
    fread(secidxvec, sizeof(Secidx), secidxsize, secidxfp);
    fread(secinvlistvec, sizeof(SecInvList), secinvlistsize, secinvlistfp);
    rewind(secidxfp);
    rewind(secinvlistfp);
    i = 0;
    int head;
    while(i < secidxsize)
    {
        if(strcmp(secidxvec[i].course, remcourse) == 0)
        {
            head = secidxvec[i].head;
            break;
        }
        ++i;
    }
    int prevhead;
    if(strcmp(secinvlistvec[head].primkey, remprimkey) == 0)
    {
        secidxvec[i].head = secinvlistvec[head].next;
        secinvlistvec[head].primkey[0] = '*';
        fwrite(secidxvec, sizeof(Secidx), secidxsize, secidxfp);
        fwrite(secinvlistvec, sizeof(SecInvList), secinvlistsize, secinvlistfp);
        head = -1;
    } else {
        prevhead = head;
        head = secinvlistvec[head].next;
    }
    fclose(secidxfp);
    while(head != -1)
    {
        if(strcmp(remprimkey, secinvlistvec[head].primkey) == 0)
        {
            secinvlistvec[prevhead].next = secinvlistvec[head].next;
            secinvlistvec[head].primkey[0] = '*';
            fwrite(secinvlistvec, sizeof(SecInvList), secinvlistsize, secinvlistfp);
            break;
        } else {
            prevhead = head;
            head = secinvlistvec[head].next;
        }
    }
    fclose(secinvlistfp);
    printf("Removed register from secondary index\n");

    return 0;
}