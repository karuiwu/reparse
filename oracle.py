#!/usr/bin/python

import sys
from pprint import pprint



# t = transition
# c = configuration

# G = current parse
# G_gold = gold parse



class Sentence:
    def __init__(self, sentence, parse):
        self.sentence = sentence
        #self.parse = parse
        self.initialConfiguration = parse
    

'''On object that is supposed to represent a parse. 
This is so that we can compare a parse to a Gold parse'''
#TODO
class Parse: 
    def __init__(self, isTerminal):
        self.isTerminal = isTerminal

    def listOfDifferentArcs(self, ParseToCampareWith):
        return []

    def makeTransitionMove(self, t):
        return self

    def bestPossibleParse(self, G_gold):
        return self


#TODO
class Transition:
    def __init__(self, name):
        self.name = name

    def __eq__(self, otherTransition):
        return self.name == otherTransition.name

    

#TODO
class Oracle:
    def __init__(self):
        temp = 0

    ''' Loss function. Returns the number of arcs that are in the Gold parse and not this one
    '''
    def LOSS(self, G, G_gold):
        differentArcs = G.listOfDifferentArcs(G_gold)
        return differentArcs.size()

    ''' Cost function. Returns the loss of the best parse currently minus the loss of the best parse after making a transition step
    NOTE: There should always exist a t s.t. cost(t,c,G_gold) = 0
    '''
    def COST(self, t, c, G_gold):
        c2 = c.makeTransitionMove(t)
        
        currentBestParse = c.bestPossibleParse(G_gold)
        transitionBestParse = c2.bestPossibleParse(G_gold)

        return self.LOSS(currentBestParse, G_gold) - self.LOSS(transitionBestParse, G_gold)
    
    ''' If above functions are implemented, then the oracle step is simple
    '''
    def ORACLE(self, t, c, G_gold):
        return cost(t, c, G_gold) == 0

    # This should give a number. Like a score.
    def features(c, allTransitions):
        return 0

    def bestTransition(self, w, features(c, allTransitions)):
        # got to do an argmax, and then return the transition that is the best.
        #TODO
        return allTransitions[0]



    def TRAINING(self, corpus, iterations):
        # w is the featureWeight
        w = 0 
        
        for i in range(iterations):
            for sentence in corpus:
                c = sentence.initialConfiguration

                while not c.isTerminal:
                    predictedTransition_fromData = bestTransition(w, features(c, allTransitions))

                    # Get all zero cost transitions
                    zeroCostTransitions = []
                    for t in allTransitions:
                        if oracle(t, c, G_Gold):
                            zeroCost.append(t)

                    predictedTransition_fromOracle = bestTransition(w, features(c, zeroCostTransitions))

                    if predictedTransition_fromData != predictedTransition_fromOracle:
                        w = w + features(c, predictedTransition_fromOracle) - features(c, predictedTransition_fromData)

                        nextTransition = CHOOSE_NEXT(i, predictedTransition_fromData, zeroCost)
                        c = c.makeTransition(predictedTransition_fromData)
        return w




    def CHOOSE_NEXT(self, i, predictedTransition_fromData, zeroCost):
        k = 2
        p = 0.1

        return CHOOSE_NEXT_amb(i, predictedTransition_fromData, zeroCost)
        #return CHOOSE_NEXT_exp(i, predictedTransition_fromData, zeroCost, k, p)


    def CHOOSE_NEXT_amb(self, i, predictedTransition_fromData, zeroCost):
        if predictedTransition_fromData in zeroCost:
            return predictedTransition_fromData
        else:
            return getRandomElement(zeroCost)

        
    def CHOOSE_NEXT_exp(self, i, predictedTransition_fromData, zeroCost, k, p):
        if i > k and Rand() > p:
            return predictedTransition_fromData
        else:
            return CHOOSE_NEXT_amb(i, predictedTransition_fromData, zeroCost)








if __name__ == "__main__":
    parse = Parse(True)
    transition = Transition()
    oracle = Oracle()



    sentence = Sentence(None, None)
    
 
