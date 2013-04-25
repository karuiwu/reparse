#!/bin/bash

LAMBDA=1
MAX=2
for i in {1..$MAX}
do
    WEIGHTS_FILENAME="../data/classifier/weights/$i"
    PR_FILENAME="../data/classifier/precision_recall/$i"
    echo "Training classifier: ../megam_i686.opt -lambda $LAMBDA binary ../data/classifier/train/0 > $WEIGHTS_FILENAME"
    ./megam_i686.opt -lambda $LAMBDA binary ../data/classifier/train/0 > $WEIGHTS_FILENAME
    python evaluteParse.py $WEIGHTS_FILENAME ../data/classifier/dev/0 > $PR_FILENAME
done