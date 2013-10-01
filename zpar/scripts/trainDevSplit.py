#!/usr/bin/python


import fileinput
from random import randint

def readInput():
    lines = []
    for line in fileinput.input():
        lines.append(line.strip())
    return lines


lines = readInput()


sentence = []
sentences = []
for line in lines:
    if not line:
        sentences.append(sentence)
        sentence = []
    else:
        sentence.append(line)

devProportion = len(sentences) / 10


def printSentence(sentence, outfile):
    for x in sentence:
        outfile.write(x)
        outfile.write("\n")
    outfile.write("\n")


devFile = open("dev.conll","w")
sentenceIndexSelection = randint(0,len(sentences)-devProportion)
for x in xrange(sentenceIndexSelection, sentenceIndexSelection+devProportion):
    sentence = sentences[x]
    printSentence(sentence, devFile)
devFile.close()

trainFile = open("train.conll","w")
for x in xrange(0, sentenceIndexSelection):
    sentence = sentences[x]
    printSentence(sentence, trainFile)
for x in xrange(sentenceIndexSelection+devProportion, len(sentences)):
    sentence = sentences[x]
    printSentence(sentence, trainFile)
trainFile.close()


