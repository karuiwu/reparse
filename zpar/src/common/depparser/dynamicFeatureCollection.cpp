/*
 * dynamicFeatureCollection.cpp
 *
 *  Created on: May 25, 2013
 *      Author: augustofheart
 */

#include "dynamicFeatureCollection.h"

DynamicFeatureCollection::DynamicFeatureCollection() {
	ngram = 3;

}

DynamicFeatureCollection::~DynamicFeatureCollection() {
	// TODO Auto-generated destructor stub
}


/**
 * Feature Zero: Base features
 * - POS tags on the stack and buffer
 */
void DynamicFeatureCollection::makeFeatures(std::vector<int> stack, std::vector<int> buffer,
		siblings_t children, std::vector<CCoNLLCPOS> tags) {//, parent_t parents) {
	m_stack = stack;
	m_buffer = buffer;
	m_children = children;
	m_tags = tags;
	//m_parents = parents;

	posFeatures();
	arcFeatures();
}

void DynamicFeatureCollection::posFeatures() {
	int ngram = 3;
	// Here are the POS tags for the sentence.
//				std::cout << "m_lCacheCoNLLCPOS: ";
//				for (int j = 0; j < sentence.size(); j++) {
//					std::cout << m_lCacheCoNLLCPOS[j] << " ";
//				}
//				std::cout << std::endl;
//
//				std::cout << "pCandidate.Stack: ";
//				for (int j = 0; j < pCandidate.Stack.size(); j++) {
//					std::cout << pCandidate.Stack[j] << " ";
//				}
//				std::cout << std::endl;

	//std::cout << "Trigram of stack: ";
	std::vector<std::string> tagFeatures;
	int j = m_stack.size()-ngram;
	if (j < 0) {
		j = 0;
	}
	while (j < m_stack.size()) {
		int sentenceIndex = m_stack[j];
		CCoNLLCPOS sentenceTag = m_tags[sentenceIndex];
		tagFeatures.push_back(sentenceTag.str());
		j++;
	}



	features.push_back(tagFeatures);
}

void DynamicFeatureCollection::arcFeatures() {
	std::ostringstream convert;
	std::vector<std::vector<std::string> > arcs;
	std::vector<std::vector<std::string> > arcFeatures;
	for (int k = 0; k < m_stack.size(); ++k) {
		arcs.push_back(std::vector<std::string>());
		int item = m_stack.at(k);
		siblings_t::iterator it = m_children.find(item);
		if (it != m_children.end()) {
		  for (int c = 0; c < it->second.size(); c++) {
			  convert << item;//add number to the stream
			  std::string parent = convert.str();
			  convert.str("");
			  convert.clear();
			  convert << it->second.at(c);
			  std::string child = convert.str();
			  convert.str("");
			  convert.clear();
			  arcs.back().push_back("(" + child + ", " + parent + ")");
		  }
		}
		else {
			arcs.back().push_back("###");
		}
	}
	while (arcFeatures.size() < ngram && !arcs.empty()) {
		arcFeatures.push_back(arcs.back());
		arcs.pop_back();
	}
	while (arcFeatures.size() < ngram) {
		arcFeatures.push_back(std::vector<std::string>());
		arcFeatures.back().push_back("###");
	}

	features.push_back(std::vector<std::string>());
	for (int k=0; k < arcFeatures.size(); ++k) {
		for (int j=0; j < arcFeatures.at(k).size(); ++j) {
			features.back().push_back(arcFeatures.at(k).at(j));
		}
	}
}

void DynamicFeatureCollection::readInFeature(std::vector<std::string> feature) {
	features.push_back(feature);
}

void DynamicFeatureCollection::printFeatures() {
	for (int f = 0; f < features.size(); f++) {
		for (int g = 0; g < features.at(f).size(); g++) {
			std::cout << features.at(f).at(g);
			if (f < features.at(f).size()-1) {
				std::cout << " ";
			}
		}
		if (f < features.size()-1) {
			std::cout << "\t";
		}
	}
	std::cout << std::endl;
}

void DynamicFeatureCollection::clear() {
	features.clear();
}
