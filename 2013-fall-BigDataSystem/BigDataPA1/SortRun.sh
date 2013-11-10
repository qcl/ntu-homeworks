#!/bin/bash

#   NTU CSIE BigData Homework #1
#
#   SortRun.sh
#   Run the Sort via MapReduce.
#
#   Qing-Cheng Li (qcl) <r01922024 at csie.ntu.edu.tw>
#   2013.10.14

# clean the output
hadoop fs -rmr $2

# run 
hadoop jar Sort.jar tw.qcl.Sort $1 $2

