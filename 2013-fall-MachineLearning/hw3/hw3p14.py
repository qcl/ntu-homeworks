# -*- coding: utf-8 -*-

import numpy as np
import random as rd

e = []

w0 = 0.0
w1 = 0.0
w2 = 0.0
w3 = 0.0
w4 = 0.0
w5 = 0.0

for itera in range(0,1000):
    rd.seed()
    np.random.seed()

    tx = np.random.uniform(-1,1,(1000,2))
    x = []
    y = []
    for i in range(0,len(tx)):
        tmpx = tx[i]
        s = tmpx[0]*tmpx[0] + tmpx[1]*tmpx[1] - 0.6
        if s < 0:
            y.append([-1])
        else:
            y.append([1])

        x.append([1,tmpx[0],tmpx[1],tmpx[0]*tmpx[1],tmpx[0]*tmpx[0],tmpx[1]*tmpx[1]])

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

    print 'iteration #',itera,w,err
    e.append(err)

    w0 = w0 + w[0]
    w1 = w1 + w[1]
    w2 = w2 + w[2]
    w3 = w3 + w[3]
    w4 = w4 + w[4]
    w5 = w5 + w[5]


print 'avg', w0/1000,w1/1000,w2/1000,w3/1000,w4/1000,w5/1000,':',sum(e)/float(len(e))
