/*
 * SFCMapping.h
 *
 *  Created on: 24.02.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef SRC_AML_RESOURCES_SFCMAPPING_H_
#define SRC_AML_RESOURCES_SFCMAPPING_H_

#include <map>

#include "Parameter.h"

#include "tools/String.h"
#include "rapidxml-1.13/rapidxml.hpp"

using namespace std;

namespace aml {

/**
 * Class that represents the mapping between a Blackboard Skill and the sfc-file.
 */
class SFCMapping {
private:
	map<String, Parameter*> parameters;
	bool readInParamter(rapidxml::xml_node<> *node, String name, String trans);

public:
	SFCMapping(rapidxml::xml_node<> *node);
	virtual ~SFCMapping();

	String getMappedName(String name);
	int getMappedPos(String name);
};

} /* namespace aml */

#endif /* SRC_AML_RESOURCES_SFCMAPPING_H_ */
