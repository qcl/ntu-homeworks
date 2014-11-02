# -*- coding: utf-8 -*-
# ml hw5 p1

import numpy as np
from qps import trainGsvm,kernelSVMwithQP

def transToZ(v):
    return [ v[1]**2 - 2*v[0] - 2 , v[0]**2 - 2*v[1] + 3 ]

X = []
Z = []
y = []

f = open("01.in","r")
for line in f:
    points = line[:-1].split()
    tmpx = []
    for a in points[:-1]:
        tmpx.append(float(a))
    tmpy = [ float(points[-1]) ]

    X.append(tmpx)
    Z.append(transToZ(tmpx))
    y.append(tmpy)

f.close()

X = np.array(X)
Z = np.array(Z)
y = np.array(y)

print "x"
print X

print "z"
print Z

print "y"
print y

print "p1"
result = trainGsvm(Z,y)
print result

def k(a,b):
    return (2+np.dot(a.T,b))**2


print "p3"
result = kernelSVMwithQP(X,y,k)
print result

for i in range(7):
    print X[i],X[5],y[i],k(X[i],X[5])
