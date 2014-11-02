# -*- coding: utf-8 -*-
# ada boost
# qcl

from utils import readTrainData,readTestData,getDimN
import math

def buildHyp(ranges):
    ranges.sort()
    r = [ranges[0]-100]
    for i in xrange(1,len(ranges)):
        a = ranges[i-1]
        b = ranges[i]
        r.append((a+b)/2)
    r.append(ranges[-1]+100)
    return r

def desionstump(x,s,i,th):
    return s*(x[i] - th);

def gt(x,s,i,th):
    if desionstump(x,s,i,th) >= 0:
        return 1
    else:
        return -1

# single ds test, return 1 or -1
def testDesionStump(x,y,s,i,th):
    if desionstump(x,s,i,th)*y >= 0:
        return 1
    else:
        return -1

def getEviaDSu(trainData,U,s,i,th):
    cu = 0. 
    eu = 0. 
    en = 0
    for idx in xrange(len(trainData)):
        x = trainData[idx]["x"]
        y = trainData[idx]["y"]
        u = U[idx]

        if testDesionStump(x,y,s,i,th) >= 0:
            cu += u
        else:
            eu += u
            en += 1
    #print s,i,th,cu,eu,en
    return eu, cu, en

# return sign, dim, th, ein
def Ads(trainData,U):
    dimN = len(trainData[0]["x"])
    sign = [1,-1]

    err = len(trainData) 
    en  = len(trainData)
    dim = 0
    th  = 0
    sig = 1
    meu = 0. 
    mcu = 0. 
    for i in range(dimN):
        hyps = buildHyp(getDimN(trainData,i))
        for s in sign:
            for h in hyps:
                eu,cu,n = getEviaDSu(trainData,U,s,i,h)
                #print "\t",i,s,h
                if eu < err:
                    err = eu
                    meu = eu
                    mcu = cu
                    en  = n
                    dim = i
                    th  = h
                    sig = s

    #return Algo
    err = meu/(meu+mcu)
    #print en,sum(U),err,meu,mcu
    #return {}
    de = math.sqrt((1.0-err)/err)
    algo = {"err":err,"sign":sig,"dim":dim,"th":th,"de":de,"alpha":math.log(de),"en":en}
    return algo

def updateU(trainData,Ut,algo):
    if len(Ut) == 0:
        return [float(1)/len(trainData)]*len(trainData)

    U = []
    i = algo["dim"]
    s = algo["sign"]
    th = algo["th"]
    de = algo["de"]

    for idx in range( len( trainData)):
        x = trainData[idx]["x"]
        y = trainData[idx]["y"]
        if testDesionStump(x,y,s,i,th) >= 0:
            u = Ut[idx]/de
        else:
            u = Ut[idx]*de
        U.append(u)
    return U

def adaboost(trainData,iterN):
    U = []
    A = []
    for i in xrange(iterN):
        if i == 0:
            prevU = []
            prevA = {}
        else:
            prevU = U[i-1]
            prevA = A[i-1]
        u = updateU(trainData,prevU,prevA)
        algo = Ads(trainData,u)
        print "%d,%.10f" % (i,sum(u))
        #print algo
        U.append(u)
        A.append(algo)

    return A,U

def _adaboostTest(x,A):
    r = 0.
    for algo in A:
        a = algo["alpha"]
        i = algo["dim"]
        s = algo["sign"]
        th = algo["th"]
        r += (a*gt(x,s,i,th))
    if r >= 0:
        return 1
    else:
        return -1


def adaboostTest(data,A):
    err = 0;
    for d in data:
        x = d["x"]
        y = d["y"]
        r = _adaboostTest(x,A)
        if y*r >= 0:
            pass
        else:
            err += 1
    return float(err)/len(data)

def mlhw6p10():
    trainD = readTrainData()
    testD  = readTestData()

    G,U = adaboost(trainD,300)

    print "testing..."

    # P10/11
    for i in range(1,300):
        ein = adaboostTest(trainD,G[0:i])
        eout = adaboostTest(testD,G[0:i])
        print "%03d,%.6f,%.6f" % (i,ein,eout)

    #print "p12"
    # P12
    #u = U[-1]
    #for idx in range(len(u)):
    #    if trainD[idx]["y"] == 1:
    #        print "%d,%.6f,%.6f,%d,%f" % (idx,trainD[idx]["x"][0],trainD[idx]["x"][1],trainD[idx]["y"],u[idx])
    #for idx in range(len(u)):
    #    if trainD[idx]["y"] != 1:
    #        print "%d,%.6f,%.6f,%d,%f" % (idx,trainD[idx]["x"][0],trainD[idx]["x"][1],trainD[idx]["y"],u[idx])
    #
    #u.sort()
    #print u

if __name__ == "__main__":
    print "ML HW#6 P10"
    mlhw6p10()
