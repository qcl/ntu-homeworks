# -*- coding: utf-8 -*-
# b97501046 李卿澄
# 2011 Compiler Lab Bonus

import sys
import os

def main():
  fin = open(sys.argv[1],'r')
  fout = open(sys.argv[2],'w')
  
  sf = False
  sfc = False
  ds = False
  
  i = 0
  outline = []
  s = ''
  for line in fin:
    i = i+1;
    j = 0
    for c in line:
      j = j + 1
      if c == '"':
        if sf == False:
          sf = True
        else:
          if sfc == False:
            sfc = True
          else:
            sfc = False
            s = s + '"'
      elif c == ',':
        if sf == True and sfc == True:
          sf = False
          sfc = False
          outline.append(s)
          s = ''
        elif sf == True:
          s = s + ','
        else:
          outline.append(s)
          s = ''
      elif c == '\n':
        if sf == True and sfc == False:
          s = s + ' '
        else:
          sf = False
          sfc = False
          ds = True
          outline.append(s)
          s = ''
      elif c == '\t':
        s = s + ' '
      else:
        s = s + ('%c' % c)
    
    if ds == True:
      ds = False
      fout.write('*** LINE %d ***\n' % (i))
      for eachline in outline:
        ws = False
        tmps = ''
        for char in eachline:
          if char == ' ':
            if ws == False :
              ws = True
          else:
            if ws == True:
              ws = False
              tmps = tmps + ' ' + char
            else:
              tmps = tmps + char
        
        if len(tmps) > 0 and tmps[-1] == '\r':
          tmps = tmps[:-1]
        
        if len(tmps) > 0 and tmps[0] == ' ':
          tmps = tmps[1:]
        
        if len(tmps) > 0 and tmps[-1] == ' ':
          tmps = tmps[:-1]
        
        fout.write(tmps+'\n')
        tmps = ''
      outline = []
    
  fout.write('*** END ***\n')
if __name__ == '__main__':
  main()
