# -*- coding: utf-8 -*-
# Quadratic Programming Solver

import numpy as np
from scipy import optimize

def QP(A,c,P,r):

    dim = A.shape[0]

    u0 = np.ones((dim,1))

    def objective(x):
        return (0.5*np.dot(x.T,np.dot(A,x))+np.dot(c.T,x))

    cons = []
    for i in range(r.shape[0]):
        def f(x, i=i):
            return np.dot(P[i],x) - r[i]
        cons.append({"type":"ineq","fun":f})

    res_cons = optimize.minimize(objective,u0,constraints=cons,method='SLSQP',options={'disp':False})

    return res_cons 

def trainGsvm(X,y):
    num,dim = X.shape

    # Build A
    A = np.identity(dim+1)
    A[0][0] = 0.

    # Build c
    c = np.zeros((dim+1,1))

    # Build P
    P = np.ones((num,dim+1))
    P[:,1:] = X
    for _ in xrange(y.shape[0]):
        P[_] = P[_]*y[_]

    # Build r
    r = np.ones((num,1))

    return QP(A,c,P,r)

# kernel function:
# def kernel(X,n,m), return a value?

def kernelSVMwithQP(X,y,kernel):

    num,dim = X.shape

    # build Q
    Q = np.zeros((num,num))
    for n in range(num):
        for m in range(num):
            Q[n][m] = y[n]*y[m]*kernel(X[n],X[m])

    print "Q"
    print Q

    # build c
    c = np.ones((num,1))*-1

    #alpha = np.random.randn(num,1)
    alpha = np.ones((num,1))

    def objective(x):
        return (0.5*np.dot(x.T,np.dot(Q,x))+np.dot(c.T,x))

    cons = []
    for i in range(num):
        def f(x, i=i):
            return x[i]
        cons.append({"type":"ineq","fun":f})
    cons.append({"type":"eq","fun":lambda x: np.dot(y.T,x)})

    #print np.dot(y.T,alpha)

    res_cons = optimize.minimize(objective,alpha,constraints=cons,method='SLSQP',options={'disp':False})

    return res_cons

if __name__ == "__main__":
    print "Quadratic Programming Solver Testing"

    # Input
    X = np.array([ [0.,0.] , [2.,2.] , [2.,0.] , [3.,0.] ])
    
    # Label
    y= np.array([ [-1], [-1], [1], [1]])

    #print trainGsvm(X,y)

    def k(X,n,m):
        return (2+np.dot(X[n].T,X[m]))**2

    kernelSVMwithQP(X,y,k)
