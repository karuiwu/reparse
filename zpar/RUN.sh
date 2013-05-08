#!/bin/sh




# Training step
#dist/english.depparser/train data/train.txt model 1


# Parsing step
dist/english.depparser/depparser data/INPUT data/output.txt model

# #Evaluation step
python data/evaluate.py data/output.txt data/reference.txt
