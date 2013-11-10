# -*- coding: utf-8 -*-
# NTU CSIE ML HW#2 18-20
# Qing-Cheng Li (qcl) R01922024 
# r01922024 at csie.ntu.edu.tw

import dsa

def hw2_18():

    f = open('./hw2_train.dat','r')
    data = []
    for line in f:
        l = line.split()
        data.append([[float(i) for i in l[:-1]],int(l[-1])])
    f.close()

    f = open('./hw2_test.dat','r')
    testing = []
    for line in f:
        l = line.split()
        testing.append([[float(i) for i in l[:-1]],int(l[-1])])
    f.close()

    s,i,theta,ein = dsa.DSA(data)
    cr,er = dsa.Testing(testing,i,s,theta)

    print 'E-in: %f' % (ein)
    print '(s,i,theta) = %d %d %f' % (s,i,theta)
    print 'E-out: %f' % (er)


if __name__ == '__main__':
    hw2_18()
