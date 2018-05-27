typedef struct reg {
    char matric[7];
    char name[41];
    char op[3];
    char course[3];
    char class[2];
} Reg;

typedef struct primidx {
    char primkey[31];
    unsigned short rrn;
} Primidx;

typedef struct secidx {
    char course[3];
    short head;
} Secidx;

typedef struct secinvlist {
    char primkey[31];
    short next;
} SecInvList;