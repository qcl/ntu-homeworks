# -*- coding: utf-8 -*-
# ml hw5 p1

import numpy as np
import random
from qps import trainGsvm,kernelSVMwithQP

def readFile(filename):
    x = []
    y = []
    f = open(filename,"r")
    for line in f:
        points = line[:-1].split()
        tmpx = []
        for a in points[:-1]:
            tmpx.append(float(a))
        tmpy = [ float(points[-1]) ]

        x.append(tmpx)
        y.append(tmpy)
    f.close()

    return x,y

trainX,trainy = readFile("hw5_13_train.dat")
testX,testy   = readFile("hw5_13_test.dat")

num = int(len(trainX)*0.8)

for _ in xrange(100):

    X = []
    y = []

    random.seed()

    # get 80% as train data
    for i in random.sample(range(len(trainX)),num):
        X.append(trainX[i])
        y.append(trainy[i])

    X = np.array(X)
    y = np.array(y)

    result = trainGsvm(X,y)["x"]

    b = result[0]
    w = result[1:]

    margin = 1.0/np.dot(w,w.T)

    err = 0.
    for i in xrange(len(testX)):
        if np.dot(w.T,testX[i]) < 0:
            if testy[i] > 0:
                err = err + 1    
        else:
            if testy[i] < 0:
                err= err + 1

    print "\"%f\",\"%f\"" % (margin,err/len(testX))



