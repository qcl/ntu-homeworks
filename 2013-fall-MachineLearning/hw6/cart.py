# -*- coding: utf-8 -*-
# c&rt algo
# qcl

from utils import readTrainData,readTestData,getDimN

def getClassesByData(data):
    cls = {}
    for d in data:
        if not d["y"] in cls:
            cls[d["y"]] = 0
        cls[d["y"]] += 1
    return cls

def buildHyp(ranges):
    ranges.sort()
    #r = [ranges[0]-0.001]
    r = []
    for i in xrange(1,len(ranges)):
        a = ranges[i-1]
        b = ranges[i]
        r.append((a+b)/2)
    #r.append(ranges[-1]+0.001)
    return r

def splitData(data,dim,th):
    a = []
    b = []
    for d in data:
        x = d["x"][dim]
        if x >= th:
            a.append(d)
        else:
            b.append(d)
    return a,b

def imp(data):
    rlt = getClassesByData(data)
    if len(rlt) == 1:
        return 0.
    else:
        m = 0
        cls = 0
        for k in rlt:
            if rlt[k] > m:
                m = rlt[k]
                cls = k
        return float(len(data)-m)/float(len(data))

def CART(trainData):
    
    if len(getClassesByData(trainData)) == 1:
        leaf = {"type":"leaf","class":trainData[0]["y"]}
        return leaf

    minE = len(trainData)
    minT = 0.
    minD = 0
    dimN = len(trainData[0]["x"])
    for i in xrange(dimN):
        dimX = getDimN(trainData,i)
        hyps = buildHyp(dimX)
        for th in hyps:
            da,db = splitData(trainData,i,th)
            ip = imp(da) + imp(db)
            #print i,th,ip

            if ip < minE:
                minE = ip
                minT = th
                minD = i
    
    da,db = splitData(trainData,minD,minT)
    #print minD,minT,minE,len(da),imp(da),len(db),imp(db)

    node = {"type":"node","dim":minD,"th":minT}

    rt = CART(da)
    rl = CART(db)

    node["rtree"] = rt
    node["ltree"] = rl

    return node

def travel(tree,deep=0):
    if tree["type"] == "leaf":
        #s = "--"*deep
        #s += str(tree["class"])
        s = " "*deep
        print s+"return "+str(tree["class"])
        #print s
    else:
        s = " "*deep
        print "%sif x[%d] >= %f:" % (s,tree["dim"],tree["th"])
        travel(tree["rtree"],deep+1)
        print "%selse:" % (s)
        #print "%sd(%d),t(%f)" % (s,tree["dim"],tree["th"])
        travel(tree["ltree"],deep+1)

def classify(tree,vector):
    
    if tree["type"]=="leaf":
        return tree["class"]
    else:
        d = tree["dim"]
        t = tree["th"]
        if vector[d] >= t:
            return classify(tree["rtree"],vector)
        else:
            return classify(tree["ltree"],vector)

def testing(tree,data):
    rlt = {"cor":0,"err":0}
    for d in data:
        x = d["x"]
        y = d["y"]

        if classify(tree,x)*y >= 0:
            rlt["cor"] += 1
        else:
            rlt["err"] += 1
    rlt["cor"] = float(rlt["cor"])/len(data)
    rlt["err"] = float(rlt["err"])/len(data)
    return rlt

def mlhw6p16():
    trainD = readTrainData()
    testD  = readTestData()

    dt = CART(trainD)
    
    travel(dt)

    eintest = testing(dt,trainD)
    print eintest
    eouttest = testing(dt,testD)
    print eouttest


if __name__ == "__main__":
    print "ML HW#6 P16"
    mlhw6p16()
