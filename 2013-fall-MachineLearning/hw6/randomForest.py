# -*- coding: utf-8 -*-
# random forest
# qcl

from utils import readTrainData,readTestData,getDimN
from cart import CART,testing,classify
import random
import math


def randomForest(trainData,size=100):
    forest = []
    oobSize = int(len(trainData)/math.exp(1))
    for i in xrange(size):
        random.seed()
        seedTrainData = random.sample(trainData,len(trainData)-oobSize)
        tree = CART(seedTrainData)
        forest.append(tree)
        print "Build tree #%d" % (i+1)
    return forest

def testEachTreeInForest(forest,data):
    rlt = []
    for tree in forest:
        rlt.append(testing(tree,data))
    return rlt

def classifyByForset(forest,vector):
    r = 0
    for tree in forest:
        r += classify(tree,vector)
    if r >= 0:
        return 1
    else:
        return -1

def testForest(forest,data):
    rlt = {"cor":0,"err":0}
    for d in data:
        x = d["x"]
        y = d["y"]

        if classifyByForset(forest,x)*y >= 0:
            rlt["cor"] += 1
        else:
            rlt["err"] += 1
    rlt["cor"] = float(rlt["cor"])/len(data)
    rlt["err"] = float(rlt["err"])/len(data)
    return rlt


def histogram(num):
    num.sort()
    mi = math.floor(num[0]/0.01)
    ma = math.ceil(num[-1]/0.01)
    i = mi
    his = {}
    while i < ma:
        his[i] = 0
        i += 1
    for n in num:
        his[math.floor(n/0.01)] += 1
    return his

def mlhw6p17():
    trainD = readTrainData()
    testD  = readTestData()

    # training 
    forest = randomForest(trainD)
    
    # testing
    result = testEachTreeInForest(forest,testD)

    eouts = map(lambda r:r["err"],result)
    eouts.sort()

    print "histogram"
    his = histogram(eouts)
    # Show histogram
    for key in his:
        print "[%3d%%,%3d%%) |%s %d (%.0f%%)" % (key,key+1,"|"*his[key],his[key],float(his[key])*100/len(eouts))


    for i in xrange(len(forest)):
        ein  = testForest(forest[0:i+1],trainD)
        eout = testForest(forest[0:i+1],testD)
        ein = ein["err"]
        eout = eout["err"]
        print "%d,%.4f,%.4f" % (i+1,ein,eout)



if __name__ == "__main__":
    print "ML HW#6 P17"
    mlhw6p17()
