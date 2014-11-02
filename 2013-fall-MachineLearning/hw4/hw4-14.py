# -*- coding: utf-8 -*-

import numpy as np

trainFile = open("./hw4_train.dat","r")
testFile = open("./hw4_test.dat","r")

Z = []
Y = []
Ztest = []
Ytest = []

Lambdas = [i for i in range(-10,3)]
Ein = []
Eout = []
Wregs = []

for line in trainFile:
    l = line.split()
    l.insert(0,1.0)
    Z.append([float(i) for i in l[:-1]])
    Y.append([float(l[-1])])

trainFile.close()

for line in testFile:
    l = line.split()
    l.insert(0,1.0)
    Ztest.append([float(i) for i in l[:-1]])
    Ytest.append([float(l[-1])])
testFile.close()

Z = np.matrix(Z)
Y = np.matrix(Y)
Ztest = np.matrix(Ztest)
Ytest = np.matrix(Ytest)

for Lambda in Lambdas:
    Wreg = np.dot( np.dot( np.linalg.inv( np.dot(np.transpose(Z),Z) + (10**Lambda)*np.matrix(np.identity(Z.shape[1])) ) , np.transpose(Z)  ), Y )

    Wregs.append(Wreg)
    Ein.append(float(sum(abs(np.sign(Z*Wreg) - Y)/2))/len(Y))
    Eout.append(float(sum(abs(np.sign(Ztest*Wreg) - Ytest)/2))/len(Ytest))


minEinIndex = Ein.index(min(Ein))
minEoutIndex = Eout.index(min(Eout))


print '14. min Ein :',min(Ein),'lambda =',Lambdas[minEinIndex],'Eout =',Eout[minEinIndex]
print '15. min Eout:',min(Eout),'lambda =',Lambdas[minEoutIndex],'Ein =',Ein[minEoutIndex]
print 'Ein',Ein
print 'Eout',Eout

