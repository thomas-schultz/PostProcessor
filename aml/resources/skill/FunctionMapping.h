/*
 * FunctionMapping.h
 *
 *  Created on: 26.02.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef SRC_AML_RESOURCES_FUNCTIONMAPPING_H_
#define SRC_AML_RESOURCES_FUNCTIONMAPPING_H_

#include <map>
#include <vector>

#include "tools/String.h"
#include "rapidxml-1.13/rapidxml.hpp"

using namespace std;

namespace aml {

/**
 * Class that represents the mapping between a Blackboard Skill and a Blackboard Function.
 */
class FunctionMapping {
private:
	String id;
	String name;
	int order;
	map<String, String> parameters;

	bool readInParamter(rapidxml::xml_node<> *node, String name);

public:
	FunctionMapping(rapidxml::xml_node<>* node);
	virtual ~FunctionMapping();

	String getId();
	String getName();
	int getOrder();
	vector<String> getParameterNames();
	String getMappedName(String name);
};

} /* namespace aml */

#endif /* SRC_AML_RESOURCES_FUNCTIONMAPPING_H_ */
