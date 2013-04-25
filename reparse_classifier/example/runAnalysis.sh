#!/bin/bash

#LAMBDA= 1
for i in {1..2}
do
    FILENAME="weights$i"
    ../megam.opt -lambda $LAMBDA binary ../data/classifier/train/0 > $FILENAME
done

#python evaluteParse.py results data/classifier/dev/0 