# -*- coding: utf-8 -*-
# NTU CSIE ML HW#1 15
# Qing-Cheng Li (qcl) R01922024 
# r01922024 at csie.ntu.edu.tw

from pla import PLA 

def hw15():

    f = open('./hw1_15_train.dat','r')
    data = []
    for line in f:
        l = line.split()
        data.append([[float(i) for i in l[:-1]],int(l[-1])])
    f.close()

    print PLA(data)

if __name__ == '__main__':
    hw15()
