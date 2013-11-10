# -*- coding: utf-8 -*-
# NTU CSIE ML HW#1 20
# Qing-Cheng Li (qcl) R01922024 
# r01922024 at csie.ntu.edu.tw

from pocket import Pocket
from pocket import Testing
import random
import copy

def hw20():

    f = open('./hw1_18_train.dat','r')
    data = []
    for line in f:
        l = line.split()
        data.append([[float(i) for i in l[:-1]],int(l[-1])])
    f.close()

    f = open('./hw1_18_train.dat','r')
    testing = []
    for line in f:
        l = line.split()
        testing.append([[float(i) for i in l[:-1]],int(l[-1])])
    f.close()

    errors = []

    for i in range(0,2000):
        wp,w = Pocket(data,100)
        cr,er = Testing(wp,testing)
        print '#%d\t%f' % (i+1,er),wp
        errors.append(er)

    print 'Avg-err:',sum(errors)/len(errors)

if __name__ == '__main__':
    hw20()
