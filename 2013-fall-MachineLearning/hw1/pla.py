# -*- coding: utf-8 -*-
# NTU CSIE ML HW#1 PLA
# Qing-Cheng Li (qcl) R01922024 
# r01922024 at csie.ntu.edu.tw
import random
import copy

demo_data = [
    [[0.0,0.0],1],
    [[0.0,1.0],1],
    [[1.0,0.0],1],
    [[1.0,1.0],-1]
]

# data [ [[x,x,x,...,x],y], ... ]
def PLA(origin_data,rand=False,nu=1.0):

    def inner_product(v1,v2):
        return sum(i*j for i,j in zip(v1,v2))

    data = copy.deepcopy(origin_data)

    for raw in data:
        raw[0].insert(0,1)
    
    if rand:
        random.seed()
        random.shuffle(data)

    update = 0
    
    # init weight vector
    w = [1] * len(data[0][0])

    while True:
       
        err = 0
        for raw in data:
            x,y = raw
            r = inner_product(w,x)
            
            # The same sign: > 0
            if y*r <= 0:
                err = err + 1
                update = update + 1
                #print 'update!,',
                w = [i+nu*j*y for i,j in zip(w,x)]
                #print w
                #break

        if err == 0:
            break

    return w,update

def demo():
    print 'Demo PLA'
    print PLA(demo_data)
    print PLA(demo_data,0.5)

if __name__ == '__main__':
    demo()
