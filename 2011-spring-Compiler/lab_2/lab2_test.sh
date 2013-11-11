#!/bin/bash

stuID=b97501046

for testfile in example2.g example3.g grammarF5.g GE1.g GE1\'.g GE2.g GE2\'.g GLA.g GLA\'.g GLB.g GLC.g GLD.g GLE.g GLF.g GDanglingElse.g
do
    make judge STU=${stuID} GRAM=${testfile}
done

