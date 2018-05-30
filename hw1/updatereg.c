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

void heapsort_prim_idx(Primidx* primidxvec, int nregs)
{
    int i = nregs/2;
    int parentidx, childidx;
    Primidx tmpreg;
    while(1)
    {
        if (i > 0) {
            --i;
            tmpreg = primidxvec[i];
        } else {
            --nregs;
            if(!nregs)
            {
                return;
            }
            tmpreg = primidxvec[nregs];
            primidxvec[nregs] = primidxvec[0];
        }
        
        parentidx = i;
        childidx = 2*i + 1;
        while(childidx < nregs)
        {
            if((childidx + 1 < nregs) && (strcmp(primidxvec[childidx + 1].primkey, primidxvec[childidx].primkey) > 0))
            {
                ++childidx;
            }
            if(strcmp(primidxvec[childidx].primkey, tmpreg.primkey) > 0)
            {
                primidxvec[parentidx] = primidxvec[childidx];
                parentidx = childidx;
                childidx = 2*parentidx + 1;
            } else {
                break;
            }
        }
        primidxvec[parentidx] = tmpreg;
    }
}

void heapsort_sec_idx(Secidx* secidxvec, int ncourses)
{
    int i = ncourses/2;
    int parentidx, childidx;
    Secidx tmpreg;
    while(1)
    {
        if (i > 0) {
            --i;
            tmpreg = secidxvec[i];
        } else {
            --ncourses;
            if(!ncourses)
            {
                return;
            }
            tmpreg = secidxvec[ncourses];
            secidxvec[ncourses] = secidxvec[0];
        }
        
        parentidx = i;
        childidx = 2*i + 1;
        while(childidx < ncourses)
        {
            if((childidx + 1 < ncourses) && (strcmp(secidxvec[childidx + 1].course, secidxvec[childidx].course) > 0))
            {
                ++childidx;
            }
            if(strcmp(secidxvec[childidx].course, tmpreg.course) > 0)
            {
                secidxvec[parentidx] = secidxvec[childidx];
                parentidx = childidx;
                childidx = 2*parentidx + 1;
            } else {
                break;
            }
        }
        secidxvec[parentidx] = tmpreg;
    }
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

    char primkey[31], newprimkey[31], oldcourse[3];
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
                break;
            }
        }
        ++i;
    }
    fclose(primidxfp);
    if(i == primidxsize)
    {
        printf("Student not found.\n");
        return -1;
    }
    
    datafp = fopen(datafile, "r+");
    fseek(datafp, regrrn*sizeof(Reg), SEEK_SET);
    fread(&reg, sizeof(Reg), 1, datafp);
    fseek(datafp, -1*sizeof(Reg), SEEK_CUR);    
    strcpy(oldcourse, reg.course);

    printf("What field do you want to update?\n");
    printf("1) MATRIC\n");
    printf("2) NAME\n");
    printf("3) OP\n");
    printf("4) COURSE\n");
    printf("5) CLASS\n");

    Reg newreg = reg;
    int input;
    do{
        printf("Option: ");
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
            scanf("%2s", newreg.course);
            while(getchar() != '\n');
            break;
        case 5:
            printf("New CLASS: ");
            scanf("%c", &(newreg.class[0]));
            while(getchar() != '\n');
            break;
    }

    fseek(datafp, regrrn*sizeof(Reg), SEEK_SET);
    fwrite(&newreg, sizeof(Reg), 1, datafp);
    fclose(datafp);

    /*If MATRIC or NAME fields are modified, primary key changes*/
    Primidx newprimidx;
    gen_primkey(newprimkey, newreg);
    if(input == 1 || input == 2)
    {
        strcpy(newprimidx.primkey, newprimkey);
        newprimidx.rrn = regrrn;
        unsigned long primidxsize = fsize(primidxfile);
        Primidx* primidxvec = (Primidx*)malloc(primidxsize);
        primidxfp = fopen(primidxfile, "r+");
        fread(primidxvec, primidxsize, 1, primidxfp);
        primidxvec[primidxrrn] = newprimidx;
        heapsort_prim_idx(primidxvec, primidxsize/sizeof(Primidx));
        rewind(primidxfp);
        fwrite(primidxvec, primidxsize, 1, primidxfp);
    }
    fclose(primidxfp);
   
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
            if(strcmp(secidxvec[i].course, oldcourse) == 0)
            {
                ptr = secidxvec[i].head;
                head = ptr;
                courseidx = i;
                break;
            }
            ++i;
        }
        if(i != secidxsize)
        {
            if(strcmp(secinvlistvec[head].primkey, primkey) == 0)
            {
                secidxvec[i].head = secinvlistvec[head].next;
                newsecrrn = head;
                head = secidxvec[i].head;
                strcpy(secinvlistvec[newsecrrn].primkey, newprimkey);
                secinvlistvec[newsecrrn].next = -1;
                ptr = -1;
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
    }

    if(input == 1 || input == 2)
    {
        if(strcmp(secinvlistvec[secidxvec[courseidx].head].primkey, newprimkey) >= 0)
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
            secidxvec = realloc(secidxvec, (secidxsize+1)*sizeof(Secidx));
            strcpy(secidxvec[secidxsize].course, newreg.course);
            secidxvec[secidxsize].head = newsecrrn;
            heapsort_sec_idx(secidxvec, secidxsize+1);
            if(secidxfp != NULL) fclose(secidxfp);
            secidxfp = fopen(secidxfile, "w");
            fwrite(secidxvec, sizeof(Secidx), secidxsize+1, secidxfp);
            fclose(secidxfp);
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
        if(secidxfp != NULL)
        {
            rewind(secidxfp);
            fwrite(secidxvec, sizeof(Secidx), secidxsize, secidxfp);
            fclose(secidxfp);
        }
        rewind(secinvlistfp);
        fwrite(secinvlistvec, sizeof(SecInvList), secinvlistsize, secinvlistfp);
        fclose(secinvlistfp);
    }

    return 0;
}
