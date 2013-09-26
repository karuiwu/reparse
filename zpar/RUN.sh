#!/bin/sh

# Run Script. Compiles, trains, and runs Zpar's dependency parser in one go. You need to specify the name of the model, the training and testing inputs, the parsing output, and the evaluation result output.

# Example usage:
# > time ./RUN.sh models/model_2states_beam1 data/english_bnews_train.conll data/english_bnews_test.conll data/output_2states_beam1.txt data/results_2states_beam1.txt 

model=$1
trainingInput=$2
testingInput=$3
parsingOutput=$4
resultOutput=$5


#compile
./COMPILE.sh 

#train
if [ "$?" = 0 ]; then
    touch $model 
    rm -f $model
    dist/generic.depparser/train -c $trainingInput $model 1 
fi

#parse
if [ "$?" = 0 ]; then
    touch $parsingOutput
    rm -f $parsingOutput
    dist/generic.depparser/depparser -c $testingInput $parsingOutput $model > tempOutput
fi

#evaluate
if [ "$?" = 0 ]; then
    ./data/eval.pl -g $testingInput -s $parsingOutput > $resultOutput
fi



