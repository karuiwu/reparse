#!/bin/sh


#rm model

# Training step
#dist/generic.depparser/train data/train.txt model 1
#dist/generic.depparser/train data/english_bnews_train.conll model 1


# Parsing step
dist/generic.depparser/depparser data/INPUT data/output_small.txt model
#dist/generic.depparser/depparser data/english_bnews_test.conll data/outputTemp.txt model

# #Evaluation step
#python data/evaluate.py data/output.txt data/reference.txt
#python data/evaluate.py data/outputTemp.txt data/referenceTemp.txt
