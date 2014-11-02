# -*- coding: utf-8 -*-
# utils for ML hw6
# qcl

def getDimN(data,dim):
    return map(lambda d: d["x"][dim], data)

def readData(filename):
    f = open(filename,"r")
    Data = []
    for line in f:
        rawdata = line[:-1].split()
        x = map(lambda x: float(x), rawdata[:-1])
        y = int(rawdata[-1])
        Data.append({"x":x,"y":y})
    f.close()
    return Data

def readTrainData():
    return readData("./hw6_train.dat")

def readTestData():
    return readData("./hw6_test.dat")

if __name__ == "__main__":
    print "Utils demo"
    print "Train Data length",len(readTrainData())
    print "Test  Data length",len(readTestData())
