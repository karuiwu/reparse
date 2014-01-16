#include "dependenciesCollection.h"

/*DependenciesCollection::DependenciesCollection() {
  leftChildren.push_back(siblings_t());
  rightChildren.push_back(siblings_t());
  parents.push_back(parent_t());
  numSentences = 0;
}*/

siblings_t DependenciesCollection::getLeftChildren(int sentence) {
  return leftChildren.at(sentence);
}

siblings_t DependenciesCollection::getRightChildren(int sentence) {
  return rightChildren.at(sentence);
}

parent_t DependenciesCollection::getParent(int sentence) {
  return parents.at(sentence);
}

void DependenciesCollection::addAll(sentence_t conllSentence) {
  for (unsigned int i=0; i < conllSentence.size(); i++) {
    int parentID = atoi(conllSentence.at(i).at(CONLLFIELD_HEAD).c_str());
    int childID = atoi(conllSentence.at(i).at(CONLLFIELD_ID).c_str());

    add(parentID, childID);
  }
}

void DependenciesCollection::add(int parentID, int childID) {
  parents.at(numSentences).insert(parent_t::value_type(childID, parentID));

  if (childID > parentID) {
	siblings_t rightSiblings = rightChildren.at(numSentences);
	siblings_t::iterator parent_right_it = rightSiblings.find(parentID);
    if (parent_right_it == rightSiblings.end()) {
      rightChildren.at(numSentences).insert(siblings_t::value_type(parentID, std::vector<int>()));
    }
    rightChildren.at(numSentences)[parentID].push_back(childID);
  }
  else if (childID < parentID) {
	siblings_t leftSiblings = leftChildren.at(numSentences);
	siblings_t::iterator parent_left_it = leftSiblings.find(parentID);
    if (parent_left_it == leftSiblings.end()) {
      leftChildren.at(numSentences).insert(siblings_t::value_type(parentID, std::vector<int>()));
    }
    leftChildren.at(numSentences)[parentID].push_back(childID);
  }
  else {
    //Shouldn't be here
	//Means that parentID == childID, or we're comparing the same words in a sentence
  }
}

void DependenciesCollection::newSentence() {
  leftChildren.push_back(siblings_t());
  rightChildren.push_back(siblings_t());
  parents.push_back(parent_t());
  numSentences++;
}
