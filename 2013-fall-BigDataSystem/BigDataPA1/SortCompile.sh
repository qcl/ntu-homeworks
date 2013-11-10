#!/bin/bash

#   NTU CSIE BigData Homework #1
#
#   SortCompile.sh
#   Compile the Sort.java then package it.
#
#   Qing-Cheng Li (qcl) <r01922024 at csie.ntu.edu.tw>
#   2013.10.14

rm -rf Sort
mkdir Sort

# compile
javac -classpath ${CLASSPATH} -d Sort Sort.java

# package
jar -cvf Sort.jar -C Sort .


