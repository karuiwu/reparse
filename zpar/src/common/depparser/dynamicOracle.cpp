// basic file operations
#include "dynamicOracle.h"

/**
 * Reads in the file containing CoNLL training data
 * @param file
 */
void DynamicOracle::readInSentence(CCoNLLInput conllData) {
	int sentenceNum = conllSentences.size();
	conllSentences.push_back(std::vector<std::vector<std::string> >());
	std::stringstream convert;

	for (int i=0; i < conllData.size(); i++) {
		CCoNLLInputNode word = conllData.at(i);
		std::vector<std::string> word_fields;
		convert << word.id;
		word_fields.push_back(convert.str());
		convert.str("");
		convert.clear();
		word_fields.push_back(word.word);
		word_fields.push_back(word.lemma);
		word_fields.push_back(word.ctag);
		word_fields.push_back(word.tag);
		word_fields.push_back(word.feats);
		convert << word.head;
		word_fields.push_back(convert.str());
		convert.str("");
		convert.clear();

		conllSentences.at(sentenceNum).push_back(word_fields);
	}

	dependencies.addAll(conllSentences.at(sentenceNum));
	if (DEBUG) {
		  std::cout << "parents: " << std::endl;
		  parent_t parent = dependencies.getParent(sentenceNum);
	      for(parent_t::const_iterator it = parent.begin(); it != parent.end(); it++)
	      {
	    	  std::cout << "(" << it->first << ", " << it->second << ")" << " ";
	      }
	      std::cout << std::endl;

		  std::cout << "right children: " << std::endl;
		  siblings_t rightChildren = dependencies.getRightChildren(sentenceNum);
	      for(siblings_t::const_iterator it = rightChildren.begin(); it != rightChildren.end(); it++)
	      {
	    	  std::cout << "(" << it->first << ", " << "{ ";
	    	  for(std::vector<int>::const_iterator vec_it = it->second.begin(); vec_it != it->second.end(); vec_it++) {
	    		  std::cout << *vec_it << " ";
	    	  }
		      std::cout << "}) ";
	      }
	      std::cout << std::endl;
	}
	dependencies.newSentence();
}

std::vector<int> DynamicOracle::nextAction(std::vector<int> stack, std::vector<int> buffer) {
	currentSentenceNum = conllSentences.size()-1;

	if (DEBUG) {
		std::cout << "Stack: ";
		for (int i=0; i < stack.size(); i++) {
			std::cout << stack.at(i) << " ";
		}
		std::cout << std::endl;
		std::cout << "Queue: ";
		for (int i=0; i < buffer.size(); i++) {
			std::cout << buffer.at(i) << " ";
		}
		std::cout << std::endl;
	}

	std::vector<int> costs;
	costs.push_back(costOfShift(stack, buffer));
	costs.push_back(costOfReduce(stack, buffer));
	costs.push_back(costOfLeftArc(stack, buffer));
	costs.push_back(costOfRightArc(stack, buffer));

	int minimumCost = *std::min_element(costs.begin(), costs.end());
	std::vector<int> actions;
	for (int i = 0; i < costs.size(); i++) {
		if (costs.at(i) == minimumCost) {
			actions.push_back(i+1);
		}
	}

	noReduce = false;
	return actions;
	//return std::vector<int>();
}

