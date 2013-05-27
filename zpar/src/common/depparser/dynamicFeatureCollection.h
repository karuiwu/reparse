/*
 * dynamicFeatureCollection.h
 *
 *  Created on: May 25, 2013
 *      Author: augustofheart
 */

#ifndef DYNAMICFEATURECOLLECTION_H_
#define DYNAMICFEATURECOLLECTION_H_

#include <vector>
#include <map>
#include <iostream>
#include <stdlib.h>
#include <sstream>

enum { ARCFEATURES = 0 };

typedef std::map<int, std::vector<int> > siblings_t;
typedef std::map<int, int> parent_t;

class DynamicFeatureCollection {
public:
	DynamicFeatureCollection();
	virtual ~DynamicFeatureCollection();
	void makeFeatures(std::vector<int> stack, std::vector<int> buffer, siblings_t children);//, parent_t parents);
	void arcFeatures();
	void readInFeature(std::vector<std::string> feature);
	void printFeatures();
	void clear();

	std::vector<std::vector<std::string> > features;
	int ngram;
	std::vector<int> m_stack;
	std::vector<int> m_buffer;
	siblings_t m_children;
	parent_t m_parents;

};

#endif /* DYNAMICFEATURECOLLECTION_H_ */
