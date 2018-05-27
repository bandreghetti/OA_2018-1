#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
    strcpy(secinvlistfile, "indiceseclist");
    gen_idx_fname(primidxfile, datafile, ".ind");
    gen_idx_fname(secidxfile, datafile, ".ind");
    gen_idx_fname(secinvlistfile, datafile, ".ind");

    Reg newreg;

    printf("Adding student to file %s\n\n", datafile);
    printf("MATRIC: ");
    scanf("%6s", newreg.matric);
    while(getchar() != '\n');
    printf("NAME: ");
    scanf("%40[^\n]s", newreg.name);
    int i = 0;
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
    printf("OP: ");
    scanf("%2s", newreg.op);
    while(getchar() != '\n');
    printf("COURSE: ");
    scanf("%2s", newreg.course);
    while(getchar() != '\n');
    printf("CLASS: ");
    scanf("%c", &newreg.class[0]);
    while(getchar() != '\n');

    unsigned short newrrn = fsize(datafile)/sizeof(Reg);
    datafp = fopen(datafile, "a");
    fwrite(&newreg, sizeof(Reg), 1, datafp);
    fclose(datafp);

    /*Adding data to primary index*/
    unsigned long int primidxsize = (fsize(primidxfile)/sizeof(Primidx))+1;
    Primidx* primidxvec = (Primidx*)malloc(primidxsize*sizeof(Primidx));
    primidxfp = fopen(primidxfile, "r");
    fread(primidxvec, sizeof(Primidx), primidxsize-1, primidxfp);
    fclose(primidxfp);

    char newprimkey[31];
    gen_primkey(newprimkey, newreg);

    strcpy(primidxvec[primidxsize-1].primkey, newprimkey);
    primidxvec[primidxsize-1].rrn = newrrn;

    heapsort_prim_idx(primidxvec, primidxsize);

    primidxfp = fopen(primidxfile, "w");
    fwrite(primidxvec, sizeof(Primidx), primidxsize, primidxfp);
    fclose(primidxfp);

    /*Adding data to secondary index*/
    unsigned int secinvlistsize = fsize(secinvlistfile)/sizeof(SecInvList);
    unsigned int n_courses = fsize(secidxfile)/sizeof(Secidx);

    Secidx* secidxvec = (Secidx*)malloc(n_courses*sizeof(Secidx)+1);
    secidxfp = fopen(secidxfile, "r");
    fread(secidxvec, sizeof(Secidx), n_courses, secidxfp);
    fclose(secidxfp);
    
    int invlisthead = -1;
    i = 0;
    int courseidx = n_courses;;
    while(i < n_courses)
    {
        if(strcmp(newreg.course, secidxvec[i].course) == 0)
        {
            invlisthead = secidxvec[i].head;
            courseidx = i;
            break;
        }
        ++i;
    }

    SecInvList* secinvlistvec = (SecInvList*)malloc((secinvlistsize+1)*sizeof(SecInvList));
    secinvlistfp = fopen(secinvlistfile, "r");
    fread(secinvlistvec, sizeof(SecInvList), secinvlistsize, secinvlistfp);
    int newinvlistpos = secinvlistsize;
    i = 0;
    while(i < secinvlistsize)
    {
        if(secinvlistvec[i].primkey[0] == '*') /*If reusable slot is found*/
        {
            newinvlistpos = i;
            break;
        }
        ++i;
    }
    strcpy(secinvlistvec[newinvlistpos].primkey, newprimkey);
    secinvlistvec[newinvlistpos].next = -1;
    fclose(secinvlistfp);

    /*If new course is added*/
    if(invlisthead == -1)
    {
        strcpy(secidxvec[n_courses].course, newreg.course);
        secidxvec[n_courses].head = newinvlistpos;
        heapsort_sec_idx(secidxvec, n_courses+1);
        secidxfp = fopen(secidxfile, "w");
        fwrite(secidxvec, sizeof(Secidx), n_courses+1, secidxfp);
        fclose(secidxfp);
    } else {
        printf("InvListHead = %d\n", invlisthead);
        int k = invlisthead;
        int isfirst = 0;
        if(strcmp(secinvlistvec[k].primkey, newprimkey))
        {
            secinvlistvec[newinvlistpos].next = invlisthead;
            secidxvec[courseidx].head = newinvlistpos;
            isfirst = 1;
        }
        while(secinvlistvec[k].next != -1 && !isfirst)
        {
            if(strcmp(secinvlistvec[k].primkey, newprimkey) < 0 &&
               strcmp(secinvlistvec[secinvlistvec[k].next].primkey, newprimkey) >= 0)
            {
                break;
            } else {
                k = secinvlistvec[k].next;
            }
        }
        if(!isfirst)
        {
            secinvlistvec[newinvlistpos].next = secinvlistvec[k].next;
            secinvlistvec[k].next = newinvlistpos;
        }
        secinvlistfp = fopen(secinvlistfile, "w");
        if(newinvlistpos == secinvlistsize)
        {
            fwrite(secinvlistvec, sizeof(SecInvList), secinvlistsize+1, secinvlistfp);
        } else {
            fwrite(secinvlistvec, sizeof(SecInvList), secinvlistsize, secinvlistfp);
        }
    }

    return 0;
}