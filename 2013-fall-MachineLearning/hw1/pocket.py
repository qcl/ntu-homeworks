# -*- coding: utf-8 -*-
# NTU CSIE ML HW#1 PLA Pocket Algorithm
# Qing-Cheng Li (qcl) R01922024 
# r01922024 at csie.ntu.edu.tw
import random
import copy

# data [ [[x,x,x,...,x],y], ... ]
def Pocket(origin_data,updateTimes,nu=1.0):

    # get inner product of 2 vectors
    def inner_product(v1,v2):
        return sum(i*j for i,j in zip(v1,v2))

    # get the error in data when using a weight vector 
    def get_error(wv):
        e = []
        for i in range(0,len(data)):
            x,y = data[i]
            if inner_product(wv,x)*y <= 0:
                e.append(i)
        return e

    data = copy.deepcopy(origin_data)
    for raw in data:
        raw[0].insert(0,1)
    
    update = 0
    
    # init weight vector
    w = [1] * len(data[0][0])
    wp = w
    we = get_error(wp)
    wec = len(we)

    random.seed()
    while True:
    
        if wec == 0:
            break
        
        i = random.choice(we)
        x,y = data[i]
        r = inner_product(w,x)

        if y*r <= 0:
            update = update + 1
            w = [i+nu*j*y for i,j in zip(w,x)]
            we = get_error(w)
            if len(we) < wec:
                wp = w
                wec = len(we)

        if update >= updateTimes:
            break

    return wp,w

def Testing(w,origin_test_data):
    
    # get inner product of 2 vectors
    def inner_product(v1,v2):
        return sum(i*j for i,j in zip(v1,v2))

    error = 0
    data = copy.deepcopy(origin_test_data)

    for raw in data:
        x,y = raw
        x.insert(0,1)
        if inner_product(w,x)*y <= 0:
            error = error + 1

    er = float(error)/len(data)
   
    # return correct rate, error rate
    return 1-er,er

