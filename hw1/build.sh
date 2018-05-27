#!bin/bash

gcc -ansi idxgen.c -o idxgen
gcc -ansi viewdata.c -o viewdata
gcc -ansi viewprimidx.c -o viewprimidx
gcc -ansi viewsecidx.c -o viewsecidx
gcc -ansi addreg.c -o addreg