/*
 * dynamicFeatureCollection.cpp
 *
 *  Created on: May 25, 2013
 *      Author: augustofheart
 */

#include "dynamicFeatureCollection.h"

// support functions
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;

    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

inline feature vw_feature_from_string(vw& v, string fstr, unsigned long seed, float val)
{
  uint32_t foo = VW::hash_feature(v, fstr, seed);
  feature f = { val, foo};
  return f;
}

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

	while (tagFeatures.size() < ngram) {
		tagFeatures.push_back("###");
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

void DynamicFeatureCollection::readToMap(std::string fileName) {
	std::ifstream childTagsFile(fileName.c_str());
	std::string line;
	bool rightChild = false;
	bool leftChild = false;
	bool tagCount = false;
	//std::string computing = "Computing averaged";
	while (std::getline(childTagsFile, line)) {
		if (line == "right children:") {
			rightChild = true;
			leftChild = false;
			tagCount = false;
			continue;
		}
		else if (line == "left children:") {
			leftChild = true;
			rightChild = false;
			tagCount = false;
			continue;
		}
		else if (line == "tag count:"){
			leftChild = false;
			rightChild = false;
			tagCount = true;
		}
		else if (!rightChild && !leftChild) {
			continue;
		}
		else if (line == "\n") {
			std::cout << "got here!" << std::endl;
			break;
		}

		if (rightChild && !leftChild) {
			std::vector<std::string> x = split(line, ')');
			for (int i = 0; i < x.size()-1; ++i) {
				std::vector<std::string> y = split(x.at(i), ',');
				std::vector<std::string> a;
				if (y.at(0) == "(") {
					a.push_back("");
					a.push_back("");
				}
				else {
					a = split(y.at(0), '(');
				}
				std::vector<std::string> z = split(y.at(1), ' ');
				rightTags.insert(std::map<std::string, std::vector<std::string> >::value_type(a.at(1), std::vector<std::string>()));
				for (int j = 2; j < z.size()-1; ++j) {
					rightTags[a.at(1)].push_back(z.at(j));
				}
			}
		}
		else if (leftChild && !rightChild) {
			std::vector<std::string> x = split(line, ')');
			for (int i = 0; i < x.size()-1; ++i) {
				std::vector<std::string> y = split(x.at(i), ',');
				std::vector<std::string> a;
				if (y.at(0) == "(") {
					a.push_back("");
					a.push_back("");
				}
				else {
					a = split(y.at(0), '(');
				}
				std::vector<std::string> z = split(y.at(1), ' ');
				leftTags.insert(std::map<std::string, std::vector<std::string> >::value_type(a.at(1), std::vector<std::string>()));
				for (int j = 2; j < z.size()-1; ++j) {
					leftTags[a.at(1)].push_back(z.at(j));
				}
			}
		}
//		else if(!leftChild && !rightChild && tagCount){
//
//		}
	}

	/*std::cout << "right children: " << std::endl;
	for (std::map<std::string, std::vector<std::string> >::const_iterator it = rightTags.begin(); it != rightTags.end(); ++it) {
		std::cout << "(" << it->first << ", " << "{ ";
		for (std::vector<std::string>::const_iterator vec_it = it->second.begin(); vec_it != it->second.end(); ++vec_it) {
			std::cout << *vec_it << " ";
		}
		std::cout << "}) ";
	}
	std::cout << std::endl;

	std::cout << "left children: " << std::endl;
	for (std::map<std::string, std::vector<std::string> >::const_iterator it = leftTags.begin(); it != leftTags.end(); ++it) {
		std::cout << "(" << it->first << ", " << "{ ";
		for (std::vector<std::string>::const_iterator vec_it = it->second.begin(); vec_it != it->second.end(); ++vec_it) {
			std::cout << *vec_it << " ";
		}
		std::cout << "}) ";
	}
	std::cout << std::endl;*/
	childTagsFile.close();
}

void DynamicFeatureCollection::writeToMap(std::string fileName) {
	std::ofstream childTagsFile(fileName.c_str());
	childTagsFile << "left children:" << std::endl;
	for (std::map<std::string, std::vector<std::string> >::const_iterator it = leftTags.begin(); it != leftTags.end(); ++it) {
		childTagsFile << "(" << it->first << ", " << "{ ";
		for (std::vector<std::string>::const_iterator vec_it = it->second.begin(); vec_it != it->second.end(); ++vec_it) {
			childTagsFile << *vec_it << " ";
		}
		childTagsFile << "}) ";
	}
	childTagsFile << "\n";

	childTagsFile << "right children:" << std::endl;
	for (std::map<std::string, std::vector<std::string> >::const_iterator it = rightTags.begin(); it != rightTags.end(); ++it) {
		childTagsFile << "(" << it->first << ", " << "{ ";
		for (std::vector<std::string>::const_iterator vec_it = it->second.begin(); vec_it != it->second.end(); ++vec_it) {
			childTagsFile << *vec_it << " ";
		}
		childTagsFile << "}) ";
	}
	childTagsFile << "\n";


//	childTagsFile << "tag counts:" <<std::endl;
//	for(std::map<std::string, std::vector<std::pair<std::string, int> > >::const_iterator it = tagCount.begin(); it != tagCount.end(); ++it){
//		childTagsFile << "(" << it->first << ", " << "{ ";
//		for (std::vector<std::pair<std::string, int> >::const_iterator vec_it = it->second.begin(); vec_it != it->second.end(); ++vec_it){
//			childTagsFile << vec_it->first << "|" << vec_it->second << " ";
//		}
//		childTagsFile << "}) ";
//	}
//	childTagsFile << "\n";


}

void DynamicFeatureCollection::writeToMap(CCoNLLOutput conllSentenceTrain) {
	for (int i = 1; i < conllSentenceTrain.size(); i++) {
		int childID = conllSentenceTrain.at(i).id;
		std::string tag = conllSentenceTrain.at(childID).tag;
		int parentID = conllSentenceTrain.at(i).head;
		std::string parent = conllSentenceTrain.at(parentID).word;

		//right child
		if (childID > parentID) {
			std::map<std::string, std::vector<std::string> >::iterator right_it = rightTags.find(parent);
			if (right_it == rightTags.end()) {
				rightTags.insert(std::map<std::string, std::vector<std::string> >::value_type(
						parent, std::vector<std::string>()));
			}
			rightTags[parent].push_back(tag);
		}
		else if (childID < parentID) {
			std::map<std::string, std::vector<std::string> >::iterator left_it = leftTags.find(parent);
			if (left_it == leftTags.end()) {
				leftTags.insert(std::map<std::string, std::vector<std::string> >::value_type(
						parent, std::vector<std::string>()));
			}
			leftTags[parent].push_back(tag);
		}
		else {
			// you shouldn't be here
		}

		// here we will populate tag counts.
		std::string word = conllSentenceTrain.at(i).word;

		std::map<std::string, std::vector<std::pair<std::string, int> > >::iterator it =
				tagCount.find(word);
		// if we haven't found the appropriate tagCount vector entry:
		if (it == tagCount.end()) {


			std::vector<std::pair<std::string, int> > vectorOfPairs = std::vector<std::pair<std::string, int> >();
			tagCount.insert(
					std::map<std::string,
							std::vector<std::pair<std::string, int> > >::value_type(
							word, vectorOfPairs));
		}
		std::vector<std::pair<std::string, int> > counts = tagCount[word];

		bool found = false;
		for (int j = 0; j < counts.size(); j++) {
			std::pair<std::string, int> pairs = counts[j];
			std::string individualTag = pairs.first;

			if (individualTag.compare(tag) == 0) {
				pairs.second++;
				found = true;
				break;
			}
		}
		if (!found) {
			std::pair<std::string, int> newPair = std::pair<std::string, int>(
					tag, 1);
			counts.push_back(newPair);
		}
	}






	/*std::cout << "left children: " << std::endl;
	for (std::map<std::string, std::vector<std::string> >::const_iterator it = leftTags.begin(); it != leftTags.end(); ++it) {
		std::cout << "(" << it->first << ", " << "{ ";
		for (std::vector<std::string>::const_iterator vec_it = it->second.begin(); vec_it != it->second.end(); ++vec_it) {
			std::cout << *vec_it << " ";
		}
		std::cout << "}) ";
	}
	std::cout << std::endl;

	std::cout << "right children: " << std::endl;
	for (std::map<std::string, std::vector<std::string> >::const_iterator it = rightTags.begin(); it != rightTags.end(); ++it) {
		std::cout << "(" << it->first << ", " << "{ ";
		for (std::vector<std::string>::const_iterator vec_it = it->second.begin(); vec_it != it->second.end(); ++vec_it) {
			std::cout << *vec_it << " ";
		}
		std::cout << "}) ";
	}
	std::cout << std::endl;*/
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

