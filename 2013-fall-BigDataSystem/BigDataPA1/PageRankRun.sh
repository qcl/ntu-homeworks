#!/bin/bash

#   NTU CSIE BigData Homework #1
#
#   PageRankRun.sh
#   Run the PageRank via MapReduce.
#
#   Qing-Cheng Li (qcl) <r01922024 at csie.ntu.edu.tw>
#   2013.10.14

# clean the output
hadoop fs -rmr $3
hadoop fs -rmr $2-iter-*

# run 
hadoop jar PageRank.jar tw.qcl.PageRank $1 $2 $3

