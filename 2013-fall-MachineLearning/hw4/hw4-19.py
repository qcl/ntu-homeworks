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
Eval = []
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


Evc = []
for Lambda in Lambdas:
    Evc.append([])
for j in range(0,len(Z)/40):
    #print j*40,j*40+40
    Ztrain = []
    Ytrain = []
    Zval = []
    Yval = []
    for i in range(0,len(Z)):
        if i < j*40+40 and i >= j*40:
            Zval.append(Z[i])
            Yval.append(Y[i])
        else:
            Ztrain.append(Z[i])
            Ytrain.append(Y[i])

    Ztrain = np.matrix(Ztrain)
    Ytrain = np.matrix(Ytrain)
    Zval = np.matrix(Zval)
    Yval = np.matrix(Yval)

    for Lambda in Lambdas:
        Wreg = np.dot( np.dot( np.linalg.inv( np.dot(np.transpose(Ztrain),Ztrain) + (10**Lambda)*np.matrix(np.identity(Ztrain.shape[1])) ) , np.transpose(Ztrain)  ), Ytrain )

        #Wregs.append(Wreg)
        tmpevc = float(sum(abs(np.sign(Zval*Wreg) - Yval)/2))/len(Yval)
        Evc[Lambdas.index(Lambda)].append(tmpevc)

evc = []
for Lambda in Lambdas:
    evc.append(sum(Evc[Lambdas.index(Lambda)])/float(len(Evc[Lambdas.index(Lambda)])))
Z = np.matrix(Z)
Y = np.matrix(Y)
Ztest = np.matrix(Ztest)
Ytest = np.matrix(Ytest)

minEvcIndex = evc.index(min(evc))
print '19. Evc',min(evc),'lambda=',Lambdas[minEvcIndex]

Wreg = np.dot( np.dot( np.linalg.inv( np.dot(np.transpose(Z),Z) + (10**Lambdas[minEvcIndex])*np.matrix(np.identity(Z.shape[1])) ) , np.transpose(Z)  ), Y )
print '20. Ein',float(sum(abs(np.sign(Z*Wreg) - Y)/2))/len(Y)
print '20. Eout',float(sum(abs(np.sign(Ztest*Wreg) - Ytest)/2))/len(Ytest)
