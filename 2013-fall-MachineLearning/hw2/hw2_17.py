# -*- coding: utf-8 -*-
# NTU CSIE ML HW#2 
# Qing-Cheng Li (qcl) R01922024
# r01922024 at csie.ntu.edu.tw
import random
import dsa

def hw2_17():
        
    e_in = []
    e_out = []

    for t in range(0,5000):
    
        #generate data set.
        random.seed()

        noise = 0.2
        data = []
        x = []
    

        while len(x) < 20:
            n = random.uniform(-1,1)
            if not n in x:
                x.append(n)

        for i in range(0,20):
            data.append([])
            data[i].append([x[i]])
            if random.random() < noise:
                data[i].append(-1 if x[i]>=0 else 1)
            else:
                data[i].append(1 if x[i]>=0 else -1)

        sign,i,theta,ein = dsa.DSA(data)
        eout = dsa.Eout(sign,theta,noise)

        print "%04d sign=%02d i=%d theta=%.3f e_in=%.3f e_out=%.3f" % ((t+1),sign,i,theta,ein,eout)

        e_in.append(ein)
        e_out.append(eout)

    print 'avg E-in:',sum(e_in)/len(e_in)
    print 'avg E-out:',sum(e_out)/len(e_out)

if __name__ == '__main__':
    hw2_17()
