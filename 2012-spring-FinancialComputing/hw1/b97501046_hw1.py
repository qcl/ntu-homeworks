# -*- coding:utf-8 -*-
# Principles of Financial Computing
# Homework 01
#
# Qing-Cheng Li
# B97501046

import sys

def main():
    print 'Please input n:'
    n = sys.stdin.readline()
    n = int(n.split()[0])
    if n <= 0:
        return
    
    p = []
    for i in range(n):
        print 'prices',i+1,'='
        t = sys.stdin.readline()
        p.append(float(t.split()[0]))

    prev = 100.0

    for i in range(n):
        this = p[i]
        f = (prev/this - 1)*100
        print 'f('+str(i)+','+str(i+1)+') =',f
        prev = this


if __name__ == '__main__':
    main()
