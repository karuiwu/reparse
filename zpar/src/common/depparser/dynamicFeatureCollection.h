/*
 * dynamicFeatureCollection.h
 *
 *  Created on: May 25, 2013
 *      Author: augustofheart
 */

#ifndef DYNAMICFEATURECOLLECTION_H_
#define DYNAMICFEATURECOLLECTION_H_

#include <vector>

class dynamicFeatureCollection {
public:
	dynamicFeatureCollection();
	virtual ~dynamicFeatureCollection();
	void makeFeatures();

	std::vector<std::string> features;
};

#endif /* DYNAMICFEATURECOLLECTION_H_ */
