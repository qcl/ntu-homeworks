# -*- coding: utf-8 -*-
import math

def theta(s):
    return 1.0/(1+math.exp(s))

def vadd(a,b):
    t = []
    for i in range(0,len(a)):
        t.append(a[i]+b[i])
    return t

def vsub(a,b):
    t = []
    for i in range(0,len(a)):
        t.append(a[i]-b[i])
    return t

def vmul(v,m):
    t = []
    for i in range(0,len(v)):
        t.append(v[i]*m)
    return t

def vdiv(v,d):
    t = []
    for i in range(0,len(v)):
        t.append(float(v[i])/float(d))
    return t

def vvm(a,b):
    t = []
    for i in range(0,len(a)):
        t.append(a[i]*b[i])
    return sum(t)

f_train = open('./hw3_train.dat','r')
f_test  = open('./hw3_test.dat','r')

x = []
x_t = []
y = []
y_t = []

for line in f_train:
    l = line.split()
    tmp = [1.0]
    for i in range(0,len(l[:-1])):
        tmp.append(float(l[i]))
    x.append(tmp)
    y.append(float(l[-1]))

f_train.close()

for line in f_test: 
    l = line.split()
    tmp = [1.0]
    for i in range(0,len(l[:-1])):
        tmp.append(float(l[i]))
    x_t.append(tmp)
    y_t.append(float(l[-1]))

f_test.close()

dim = len(x[0])

# print dim
nu = 0.01

w = [0.0]*dim

for T in range(0,2000):
    tmpS = [0.0]*dim
    for i in range(0,len(x)):
        #print vmul(vmul(x[i],-1*y[i]) ,theta(vvm(w,x[i])*-1*y[i]))
        tmpS = vadd(tmpS, vmul( vmul(x[i],-1*y[i]), theta(vvm(w,x[i])*-1*y[i])))
    Ein = vdiv(tmpS,len(x))
    #print Ein
    w = vadd(w,vmul(Ein,-1*nu))
    print T,w,"\n"

print w

err = 0
for i in range(0,len(x_t)):
    res = vvm(w,x_t[i])
    if res < 0: # -1
        if not y_t[i] < 0:
            err = err + 1
    else:
        if y_t[i] < 0:
            err = err + 1

print err,err/float(len(x_t))

