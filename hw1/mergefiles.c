#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datatypes.c"

typedef struct courselistelem {
    char course[3];
    struct courselistelem* next;
} CourseListElem;

typedef struct courselist {
    CourseListElem* first;
    CourseListElem* last;
    unsigned int size;
} CourseList;

CourseList* CourseListCreate()
{
   CourseList* l = (CourseList*)malloc(sizeof(CourseList));
   l->first = NULL;
   l->last = NULL;
   return l;
}

void appendToCourseList(char* course, CourseList* l)
{
    CourseListElem* newelem = (CourseListElem*)malloc(sizeof(CourseListElem));
    strcpy(newelem->course, course);
    newelem->next = l->first;
    l->first = newelem;
    if(l->last == NULL){
        l->last = newelem;
    }
    ++(l->size);
}

int isInList(char* course, CourseList* l)
{
    CourseListElem* elem = l->first;
    while(elem != NULL)
    {
        if(strcmp(elem->course, course) == 0)
        {
            return 1;
        }
        elem = elem->next;
    }
    return 0;
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

char datafile0[31];
char primidxfile0[40];
char secidxfile0[40];
char secinvlistfile0[40];

FILE *datafp0, *primidxfp0, *secidxfp0, *secinvlistfp0;

char datafile1[31];
char primidxfile1[40];
char secidxfile1[40];
char secinvlistfile1[40];

FILE *datafp1, *primidxfp1, *secidxfp1, *secinvlistfp1;

char datafileout[] = "lista12.data";
char primidxfileout[40];
char secidxfileout[40];
char secinvlistfileout[40];

FILE *datafpout, *primidxfpout, *secidxfpout, *secinvlistfpout;

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
    if(argc > 2)
    {
        strcpy(datafile0, argv[1]);
        strcpy(datafile1, argv[2]);
    } else {
        printf("ERROR: Missing argument(s).\n");
        return -1;
    }

    strcpy(primidxfile0, "indice");
    strcpy(secidxfile0, "indicesec");
    strcpy(secinvlistfile0, "indicesecinv");
    gen_idx_fname(primidxfile0, datafile0, ".ind");
    gen_idx_fname(secidxfile0, datafile0, ".ind");
    gen_idx_fname(secinvlistfile0, datafile0, ".ind");

    strcpy(primidxfile1, "indice");
    strcpy(secidxfile1, "indicesec");
    strcpy(secinvlistfile1, "indicesecinv");
    gen_idx_fname(primidxfile1, datafile1, ".ind");
    gen_idx_fname(secidxfile1, datafile1, ".ind");
    gen_idx_fname(secinvlistfile1, datafile1, ".ind");

    strcpy(primidxfileout, "indice");
    strcpy(secidxfileout, "indicesec");
    strcpy(secinvlistfileout, "indicesecinv");
    gen_idx_fname(primidxfileout, datafileout, ".ind");
    gen_idx_fname(secidxfileout, datafileout, ".ind");
    gen_idx_fname(secinvlistfileout, datafileout, ".ind");

    /*Merging primary indexes and data files*/

    primidxfp0 = fopen(primidxfile0, "r");
    primidxfp1 = fopen(primidxfile1, "r");
    primidxfpout = fopen(primidxfileout, "w");
    datafp0 = fopen(datafile0, "r");
    datafp1 = fopen(datafile1, "r");
    datafpout = fopen(datafileout, "w");

    Primidx primidx0, primidx1;
    Reg tmpreg;
    fread(&primidx0, sizeof(Primidx), 1, primidxfp0);
    fread(&primidx1, sizeof(Primidx), 1, primidxfp1);

    CourseList* courselist = CourseListCreate();

    int dataoutidx = 0;
    while(!feof(primidxfp0) || !feof(primidxfp1))
    {
        while(primidx0.primkey[0] == '*' && !feof(primidxfp0))
        {
            fread(&primidx0, sizeof(Primidx), 1, primidxfp0);
        }
        while(primidx1.primkey[0] == '*' && !feof(primidxfp1))
        {
            fread(&primidx1, sizeof(Primidx), 1, primidxfp1);
        }
        if(feof(primidxfp0))
        {
            while(!feof(primidxfp1))
            {
                fseek(datafp1, primidx1.rrn*sizeof(Reg), SEEK_SET);
                primidx1.rrn = dataoutidx;
                dataoutidx++;
                fread(&tmpreg, sizeof(Reg), 1, datafp1);
                if(!isInList(tmpreg.course, courselist))
                {
                    appendToCourseList(tmpreg.course, courselist);
                }
                fwrite(&tmpreg, sizeof(Reg), 1, datafpout);
                fwrite(&primidx1, sizeof(Primidx), 1, primidxfpout);
                fread(&primidx1, sizeof(Primidx), 1, primidxfp1);
            }
        } else if(feof(primidxfp1)) {
            while(!feof(primidxfp0))
            {
                fseek(datafp0, primidx0.rrn*sizeof(Reg), SEEK_SET);
                primidx0.rrn = dataoutidx;
                dataoutidx++;
                fread(&tmpreg, sizeof(Reg), 1, datafp0);
                if(!isInList(tmpreg.course, courselist))
                {
                    appendToCourseList(tmpreg.course, courselist);
                }
                fwrite(&tmpreg, sizeof(Reg), 1, datafpout);
                fwrite(&primidx0, sizeof(Primidx), 1, primidxfpout);
                fread(&primidx0, sizeof(Primidx), 1, primidxfp0);
            }
        } else if(strcmp(primidx0.primkey, primidx1.primkey) < 0) {
            fseek(datafp0, primidx0.rrn*sizeof(Reg), SEEK_SET);
            primidx0.rrn = dataoutidx;
            dataoutidx++;
            fread(&tmpreg, sizeof(Reg), 1, datafp0);
            if(!isInList(tmpreg.course, courselist))
            {
                appendToCourseList(tmpreg.course, courselist);
            }
            fwrite(&tmpreg, sizeof(Reg), 1, datafpout);
            fwrite(&primidx0, sizeof(Primidx), 1, primidxfpout);
            fread(&primidx0, sizeof(Primidx), 1, primidxfp0);
        } else {
            fseek(datafp1, primidx1.rrn*sizeof(Reg), SEEK_SET);
            primidx1.rrn = dataoutidx;
            dataoutidx++;
            fread(&tmpreg, sizeof(Reg), 1, datafp1);
            if(!isInList(tmpreg.course, courselist))
            {
                appendToCourseList(tmpreg.course, courselist);
            }
            fwrite(&tmpreg, sizeof(Reg), 1, datafpout);
            fwrite(&primidx1, sizeof(Primidx), 1, primidxfpout);
            fread(&primidx1, sizeof(Primidx), 1, primidxfp1);
        }
    }

    /*Merging secondary index*/
    fclose(datafpout);
    unsigned long outsize = fsize(datafileout)/sizeof(Reg);
    Secidx* secidxvec = (Secidx*)malloc((courselist->size)*sizeof(Secidx));
    
    int i = 0;
    CourseListElem* elem;
    elem = courselist->first;
    while(elem != NULL)
    {
        strcpy(secidxvec[i].course, elem->course);
        secidxvec[i].head = -1;
        elem = elem->next;
        ++i;
    }
    heapsort_sec_idx(secidxvec, courselist->size);

    SecInvList* secinvlistvec = (SecInvList*)malloc(outsize*sizeof(SecInvList));
    datafpout = fopen(datafileout, "r");
    
    int j, head;
    i = 0;
    while(i < outsize)
    {
        fread(&tmpreg, sizeof(Reg), 1, datafpout);
        gen_primkey(secinvlistvec[i].primkey, tmpreg);
        secinvlistvec[i].next = -1;
        j = 0;
        while(j < courselist->size)
        {
            if(strcmp(secidxvec[j].course, tmpreg.course) == 0)
            {
                head = secidxvec[j].head;
                break;
            }
            ++j;
        }
        if(head == -1)
        {
            secidxvec[j].head = i;
        } else {
            while(secinvlistvec[head].next != -1)
            {
                head = secinvlistvec[head].next;
            }
            secinvlistvec[head].next = i;
        }
        ++i;
    }

    secidxfpout = fopen(secidxfileout, "w");
    fwrite(secidxvec, sizeof(Secidx), courselist->size, secidxfpout);
    fclose(secidxfpout);
    secinvlistfpout = fopen(secinvlistfileout, "w");
    fwrite(secinvlistvec, sizeof(SecInvList), outsize, secinvlistfpout);
    fclose(secinvlistfpout);

    fclose(primidxfp0);
    fclose(primidxfp1);
    fclose(primidxfpout);
    fclose(datafp0);
    fclose(datafp1);
    fclose(datafpout);

    return 0;
}