#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
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

Reg parsereg(char* regbytes, Primidx* primidx)
{
    Reg tmpreg;
    strncpy(tmpreg.matric, &regbytes[0], 6);
    tmpreg.matric[6] = '\0';
    strncpy(tmpreg.name, &regbytes[7], 40);
    tmpreg.name[41] = '\0';
    strncpy(tmpreg.op, &regbytes[48], 2);
    tmpreg.op[2] = '\0';
    strncpy(tmpreg.course, &regbytes[52], 2);
    tmpreg.course[2] = '\0';
    strncpy(tmpreg.class, &regbytes[61], 1);
    tmpreg.class[1] = '\0';
    gen_primkey(primidx->primkey, tmpreg); 

    return tmpreg;
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

void gen_output_fname(char* outputfname, char* inputfname, char ext[])
{
    strcat(outputfname, inputfname);
    char* dotptr = strrchr(outputfname, '.');
    *dotptr = '\0';
    strcat(outputfname, ext);
}

CourseList* courselist;
int secinvlistcount = 0;
int secidxcount = 0;

void add_secidx_elem(Secidx* secidxvec, SecInvList* secinvlistvec, Reg reg)
{
    char primkey[31];
    gen_primkey(primkey, reg);
    strcpy(secinvlistvec[secinvlistcount].primkey, primkey);

    int i = 0;
    while(i < secidxcount)
    {
        if(strcmp(reg.course, secidxvec[i].course) == 0)
        {
            int k = secidxvec[i].head;
            int isfirst = 0;
            if(strcmp(secinvlistvec[k].primkey, primkey) >= 0)
            {
                secinvlistvec[secinvlistcount].next = secidxvec[i].head;
                secidxvec[i].head = secinvlistcount;
                isfirst = 1;
            }
            while(secinvlistvec[k].next != -1 && !isfirst)
            {
                if(strcmp(secinvlistvec[k].primkey, primkey) < 0 &&
                   strcmp(secinvlistvec[secinvlistvec[k].next].primkey, primkey) >= 0)
                {
                    break;
                } else {
                    k = secinvlistvec[k].next;
                }
            }
            if(!isfirst)
            {
                secinvlistvec[secinvlistcount].next = secinvlistvec[k].next;
                secinvlistvec[k].next = secinvlistcount;
            }
            break;
        }
        ++i;
    }
    if(i == secidxcount)
    {
        strcpy(secidxvec[secidxcount].course, reg.course);
        secidxvec[secidxcount].head = secinvlistcount;
        secinvlistvec[secinvlistcount].next = -1;
        ++secidxcount;
    }
    ++secinvlistcount;
}

void print_secidx(Secidx* secidxvec, SecInvList* secinvlistvec, int n_courses)
{
    int i = 0;
    int j;
    printf("Secondary Index:\n\n");
    while(i < n_courses)
    {
        printf("Primary key for students from course: %s\n", secidxvec[i].course);
        j = secidxvec[i].head;
        while(j != -1)
        {
            printf("    %s\n", secinvlistvec[j].primkey);
            j = secinvlistvec[j].next;
        }
        ++i;
    }
}

char input_file[30];
char output_prim_file[36];
char output_sec_file[39];
char output_sec_list[43];
char output_data_file[30];

FILE *inputfp, *datafp, *outputprimfp, *outputsecfp, *outputseclistfp;

int main(int argc, char* argv[])
{
    char inputbuffer[64];
    Reg *reglist;
    Primidx *primidxvec;

    courselist = CourseListCreate();

    strcpy(output_prim_file, "indice");
    strcpy(output_sec_file, "indicesec");
    strcpy(output_sec_list, "indiceseclist");
    strcpy(output_data_file, "\0");

    if(argc > 1)
    {
        strcpy(input_file, argv[1]);
    } else {
        printf("ERROR: Missing argument.\n");
        return -1;
    }

    gen_output_fname(output_prim_file, input_file, ".ind");
    gen_output_fname(output_sec_file, input_file, ".ind");
    gen_output_fname(output_sec_list, input_file, ".ind");
    gen_output_fname(output_data_file, input_file, ".data");

    int nregs = fsize(input_file)/64;

    inputfp = fopen(input_file, "r");
    outputprimfp = fopen(output_prim_file, "w");
    outputsecfp = fopen(output_sec_file, "w");
    outputseclistfp = fopen(output_sec_list, "w");
    datafp = fopen(output_data_file, "w");

    reglist = (Reg*)malloc(nregs*sizeof(Reg));
    primidxvec = (Primidx*)malloc(nregs*sizeof(Primidx));
    int i = 0;
    while(i < nregs)
    {
        fread(inputbuffer, sizeof(char), 64, inputfp);
        reglist[i] = parsereg(inputbuffer, &primidxvec[i]);
        primidxvec[i].rrn = i;
        if(!isInList(reglist[i].course, courselist))
        {
            appendToCourseList(reglist[i].course, courselist);
        }
        ++i;
    }
    fwrite(reglist, sizeof(Reg), nregs, datafp);
    heapsort_prim_idx(primidxvec, nregs);
    fwrite(primidxvec, sizeof(Primidx), nregs, outputprimfp);

    Secidx* secidxvec = (Secidx*)malloc((courselist->size)*sizeof(Secidx));
    SecInvList* secinvlistvec = (SecInvList*)malloc(nregs*sizeof(SecInvList));

    i = 0;
    while(i < nregs)
    {
        add_secidx_elem(secidxvec, secinvlistvec, reglist[i]);
        ++i;
    }

    i = 0;
    printf("Primary Index:\n\n");
    printf("PRIMARY KEY                   |RRN\n");
    while(i < nregs)
    {
        printf("%s|%03d\n", primidxvec[i].primkey, primidxvec[i].rrn);
        ++i;
    }
    printf("\n");

    heapsort_sec_idx(secidxvec, courselist->size);

    print_secidx(secidxvec, secinvlistvec, courselist->size);

    fwrite(secidxvec, sizeof(Secidx), courselist->size, outputsecfp);
    fwrite(secinvlistvec, sizeof(SecInvList), nregs, outputseclistfp);

    fclose(datafp);
    fclose(inputfp);
    fclose(outputprimfp);
    fclose(outputsecfp);
    fclose(outputseclistfp);

    return 0;
}