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
  for (int i=0; i < conllSentence.size(); i++) {
    int parentID = conllSentence.at(CONLLFIELD_HEAD);
    int childID = CONLLFIELD_ID;

    add(parentID, childID);
  }
}

void DependenciesCollection::add(int parentID, int childID) {
  parents.at(numSentences).insert(relation_t(childID, parentID));

  if (childID > parentID) {
	siblings_t::iterator parent_right_it = rightChildren.at(numSentences).find(parentID);
    if (parent_right_it == rightChildren.at(numSentences).end()) {
      rightChildren.at(numSentences).insert(relation_t(parentID, std::vector<int>()));
    }
    parent_right_it->second.push_back(childID);
  }
  else if (childID < parentID) {
	siblings_t::iterator parent_left_it = leftChildren.at(numSentences).find(parentID);
    if (parent_left_it == leftChildren.at(numSentences).end()) {
      leftChildren.at(numSentences).insert(relation_t(parentID, std::vector<int>()));
    }
    parent_left_it->second.push_back(childID);
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