int DynamicOracle::costOfLeftArc(std::vector<int> stack, std::vector<int> buffer) {
  //need at least two elements, one of the stack and one of the buffer
  if (stack.empty() || buffer.empty()) {
	if (DEBUG) {
		std::cout << "Cost of Left-Arc: " << INT_MAX_ORACLE << std::endl;
	}
    return INT_MAX_ORACLE;
  }
  else {
    int topOfStack = stack.at(stack.size()-1);
    int cost = 0;
	//means all arcs of top element on stack have been made
	//in this situation, the arc between its true parent can no longer be made
    parent_t parent = dependencies.getParent(currentSentenceNum);
    parent_t::iterator parentID_it = parent.find(topOfStack);
    if (parentID_it == parent.end()) {
      throw std::string("ERROR: Word doesn't contain a parent. Check training data.");
    }
    else {
      int parentID = parentID_it->second;
      if (parentID != buffer.at(0) && std::find(buffer.begin(), buffer.end(), parentID) != buffer.end()) {
        if (DEBUG) {
          std::cout << "Incorrect parent" << std::endl;
        }
        cost++;
      }
    }
	//left-arcing means all right children arcs of top element of stack have been made
	//any missed arcs are added to the costs
    siblings_t rightChildren = dependencies.getRightChildren(currentSentenceNum);
    siblings_t::iterator rightChildrenIDs_it = rightChildren.find(topOfStack);
    if (rightChildrenIDs_it != rightChildren.end()) {
      if (DEBUG) {
    	  std::cout << "Missing children" << std::endl;
      }
      std::vector<int> rightChildrenIDs = rightChildrenIDs_it->second;
      std::vector<int> rightChildrenInBuffer;
      /*std::sort(buffer.begin(), buffer.end());
      std::sort(rightChildrenIDs.begin(), rightChildrenIDs.end());

      std::set_difference(buffer.begin(), buffer.end(),
    		  rightChildrenIDs.begin(), rightChildrenIDs.end(), rightChildrenInBuffer.begin());
*/
      cost += rightChildrenInBuffer.size();
    }
    if (DEBUG) {
    	std::cout << "Cost of Left-Arc: " << cost << std::endl;
    }

    return cost;
  }
}

int DynamicOracle::costOfRightArc(std::vector<int> stack, std::vector<int> buffer) {
  //need at least two elements, one of the stack and one of the buffer
  if (stack.empty() || buffer.empty()) {
	if (DEBUG) {
		std::cout << "Cost of Right-Arc: " << INT_MAX_ORACLE << std::endl;
	}
    return INT_MAX_ORACLE;
  }
  else {
    int topOfBuffer = buffer.at(0);
    int cost = 0;
	//means all arcs of top element on buffer have been made
	//in this situation, the arc between its true parent can no longer be made
    parent_t parent = dependencies.getParent(currentSentenceNum);
    parent_t::iterator parentID_it = parent.find(topOfBuffer);
    if (parentID_it == parent.end()) {
      std::cout << topOfBuffer << std::endl;
      throw std::string("ERROR: Word doesn't contain a parent. Check training data.");
    }
    else {
      int parentID = parentID_it->second;
      if ((parentID != stack.at(0) && std::find(stack.begin(), stack.end(), parentID) != stack.end()) ||
    		  std::find(buffer.begin(), buffer.end(), parentID) != buffer.end()) {
        if (DEBUG) {
          std::cout << "Incorrect parent" << std::endl;
        }
        cost++;
      }
    }
	//right-arcing means all left children arcs of shifted element have been made
	//any missed arcs are added to the costs
    siblings_t leftChildren = dependencies.getLeftChildren(currentSentenceNum);
    siblings_t::iterator leftChildrenIDs_it = leftChildren.find(topOfBuffer);
    if (leftChildrenIDs_it != leftChildren.end()) {
      if (DEBUG) {
    	  std::cout << "Missing children" << std::endl;
      }
      //std::vector<int> leftChildrenIDs = leftChildrenIDs_it->second;
      std::vector<int> leftChildrenInBuffer;
      /*std::sort(buffer.begin(), buffer.end());
      std::sort(leftChildrenIDs.begin(), leftChildrenIDs.end());
      std::vector<int>::iterator rightChildrenInBuffer_it = std::set_difference(buffer.begin(), buffer.end(),
    		  leftChildrenIDs.begin(), leftChildrenIDs.end(), leftChildrenInBuffer.begin());
*/
      cost += leftChildrenInBuffer.size();
    }
    if (DEBUG) {
    	std::cout << "Cost of Right-Arc: " << cost << std::endl;
    }
    return cost;
  }
}

