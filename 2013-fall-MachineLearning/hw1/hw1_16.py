# -*- coding: utf-8 -*-
# NTU CSIE ML HW#1 16
# Qing-Cheng Li (qcl) R01922024 
# r01922024 at csie.ntu.edu.tw

from pla import PLA 

def hw16():

    f = open('./hw1_15_train.dat','r')
    data = []
    for line in f:
        l = line.split()
        data.append([[float(i) for i in l[:-1]],int(l[-1])])
    f.close()

    updates = []

    for i in range(0,2000):
        
        w,u = PLA(data,True)
        print '#%d' % (i),w,u
        updates.append(u)

    print 'Avg:',float(sum(updates))/len(updates)

if __name__ == '__main__':
    hw16()
