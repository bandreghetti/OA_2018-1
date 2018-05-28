#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datatypes.c"

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
                fwrite(&tmpreg, sizeof(Reg), 1, datafpout);
                fwrite(&primidx0, sizeof(Primidx), 1, primidxfpout);
                fread(&primidx0, sizeof(Primidx), 1, primidxfp0);
            }
        } else if(strcmp(primidx0.primkey, primidx1.primkey) < 0) {
            fseek(datafp0, primidx0.rrn*sizeof(Reg), SEEK_SET);
            primidx0.rrn = dataoutidx;
            dataoutidx++;
            fread(&tmpreg, sizeof(Reg), 1, datafp0);
            fwrite(&tmpreg, sizeof(Reg), 1, datafpout);
            fwrite(&primidx0, sizeof(Primidx), 1, primidxfpout);
            fread(&primidx0, sizeof(Primidx), 1, primidxfp0);
        } else {
            fseek(datafp1, primidx1.rrn*sizeof(Reg), SEEK_SET);
            primidx1.rrn = dataoutidx;
            dataoutidx++;
            fread(&tmpreg, sizeof(Reg), 1, datafp1);
            fwrite(&tmpreg, sizeof(Reg), 1, datafpout);
            fwrite(&primidx1, sizeof(Primidx), 1, primidxfpout);
            fread(&primidx1, sizeof(Primidx), 1, primidxfp1);
        }
    }

    fclose(primidxfp0);
    fclose(primidxfp1);
    fclose(primidxfpout);
    fclose(datafp0);
    fclose(datafp1);
    fclose(datafpout);

    return 0;
}