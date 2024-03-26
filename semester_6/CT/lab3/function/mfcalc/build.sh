#!/bin/sh

bison -o mfcalc.c mfcalc.y
gcc -Wall -o mfcalc mfcalc.c -lm
rm mfcalc.c