int DynamicOracle::costOfReduce(std::vector<int> stack, std::vector<int> buffer) {
  //if there aren't at least two elements on the stack, we can't reduce
  if (stack.size() < 2 || noReduce) {
	if (DEBUG) {
		std::cout << "Cost of Reduce: " << INT_MAX_ORACLE << std::endl;
	}
    return INT_MAX_ORACLE;
  }
  else {
    int topOfStack = stack.at(stack.size()-1);
    int cost = 0;
	//reducing means all right children arcs have been made
	//any missed arcs are added to the costs
    siblings_t rightChildren = dependencies.getRightChildren(currentSentenceNum);
    siblings_t::iterator rightChildrenIDs_it = rightChildren.find(topOfStack);
    if (rightChildrenIDs_it != rightChildren.end()) {
      if (DEBUG) {
    	  std::cout << "Missing children" << std::endl;
      }
      //std::vector<int> rightChildrenIDs = rightChildrenIDs_it->second;
      std::vector<int> rightChildrenInBuffer;
      /*std::sort(buffer.begin(), buffer.end());
      std::sort(rightChildrenIDs.begin(), rightChildrenIDs.end());
      std::vector<int>::iterator rightChildrenInBuffer_it = std::set_difference(buffer.begin(), buffer.end(),
    		  rightChildrenIDs.begin(), rightChildrenIDs.end(), rightChildrenInBuffer.begin());
*/
      cost += rightChildrenInBuffer.size();
    }
    if (DEBUG) {
    	std::cout << "Cost of Reduce: " << cost << std::endl;
    }

    return cost;
  }
}

int DynamicOracle::costOfShift(std::vector<int> stack, std::vector<int> buffer) {
	//need at least two elements, one of the stack and one of the buffer
  if (buffer.empty()) {
	if (DEBUG) {
		std::cout << "Cost of Shift: " << INT_MAX_ORACLE << std::endl;
	}
    return INT_MAX_ORACLE;
  }
  else {
    int topOfBuffer = buffer.at(0);
    int cost = 0;
	//means both shifted element and its parent are on the stack
	//in this situation, the arc between the element and its parent can no longer be made
    parent_t parent = dependencies.getParent(currentSentenceNum);
    parent_t::iterator parentID_it = parent.find(topOfBuffer);
    if (parentID_it == parent.end()) {
      throw "ERROR: Word doesn't contain a parent. Check training data.";
    }
    else {
      int parentID = parentID_it->second;
      if (std::find(stack.begin(), stack.end(), parentID) != stack.end()) {
        if (DEBUG) {
          std::cout << "Incorrect parent" << std::endl;
        }
        cost++;
      }
    }
	//shifting means all left children arcs of shifted element have been made
	//any missed arcs are added to the costs
    siblings_t leftChildren = dependencies.getLeftChildren(currentSentenceNum);
    siblings_t::iterator leftChildrenIDs_it = leftChildren.find(topOfBuffer);
    if (leftChildrenIDs_it != leftChildren.end()) {
      if (DEBUG) {
    	  std::cout << "Missing children" << std::endl;
      }
      //std::vector<int> leftChildrenIDs = leftChildrenIDs_it->second;
      std::vector<int> leftChildrenInBuffer;
      /*std::sort(buffer.begin(), buffer.end());
      std::sort(leftChildrenIDs.begin(), leftChildrenIDs.end());
      std::vector<int>::iterator rightChildrenInBuffer_it = std::set_difference(buffer.begin(), buffer.end(),
    		  leftChildrenIDs.begin(), leftChildrenIDs.end(), leftChildrenInBuffer.begin());
*/
      cost += leftChildrenInBuffer.size();
    }
    if (DEBUG) {
    	std::cout << "Cost of Shift: " << cost << std::endl;
    }

    return cost;
  }
}

bool DynamicOracle::isOracleAction(std::vector<int> oracleActions, int actionCode) {
  for (int i = 0; i < oracleActions.size(); i++) {
  	if (actionCode == oracleActions.at(i)) {
  		return true;
  	}
  }

  return false;
}
