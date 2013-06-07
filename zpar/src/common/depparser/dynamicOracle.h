/*
 * dynamicOracle.h
 *
 *  Created on: May 7, 2013
 *      Author: augustofheart
 */

#ifndef DYNAMICORACLE_H_
#define DYNAMICORACLE_H_

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <limits>
#include <sstream>
#include "dependenciesCollection.h"
#include "linguistics/conll.h"

#define INT_MAX_ORACLE std::numeric_limits<int>::max()

class DynamicOracle {
  public:
    DynamicOracle() {
      noReduce = false;
    };
    ~DynamicOracle() {};
    void readInSentence(CCoNLLInput conllData);
    std::vector<int> nextAction(std::vector<int> stack, std::vector<int> buffer);
    int costOfLeftArc(std::vector<int> stack, std::vector<int> buffer);
    int costOfRightArc(std::vector<int> stack, std::vector<int> buffer);
    int costOfReduce(std::vector<int> stack, std::vector<int> buffer);
    int costOfShift(std::vector<int> stack, std::vector<int> buffer);
    bool isOracleAction(std::vector<int> oracleActions, int actionCode);

    std::vector<sentence_t> conllSentences;
    DependenciesCollection dependencies;
	int currentSentenceNum;
	bool noReduce;
	std::vector<int> counter;
};

#endif /* DYNAMICORACLE_H_ */
