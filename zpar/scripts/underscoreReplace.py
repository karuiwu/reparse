#!/usr/bin/python

from sys import argv

f = open(argv[1])

lines = f.readlines()

for line in lines:
    if line == '\n':
        print
        continue

    print line.replace("_","-").strip()
    '''line = line.split()


    #print line
    print line[1]+"\t"+line[4]+"\t"+line[6]+"\t"+ line[7]

    #exit()
    '''
