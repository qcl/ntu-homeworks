# -*- coding: utf-8 -*-

import numpy as np

trainFile = open("./hw4_train.dat","r")
testFile = open("./hw4_test.dat","r")

Z = []
Y = []
Ztest = []
Ytest = []
Ztrain = []
Ytrain = []
Zval = []
Yval = []

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

for i in range(0,len(Z)):
    if i < 120:
        Ztrain.append(Z[i])
        Ytrain.append(Y[i])
    else:
        Zval.append(Z[i])
        Yval.append(Y[i])

for line in testFile:
    l = line.split()
    l.insert(0,1.0)
    Ztest.append([float(i) for i in l[:-1]])
    Ytest.append([float(l[-1])])
testFile.close()

Z = np.matrix(Z)
Y = np.matrix(Y)
Ztrain = np.matrix(Ztrain)
Ytrain = np.matrix(Ytrain)
Zval = np.matrix(Zval)
Yval = np.matrix(Yval)
Ztest = np.matrix(Ztest)
Ytest = np.matrix(Ytest)

for Lambda in Lambdas:
    Wreg = np.dot( np.dot( np.linalg.inv( np.dot(np.transpose(Ztrain),Ztrain) + (10**Lambda)*np.matrix(np.identity(Ztrain.shape[1])) ) , np.transpose(Ztrain)  ), Ytrain )

    Wregs.append(Wreg)
    Ein.append(float(sum(abs(np.sign(Ztrain*Wreg) - Ytrain)/2))/len(Ytrain))
    Eval.append(float(sum(abs(np.sign(Zval*Wreg) - Yval)/2))/len(Yval))
    Eout.append(float(sum(abs(np.sign(Ztest*Wreg) - Ytest)/2))/len(Ytest))

minEinIndex = Ein.index(min(Ein))
minEvalIndex = Eval.index(min(Eval))
print '16. min Ein :',min(Ein),'lambda =',Lambdas[minEinIndex],'Eout =',Eout[minEinIndex],'Eval',Eval[minEinIndex]
print '17. min Eval:',min(Eval),'lambda =',Lambdas[minEvalIndex],'Ein =',Ein[minEvalIndex],'Eout',Eval[minEvalIndex]
print 'Ein',Ein
print 'Eval',Eval

Wreg = np.dot( np.dot( np.linalg.inv( np.dot(np.transpose(Z),Z) + (10**Lambdas[minEvalIndex])*np.matrix(np.identity(Z.shape[1])) ) , np.transpose(Z)  ), Y )
print '18. Ein',float(sum(abs(np.sign(Z*Wreg) - Y)/2))/len(Y)
print '18. Eout',float(sum(abs(np.sign(Ztest*Wreg) - Ytest)/2))/len(Ytest)
