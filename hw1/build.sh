#!/bin/bash

gcc -ansi idxgen.c -o idxgen
gcc -ansi viewdata.c -o viewdata
gcc -ansi viewprimidx.c -o viewprimidx
gcc -ansi viewsecidx.c -o viewsecidx
gcc -ansi addreg.c -o addreg
gcc -ansi delreg.c -o delreg
gcc -ansi mergefiles.c -o mergefiles
gcc -ansi updatereg.c -o updatereg