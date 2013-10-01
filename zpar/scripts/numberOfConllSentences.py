#!/usr/bin/python


import fileinput

def readInput():
    lines = []
    for line in fileinput.input():
        lines.append(line.strip())
    return lines


lines = readInput()


counter = 0
for line in lines:
    line = line.strip()

    if not line:
        continue
    
    
    line = line[0:2]
    
    
    if line == "1\t":
        counter = counter+1

    
print counter

