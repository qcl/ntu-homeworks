# -*- coding: utf-8 -*-

import numpy as np
import random as rd

e = []

for itera in range(0,1000):
    rd.seed()
    np.random.seed()

    tx = []
    for i in range(0,1000):
        tx.append([np.random.uniform(-1,1),np.random.uniform(-1,1)])

    #print tx

    y = []
    for i in range(0,len(tx)):
        tmpx = tx[i]
        s = tmpx[0]*tmpx[0] + tmpx[1]*tmpx[1] - 0.6
        if s < 0:
            y.append([-1])
        else:
            y.append([1])

    x = np.ones((1000,3))
    x[:,1:] = tx

    flip = rd.sample(range(0,len(y)),100)
    for i in flip:
        if y[i][0] > 0:
            y[i] = [-1]
        else:
            y[i] = [1]

    xm = np.matrix(x)
    ym = np.matrix(y)

    xpinv = np.linalg.pinv(xm)

    w = np.dot(xpinv,ym)

    #print w

    predict = np.dot(xm,w)

    #print predict

    err = 0

    for i in range(0,1000):
        if predict[i] < 0:  #predict = -1
            if not (y[i] < 0 ):
                err = err + 1
        else:               #predict = +1
            if y[i] < 0:
                err = err + 1

    err = err/float(len(y))

    print 'iteration #',itera,err
    e.append(err)

print 'avg', sum(e)/float(len(e))
