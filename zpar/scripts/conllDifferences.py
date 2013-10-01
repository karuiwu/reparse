#!/usr/bin/python

import sys


file1 = sys.argv[1]
file2 = sys.argv[2]

file1 = open(file1)
file2 = open(file2)

file1 = file1.readlines()
file2 = file2.readlines() 


i = 0
conllIndex1 = 0
conllIndex2 = 0
while i < len(file1):
    line1 = file1[i]
    line2 = file2[i]

    if line1[0] == '\n':
        i += 1
        conllIndex1 = conllIndex2
        conllIndex2 = i
        
        conll1 = file1[conllIndex1:conllIndex2]
        conll2 = file2[conllIndex1:conllIndex2]

        j = 0
        notEqual = False
        while j < len(conll1):
            conllline1 = conll1[j]
            conllline2 = conll2[j]
            if set(conllline1) != set(conllline2):
                for c1 in conll1:
                    print c1.strip()
                
                for c2 in conll2:
                    print c2.strip()
                print
                print
                notEqual = True
                break
            j += 1
            
        if not notEqual: 
            for c1 in conll1:
                sys.stderr.write(c1)
            for c2 in conll2:
                sys.stderr.write(c2)
            sys.stderr.write('\n')
            sys.stderr.write('\n')
        continue

    i += 1
