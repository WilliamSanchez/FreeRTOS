#!/bin/sh

 
echo "FILE: ===>>>  $pwd"
 
 cd source
 
 make clean
 make 
 
 cd build
 
 exec prova_fiesc
