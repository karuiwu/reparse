/*
 * dynamicFeatureCollection.h
 *
 *  Created on: May 25, 2013
 *      Author: augustofheart
 */

#ifndef DYNAMICFEATURECOLLECTION_H_
#define DYNAMICFEATURECOLLECTION_H_

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

#include "linguistics/conll.h"

#include "../../libs/vowpalwabbit/parser.h"
#include "../../libs/vowpalwabbit/vw.h"

enum { ARCFEATURES = 0 };

typedef std::map<int, std::vector<int> > siblings_t;
typedef std::map<int, int> parent_t;

class DynamicFeatureCollection {
public:
	DynamicFeatureCollection();
	virtual ~DynamicFeatureCollection();
	void makeFeatures(std::vector<int> stack, std::vector<int> buffer,
			siblings_t children, std::vector<CCoNLLCPOS> tags);//, parent_t parents);
	void posFeatures();
	void arcFeatures();
	void readInFeature(std::vector<std::string> feature);
	void readToMap(std::string fileName = "map.txt");
	void writeToMap(CCoNLLOutput conllSentenceTrain);
	void writeToMap(std::string fileName = "map.txt");
	void printFeatures();
	void clear();

	std::vector<std::vector<std::string> > features;
	int ngram;

	std::map<std::string, std::vector<std::string> > leftTags;
	std::map<std::string, std::vector<std::string> > rightTags;

	std::vector<int> m_stack;
	std::vector<int> m_buffer;
	siblings_t m_children;
	parent_t m_parents;
	std::vector<CCoNLLCPOS> m_tags;
};

#endif /* DYNAMICFEATURECOLLECTION_H_ */
