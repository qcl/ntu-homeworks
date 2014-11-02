# -*- coding: utf-8 -*-

import numpy as np

trainFile = open("./hw4_train.dat","r")
testFile = open("./hw4_test.dat","r")

Z = []
Y = []
Ztest = []
Ytest = []

Lambda = 10
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

Wreg = np.dot( np.dot( np.linalg.inv( np.dot(np.transpose(Z),Z) + Lambda*np.matrix(np.identity(Z.shape[1])) ) , np.transpose(Z)  ), Y )

print Wreg
print float(sum(abs(np.sign(Z*Wreg) - Y)/2))/len(Y)
print float(sum(abs(np.sign(Ztest*Wreg) - Ytest)/2))/len(Ytest)

