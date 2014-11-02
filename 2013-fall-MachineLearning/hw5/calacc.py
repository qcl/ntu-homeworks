
def acc(ansfile,outputfile):
    ans = open(ansfile,"r")
    out = open(outputfile,"r")

    A = []

    for l in ans:
        A.append(int(l.split()[0]))
    ans.close()

    i = 0
    e = 0
    for l in out:
        r = float(l.split()[0])
        if r*A[i] >= 0:
            pass
        else:
            #err
            e = e + 1
        i = i + 1
    out.close()

    return float(e)/len(A)

E = []
for i in range(1,6):
    e = acc("./hw5_15_%d_test.in" % (i),"./hw5_15_%d.out" % (i))
    E.append(e)

e = acc("./hw5_14_train.in","./hw5_15_0.out")

print "Ein=",e
print "Evc=",sum(E)/len(E)
 
