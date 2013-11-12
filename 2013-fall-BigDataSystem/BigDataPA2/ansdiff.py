# -*- coding: utf-8 -*-

import sys

def main(ans, yans):

    fa = open(ans,'r')
    fb = open(yans,'r')

    p = 6

    a = {}
    e = False

    for line in fa:
        l = line.split('\t')
        a[l[0]] = l[1].split()[0]

    fa.close()

    for line in fb:
        l = line.split('\t')
        k = l[0]
        v = l[1].split()[0]
        if not k in a:
            print 'Error<KeyError> ',line
        else:
            c = a[k].split('.')[1]
            if not c[:p] == v.split('.')[1][:p]:
                print 'Error',k,a[k],'<--->',v
                e = True
            else:
                print '\t',k,c[:p],'OK'
    fb.close()

    if e:
        print "Jizz"
    else:
        print "Pass"

if __name__ == '__main__':

    if len(sys.argv) != 3:
        print 'Usage: python ansdiff.py <ans> <you ans>'
    else:
        main(sys.argv[1],sys.argv[2]);
