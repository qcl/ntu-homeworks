#!/bin/bash

#   NTU CSIE BigData Homework #2
#
#   run.sh
#   Run the WeightedPageRank via HAMA.
#
#   Qing-Cheng Li (qcl) <r01922024 at csie.ntu.edu.tw>
#   2013.11.11
#
#   $1  iter#
#   $2  input
#   $3  output

# clean the output
hadoop fs -rmr $3

# run 
hama jar WeightedPageRank.jar tw.qcl.WeightedPageRank $1 $2 $3

