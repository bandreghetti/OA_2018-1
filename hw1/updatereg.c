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

    Reg inputreg;
    printf("Updating student data in file %s\n\n", datafile);
    printf("Student MATRIC: ");
    scanf("%6s", inputreg.matric);
    while(getchar() != '\n');
    printf("Student NAME: ");
    scanf("%40[^\n]s", inputreg.name);
    int i = 0;
    while(i < 40)
    {
        if(inputreg.name[i] == '\0')
        {
            inputreg.name[i] = ' ';
            inputreg.name[i+1] = '\0';
        }
        ++i;
    }
    while(getchar() != '\n');

    char primkey[31], newprimkey[31], course[3];
    unsigned short primidxrrn, regrrn;
    Reg reg;
    gen_primkey(primkey, inputreg);

    /*Looking for student in primary index*/
    unsigned long int primidxsize = fsize(primidxfile)/sizeof(Primidx);
    Primidx primidx;
    
    primidxfp = fopen(primidxfile, "r+");
    i = 0;
    while(i < primidxsize)
    {
        fread(&primidx, sizeof(Primidx), 1, primidxfp);
        if(primidx.primkey[0] != '*')
        {
            if(strcmp(primidx.primkey, primkey) == 0)
            {
                primidxrrn = i;
                regrrn = primidx.rrn;
                fseek(primidxfp, -1*sizeof(Primidx), SEEK_CUR);
                break;
            }
        }
        ++i;
    }
    if(i == primidxsize)
    {
        printf("Student not found.\n");
        return -1;
    }
    
    datafp = fopen(datafile, "r+");
    fseek(datafp, regrrn*sizeof(Reg), SEEK_SET);
    fread(&reg, sizeof(Reg), 1, datafp);
    fseek(datafp, -1*sizeof(Reg), SEEK_CUR);    
    strcpy(course, reg.course);

    printf("What field do you want to update?\n");
    printf("1) MATRIC\n");
    printf("2) NAME\n");
    printf("3) OP\n");
    printf("4) COURSE\n");
    printf("5) CLASS\n");

    Reg newreg = reg;
    int input;
    do{
        scanf("%d", &input);
        while(getchar() != '\n');
        if(input < 1 || input > 5)
        {
            printf("Please choose an option from 1 to 5.\n");
        }
    } while(input < 1 || input > 5);

    switch(input)
    {
        case 1:
            printf("New MATRIC: ");
            scanf("%6s", newreg.matric);
            while(getchar() != '\n');
            break;
        case 2:
            printf("New NAME: ");
            scanf("%40[^\n]s", newreg.name);
            i = 0;
            while(i < 40)
            {
                if(newreg.name[i] == '\0')
                {
                    newreg.name[i] = ' ';
                    newreg.name[i+1] = '\0';
                }
                ++i;
            }
            while(getchar() != '\n');
            break;
        case 3:
            printf("New OP: ");
            scanf("%2s", newreg.op);
            while(getchar() != '\n');
            break;
        case 4:
            printf("New COURSE: ");
            scanf("%ss", newreg.course);
            while(getchar() != '\n');
            break;
        case 5:
            printf("New CLASS: ");
            scanf("%c", &(newreg.class[0]));
            while(getchar() != '\n');
            break;
    }

    /*If MATRIC or NAME fields are modified, primary key changes*/
    Primidx newprimidx;
    if(input == 1 || input == 2)
    {
        gen_primkey(newprimkey, newreg);
        strcpy(newprimidx.primkey, newprimkey);
        newprimidx.rrn = regrrn;
    }
    int head, prevptr, ptr, courseidx, newsecrrn;
    unsigned long secidxsize = fsize(secidxfile)/sizeof(Secidx);
    unsigned long secinvlistsize = fsize(secinvlistfile)/sizeof(SecInvList);
    secidxfp = fopen(secidxfile, "r+");
    secinvlistfp = fopen(secinvlistfile, "r+");
    Secidx* secidxvec = (Secidx*)malloc(secidxsize*sizeof(Secidx));
    SecInvList* secinvlistvec = (SecInvList*)malloc(secinvlistsize*sizeof(SecInvList));
    fread(secidxvec, sizeof(Secidx), secidxsize, secidxfp);
    fread(secinvlistvec, sizeof(SecInvList), secinvlistsize, secinvlistfp);
    if(input == 1 || input == 2 || input == 4)
    {
        i = 0;
        while(i < secidxsize)
        {
            if(strcmp(secidxvec[i].course, course) == 0)
            {
                ptr = secidxvec[i].head;
                head = ptr;
                courseidx = i;
                break;
            }
            ++i;
        }
        if(strcmp(secinvlistvec[head].primkey, primkey) == 0)
        {
            secidxvec[i].head = secinvlistvec[head].next;
            strcpy(secinvlistvec[head].primkey, newprimkey);
        } else {
            prevptr = head;
            ptr = secinvlistvec[head].next;
        }
        while(ptr != -1)
        {
            if(strcmp(secinvlistvec[ptr].primkey, primkey) == 0)
            {
                secinvlistvec[prevptr].next = secinvlistvec[ptr].next;
                strcpy(secinvlistvec[ptr].primkey, newprimkey);
                secinvlistvec[ptr].next = -1;
                newsecrrn = ptr;
                break;
            } else {
                prevptr = ptr;
                ptr = secinvlistvec[ptr].next;
            }
        }
    }
    rewind(secidxfp);
    fwrite(secidxvec, sizeof(Secidx), secidxsize, secidxfp);
    if(input == 1 || input == 2)
    {
        if(strcmp(secinvlistvec[head].primkey, newprimkey) >= 0)
        {
            secinvlistvec[newsecrrn].next = secidxvec[courseidx].head;
            secidxvec[courseidx].head = newsecrrn;
        } else {
            prevptr = head;
            ptr = secinvlistvec[head].next;
        }
        while(ptr != -1)
        {
            if(strcmp(secinvlistvec[prevptr].primkey, newprimkey) <= 0 &&
               strcmp(secinvlistvec[ptr].primkey, newprimkey) > 0)
            {
                secinvlistvec[newsecrrn].next = ptr;
                secinvlistvec[prevptr].next = newsecrrn;
                break;
            } else {
                prevptr = ptr;
                ptr = secinvlistvec[ptr].next;
            }
        }
    }
    if(input == 4)
    {
        i = 0;
        courseidx = -1;
        while(i < secidxsize)
        {
            if(strcmp(secidxvec[i].course, newreg.course) == 0)
            {
                head = secidxvec[i].head;
                courseidx = i;
                break;
            }
            ++i;
        }
        if(courseidx == -1)
        {
            Secidx newsecidx;
            strcpy(newsecidx.course, newreg.course);
            newsecidx.head = newsecrrn;
            fwrite(&newsecidx, sizeof(Secidx), 1, secidxfp);
        } else {
            if(strcmp(secinvlistvec[head].primkey, newprimkey) >= 0)
            {
                secinvlistvec[newsecrrn].next = secidxvec[courseidx].head;
                secidxvec[courseidx].head = newsecrrn;
            } else {
                prevptr = head;
                ptr = secinvlistvec[head].next;
            }
            while(ptr != -1)
            {
                if(strcmp(secinvlistvec[prevptr].primkey, newprimkey) <= 0 &&
                strcmp(secinvlistvec[ptr].primkey, newprimkey) > 0)
                {
                    secinvlistvec[newsecrrn].next = ptr;
                    secinvlistvec[prevptr].next = newsecrrn;
                    break;
                } else {
                    prevptr = ptr;
                    ptr = secinvlistvec[ptr].next;
                }
            }
        }
    }
    if(input == 1 || input == 2 || input == 4)
    {
        rewind(secinvlistfp);
        fwrite(secinvlistvec, sizeof(SecInvList), secinvlistsize, secinvlistfp);
        fclose(secinvlistfp);
        fclose(secidxfp);
    }
    fseek(datafp, regrrn*sizeof(Reg), SEEK_SET);
    fwrite(&newreg, sizeof(Reg), 1, datafp);
    fclose(datafp);

    fwrite(&newprimidx, sizeof(Primidx), 1, primidxfp);
    fclose(primidxfp);

    return 0;
}
