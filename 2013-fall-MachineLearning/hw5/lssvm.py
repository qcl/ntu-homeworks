# -*- coding: utf-8 -*-
# qcl

import numpy as np
import sys

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

def getBeta(lam,X,y,kernel):

    dim = X.shape[0]

    I = np.identity(dim)
    for i in xrange(dim):
        for j in xrange(dim):
            I[i][j] = I[i][j]*lam + kernel(X[i],X[j])
    beta = np.dot(np.linalg.inv(I),y)
    return beta
    
def evaluate(beta,X,kernel,v):

    dim = X.shape[0]
    result = 0.
    for i in xrange(dim):
        result = result + (beta[i]*kernel(X[i],v))

    if result >= 0. :
        return 1
    else:
        return -1

def main(alpha,lam,filename):
    
    x,y = readFile(filename)

    def kernel(u,v):
        c = u - v
        t = -np.dot(c.T,c)
        return t/(2*alpha*alpha)

    blockSize = len(x)/5

    err = []
    for i in xrange(5):
        trainX = []
        trainY = []
        testX  = []
        testY  = []

        for j in xrange(len(x)):
            if j/blockSize == i:
                testX.append(x[j])
                testY.append(y[j])
            else:
                trainX.append(x[j])
                trainY.append(y[j])
        
        trainX = np.array(trainX)
        trainY = np.array(trainY)
        testX  = np.array(testX)
        testY  = np.array(testY)

        beta = getBeta(lam,trainX,trainY,kernel)

        for _ in xrange(beta.shape[0]):
            if beta[_] <= 0. and beta[_] > -0.0001:
                print _,beta[_]

        e = 0.
        for j in xrange(len(testX)):
            r = evaluate(beta,trainX,kernel,testX[j])
            if r*testY[j] < 0:
                e = e + 1
        print i,e,e/len(testX)
        err.append(e/len(testX))

    trainX = np.array(x)
    trainY = np.array(y)

    beta = getBeta(lam,trainX,trainY,kernel)
    e = 0.
    for j in xrange(len(trainX)):
        r = evaluate(beta,trainX,kernel,trainX[j])
        if r*trainY[j] < 0:
            e = e + 1

    

    print "alpha",alpha
    print "lambda",lam
    print "Ein",e/len(trainY)
    print "Ecv",sum(err)/len(err)


if __name__ == "__main__":
    if len(sys.argv) == 4:
        main(float(sys.argv[1]),float(sys.argv[2]),sys.argv[3])
    else:
        print "Jizz"
