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
#include <iostream>
#include <stdlib.h>

enum { CONLLFIELD_ID = 0, CONLLFIELD_FORM, CONLLFIELD_LEMMA,
	   CONLLFIELD_CPOSTAG, CONLLFIELD_POSTAG, CONLLFIELD_FEATS,
	   CONLLFIELD_HEAD, CONLLFIELD_DEPREL, CONLLFIELD_PHEAD,
	   CONLLFIELD_PDEPREL };

typedef std::map<int, std::vector<int> > siblings_t;
typedef std::map<int, int> parent_t;
typedef std::vector<std::vector<std::string> > sentence_t;
typedef std::pair<int, int> parent_relation_t;
typedef std::pair<int, std::vector<int> > sibling_relation_t;

#define DEBUG true

class DependenciesCollection {
  public:
    DependenciesCollection() {
    	  leftChildren.push_back(std::map<int, std::vector<int> >());
    	  rightChildren.push_back(std::map<int, std::vector<int> >());
    	  parents.push_back(std::map<int,int>());
    	  numSentences = 0;
    }
    ~DependenciesCollection() {};
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
