#!/bin/sh
# Run Script. Compiles, trains, and runs Zpar's dependency parser in one go. You need to specify the name of the model, the training and testing inputs, the parsing output, and the evaluation result output.

# Example usage:
# > time ./RUN.sh models/model_2states_beam1 data/english_bnews_train.conll data/english_bnews_test.conll data/output_2states_beam1.txt data/results_2states_beam1.txt 
model=$1
trainingInput=$2
testingInput=$3
parsingOutput=$4
resultOutput=$5

if [ -z $5 ]; then
    echo
    echo "Not enough paramaters"
    echo "RUN.sh model trainingInput testingInput parsingOutput resultOutput"
    echo
    exit
fi


shift
shift
shift
shift
shift


#compile
./scripts/COMPILE.sh 

#create train/dev split
#if [ "$?" = 0 ]; then
#    ./data/trainDevSplit.py $trainingInput $testingInput
#    mv train.conll data/
#    mv dev.conll data/
#
#    trainingInput="data/train.conll"
#    testingInput="data/dev.conll"
#else
#    exit
#fi




#train
if [ "$?" = 0 ]; then
    touch $model 
    rm -f $model
    dist/generic.depparser/train -c $trainingInput $model 1 
else
    exit
fi

#parse
if [ "$?" = 0 ]; then
    touch $parsingOutput
    rm -f $parsingOutput
    dist/generic.depparser/depparser -c $testingInput $parsingOutput $model 
else
    exit
fi

#evaluate
if [ "$?" = 0 ]; then
    ./scripts/eval.pl -g $testingInput -s $parsingOutput > $resultOutput
else
    exit
fi




