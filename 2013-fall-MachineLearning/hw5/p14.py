# -*- coding: utf-8 -*-
# ml hw5 p14

def readFile(filename):
    x = []
    y = []
    f = open(filename,"r")
    for line in f:
        points = line[:-1].split()
        tmpx = []
        for a in points[:-1]:
            tmpx.append(float(a))
        tmpy = [ int(points[-1]) ]

        x.append(tmpx)
        y.append(tmpy)
    f.close()

    return x,y

trainX,trainy = readFile("hw5_14_train.dat")

for _ in xrange(len(trainX)):
    s = str(trainy[_][0])
    v = trainX[_]
    for i in xrange(len(v)):
        s = s + " " + str(i+1) + ":" + str(v[i])
    print s


