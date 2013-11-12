#!/bin/bash

#   NTU CSIE BigData Homework #2
#
#   compile.sh
#   compile the Sort.java then package it.
#
#   Qing-Cheng Li (qcl) <r01922024 at csie.ntu.edu.tw>
#   2013.11.11

rm -rf WeightedPageRank
mkdir WeightedPageRank

# compile
javac -classpath ${CLASSPATH} -d WeightedPageRank WeightedPageRank.java

# package
jar -cvf WeightedPageRank.jar -C WeightedPageRank .


