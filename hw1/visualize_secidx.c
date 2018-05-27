#include <stdio.h>
#include <string.h>

typedef struct secidx {
    char course[3];
    short head;
} Secidx;

typedef struct secinvlist {
    char primkey[31];
    short next;
} SecInvList;

unsigned long int fsize(char* fname)
{
    FILE* fp;
    fp = fopen(fname, "r");
    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);
    fclose(fp);
    return size;
}

char input_secidx[31];
char input_secinvlist[40];
FILE *fpidx, *fplist;

int main(int argc, char* argv[])
{    
    if(argc > 2)
    {
        strcpy(input_secidx, argv[1]);
        strcpy(input_secinvlist, argv[2]);
    } else {
        printf("ERROR: Missing argument(s).\n");
        return -1;
    }

    int secidxsize = fsize(input_secidx);
    int seclistsize = fsize(input_secinvlist);

    fpidx = fopen(input_secidx, "r");
    fplist = fopen(input_secinvlist, "r");

    int n_courses = secidxsize/sizeof(Secidx);
    int n_regs = seclistsize/sizeof(SecInvList);

    printf("Secidxsize = %d\n", secidxsize);
    printf("sizeof(Secidx) = %ld\n", sizeof(Secidx));
    printf("n_courses = %d\n\n", n_courses);

    Secidx secidx;
    SecInvList secinvlist;
    int i = 0, j = 0;
    printf("COURSE|HEAD          INDEX|PRIMARY KEY                   |NEXT\n");
    while(i < n_courses || j < n_regs)
    {
        // printf("%d < %d\n", i, n_courses);
        if(i < n_courses)
        {
            fread(&secidx, sizeof(Secidx), 1, fpidx);
            printf("%-6s| %03d", secidx.course, secidx.head);
        } else {
            printf("           ");
        }
        printf("          ");
        if(j < n_regs)
        {
            fread(&secinvlist, sizeof(SecInvList), 1, fplist);
            printf("  %03d|%s| %3d\n", j, secinvlist.primkey, secinvlist.next);
        }
        ++i;
        ++j;
    }

    return 0;
}