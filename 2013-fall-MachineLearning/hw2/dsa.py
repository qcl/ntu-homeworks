# -*- coding: utf-8 -*-
# NTU CSIE ML HW#1 Decision Stump Algorithm for 1-D
# Qing-Cheng Li (qcl) R01922024 
# r01922024 at csie.ntu.edu.tw

# data [ [[x,x,x,...,x],y], ... ]
def DSA(origin_data):

    data = []
    thetaSet = []

    best_i = 0
    best_s = 1
    best_theta = 0

    best_E_in = len(origin_data) + 1

    # split into 1-D and bind with y
    for d in range(0,len(origin_data[0][0])):
        data.append([])
        thetaSet.append([])
        
        for index in range(0,len(origin_data)):
            data[d].append((origin_data[index][0][d],origin_data[index][1]))
        
        data[d].sort()

        # generate theta
        thetaSet[d].append( data[d][0][0] )
        for index in range(1,len(data[d])):
            thetaSet[d].append( (data[d][index-1][0] + data[d][index][0] )/2.0 )
        thetaSet[d].append( data[d][len(data[d])-1][0]+0.001 )


    # for each d, run decisoin stump algorithm
    for d in range(0,len(data)):

        theta = 0
        sign = 1
        min_e = len(data) + 1

        # XXX I think this is not a good algorithm orz
        # print data[d]
        for tmp_theta in thetaSet[d]:
            c,e = Testing(origin_data,d,1,tmp_theta)
            #print '+',e,tmp_theta

            if e < min_e:
                min_e = e
                sign = 1
                theta = tmp_theta

            c,e = Testing(origin_data,d,-1,tmp_theta)
            #print '-',e,tmp_theta

            if e < min_e:
                min_e = e
                sign = -1
                theta = tmp_theta
        
        #print sign,theta,min_e
        if min_e < best_E_in:
            best_E_in = min_e
            best_s = sign
            best_theta = theta
            best_i = d

    return best_s,best_i,best_theta,best_E_in

def Testing(data,i,s,theta):
    error = 0

    for index in range(0,len(data)):
        if s*(data[index][0][i]-theta) >= 0:
            if data[index][1] < 0:
                error = error + 1
        else:
            if data[index][1] >= 0:
                error = error + 1

    er = float(error)/len(data)
    return 1-er,er

def Eout(s,theta,noise):
    if s == 1:
        return ((1-noise)*abs(theta) + noise*(2-abs(theta)) )/2
    else:
        return (noise*abs(theta) + (1-noise)*(2-abs(theta)) )/2

if __name__ == '__main__':
    print 'Decision Stump Algorithm'

    DSA([
        [[-0.5,0.9],1],
        [[0.8,0.3],-1]
    ])

