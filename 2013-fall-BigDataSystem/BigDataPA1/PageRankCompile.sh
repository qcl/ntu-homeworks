#!/bin/bash

#   NTU CSIE BigData Homework #1
#
#   PageRankCompile.sh
#   Compile the PageRank.java then package it.
#
#   Qing-Cheng Li (qcl) <r01922024 at csie.ntu.edu.tw>
#   2013.10.14

rm -rf PageRank
mkdir PageRank

# compile
javac -classpath ${CLASSPATH} -d PageRank PageRank.java

# package
jar -cvf PageRank.jar -C PageRank .


