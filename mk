#!/bin/sh
sed -i -f addldovr.sed stack-fix.c
gcc -m32 -mincoming-stack-boundary=2 -shared -o stack-fix.{so,c} -ldl -lX11 -lGL
sed -i -f delldovr.sed stack-fix.c
