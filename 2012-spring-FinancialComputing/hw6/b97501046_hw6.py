# -*- coding: utf-8 -*-
# Qing-Cheng Li
# B97501046

import os
import sys
import math
import copy

class HWM:
    """ Hull-White Trinomial Tree Model """
    
    def __init__(self,x,y,a,s,n,X,A,B,C):
        self.x = x
        self.y = y
        self.a = a
        self.s = s
        self.n = n
        self.X = X
        self.A = A
        self.B = B
        self.C = C

        self.dt = y/n
        self.jmax = int(math.ceil(0.184/(self.a*self.dt)))
        self.dr = s*math.sqrt(3.0*self.dt) 
        
        jmax = self.jmax
        dt = self.dt
        dr = self.dr

        self.target = int(n*float(x)/float(y))

        Q = {}
        q = {}
        payoff = {}
        bondPrice = {}
        for i in range(-jmax,jmax+1):
            Q[i] = 0.0
            q[i] = 0.0
            payoff[i] = 0.0
            bondPrice[i] = 0.0

        r = {}
        for i in range(n):
            r[i] = {}
            for j in range(-jmax,jmax+1):
                r[i][j] = 0.0

        Q[0] = 1.0
        for i in range(n):
            tmp = 0.0
            for j in range(-min(i,jmax),min(i,jmax)+1):
                tmp = tmp + Q[j]*math.exp(-j*dr*dt)
            
            beta = ((A-B*math.exp(-C*float(i+1)*dt))*float(i+1)*dt+math.log(tmp))/dt
            
            for j in range(-jmax,jmax+1):
                r[i][j] = beta+j*dr

            for j in range(-jmax,jmax+1):
                q[j] = 0.0

            for j in range(-min(i,jmax),min(i,jmax)+1):
                if j == jmax:
                    k = j - 1
                elif j == -jmax:
                    k = j + 1
                else:
                    k = j

                p1 = self.getP1(j,k)
                p2 = self.getP2(j,k)
                p3 = self.getP3(j,k)

                q[k+1] = q[k+1] + p1*Q[j]*math.exp(-r[i][j]*dt)
                q[k]   = q[k]   + p2*Q[j]*math.exp(-r[i][j]*dt)
                q[k-1] = q[k-1] + p3*Q[j]*math.exp(-r[i][j]*dt)
               
            for j in range(-jmax,jmax+1):
                Q[j] = q[j]
    
        for j in range(min(n,jmax),-min(n,jmax)-1,-1):
            bondPrice[j] = 1.0

        for i in range(n-1,0-1,-1):
            prevBondPrice = copy.deepcopy(bondPrice)

            if i < self.target:
                prevPayoff = copy.deepcopy(payoff)

            for j in range(min(i,jmax),-min(i,jmax)-1,-1):
                if j == jmax:
                    k = j - 1
                elif j == -jmax:
                    k = j + 1
                else:
                    k = j
                
                p1 = self.getP1(j,k)
                p2 = self.getP2(j,k)
                p3 = self.getP3(j,k)

                if i == self.target:
                    payoff[j] = max(bondPrice[j]-X,0.0)
                elif i < self.target:
                    payoff[j] = math.exp(-dt*r[i][j])*(p1*prevPayoff[k+1]+p2*prevPayoff[k]+p3*prevPayoff[k-1])

                bondPrice[j] = math.exp(-dt*r[i][j])*(p1*prevBondPrice[k+1]+p2*prevBondPrice[k]+p3*prevBondPrice[k-1])

        self.price = payoff[0]
        self.delta = (prevPayoff[1] - prevPayoff[-1])/(prevBondPrice[1] - prevBondPrice[-1])

    def getP1(self,j,k):
        j = float(j)
        k = float(k)
        return (1.0/6.0) + (self.a*self.a*j*j*self.dt*self.dt - 2.0*self.a*j*self.dt*(j-k)+(j-k)*(j-k)-self.a*j*self.dt+(j-k))/2.0;

    def getP2(self,j,k):
        j = float(j)
        k = float(k)
        return (2.0/3.0) - (self.a*self.a*j*j*self.dt*self.dt - 2.0*self.a*j*self.dt*(j-k)+(j-k)*(j-k));

    def getP3(self,j,k):
        j = float(j)
        k = float(k)
        return (1.0/6.0) + (self.a*self.a*j*j*self.dt*self.dt - 2.0*self.a*j*self.dt*(j-k)+(j-k)*(j-k)+self.a*j*self.dt-(j-k))/2.0;

    def getPrice(self):
        return self.price

    def getDelta(self):
        return self.delta

if __name__ == '__main__':
    
    print 'Financial Computing 2012'
    print 'Homework #6'
    print ''
    print 'B97501046'
    print 'Qing-Cheng Li'
    print ''
    
    #Input argv
    x = float(input('x (year):'))
    y = float(input('y (year):'))
    a = float(input('a:'))
    s = float(input('S (%):'))/100.0
    n = int(input('n (# of partitions):'))
    X = float(input('strike price X (%):'))/100.0
    A = float(input('A:'))
    B = float(input('B:'))
    C = float(input('C:'))

    option = HWM(x,y,a,s,n,X,A,B,C)
    print ''
    print 'Price = ',option.getPrice()
    print 'Delta = ',option.getDelta()

