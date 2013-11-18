import math

def getU(u,v):
    return math.exp(u) + v*math.exp(u*v) + 2*u - 2*v - 3

def getV(u,v):
    return 2*math.exp(2*v) + u*math.exp(u*v) - 2*u + 4*v - 2

def getG(u,v):
    return (getU(u,v),getV(u,v))

t = [getG(0,0)]

for i in range(1,6):
    p = t[i-1]
    q = getG(p[0],p[1])
    k = ( p[0] - 0.01*q[0] , p[1] - 0.01*q[1]  )
    print p,'- 0.01 *',q,'->',k
    print ''
    t.append(k)


print t
