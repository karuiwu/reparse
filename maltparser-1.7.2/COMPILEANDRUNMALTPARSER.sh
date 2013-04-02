ant dist

#INPUTFILE=eng_train.conll

#cp engmalt.poly-1.7.mco dist/maltparser-1.7.2/ 
#cp $INPUTFILE dist/maltparser-1.7.2/

#cd dist/maltparser-1.7.2
#java -jar maltparser-1.7.2.jar -c engmalt.poly-1.7 -i $INPUTFILE -o out.conll -m parse


#head out.conll


cd src/org/maltparser/examples/
java -cp classes:../../../../dist/maltparser-1.7.2/maltparser-1.7.2.jar org.maltparser.examples.ParsingExperiment

