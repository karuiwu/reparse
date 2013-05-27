#!/bin/sh


#rm model

# Training step
#dist/generic.depparser/train data/train.txt model 1
#dist/generic.depparser/train data/english_bnews_train.conll model 1


# Parsing step
#dist/generic.depparser/depparser data/INPUT data/output_small.txt model
#dist/generic.depparser/depparser -c data/english_bnews_test.conll_head data/outputTemp.txt_small model_beam1
dist/generic.depparser/depparser -c data/input2.txt data/outputTemp.txt_small model_beam1

# #Evaluation step
#python data/evaluate.py data/output.txt data/reference.txt
#python data/evaluate.py data/outputTemp.txt data/referenceTemp.txt
