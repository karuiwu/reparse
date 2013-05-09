/*
 * dependenciesCollection.h
 *
 *  Created on: May 7, 2013
 *      Author: augustofheart
 */

#ifndef DEPENDENCIESCOLLECTION_H_
#define DEPENDENCIESCOLLECTION_H_

#include <vector>
#include <map>
#include <string>

enum { CONLLFIELD_ID, CONLLFIELD_FORM, CONLLFIELD_LEMMA,
	   CONLLFIELD_CPOSTAG, CONLLFIELD_POSTAG, CONLLFIELD_FEATS,
	   CONLLFIELD_HEAD, CONLLFIELD_DEPREL, CONLLFIELD_PHEAD,
	   CONLLFIELD_PDEPREL };

typedef std::map<int, std::vector<int> > siblings_t;
typedef std::map<int, int> parent_t;
typedef std::vector<std::vector<std::string> > sentence_t;
typedef std::pair<int, int> relation_t;

class DependenciesCollection {
  public:
    DependenciesCollection() {
    	  leftChildren.push_back(siblings_t());
    	  rightChildren.push_back(siblings_t());
    	  parents.push_back(parent_t());
    	  numSentences = 0;
    }
    //~DependenciesCollection();
    siblings_t getLeftChildren(int sentence);
    siblings_t getRightChildren(int sentence);
    parent_t getParent(int sentence);
    void addAll(sentence_t conllSentence);
    void add(int parentID, int childID);
    void newSentence();

    std::vector<siblings_t> leftChildren;
    std::vector<siblings_t> rightChildren;
    std::vector<parent_t> parents;
    int numSentences;
};

#endif /* DEPENDENCIESCOLLECTION_H_ */
