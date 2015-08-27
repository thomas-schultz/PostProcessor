/*
 * Skill.h
 *
 *  Created on: 24.01.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef SRC_AML_SKILL_H_
#define SRC_AML_SKILL_H_

#include <map>
#include <set>
#include <vector>

#include "tools/String.h"
#include "Function.h"
#include "SFCMapping.h"
#include "FunctionMapping.h"
#include "aml/resources/Attribute.h"
#include "aml/resources/Resource.h"
#include "sfc/skillStep/SkillStep.h"
#include "rapidxml-1.13/rapidxml.hpp"

using namespace std;

namespace aml {

class Resource;

/**
 * Class that represents a Skill in the Blackboard.
 */
class Skill {
private:
	String name;
	String id;
	Resource *resource;
	bool mapped;
	SFCMapping *sfc_mapping;
	map<String, FunctionMapping*> func_mapping;
	map<String, Attribute*> parameter;

	bool readInParamter(rapidxml::xml_node<> *node, String name);

public:
	Skill(Resource *resource, rapidxml::xml_node<> *node);
	virtual ~Skill();

	String getName();
	String getId();
	bool isMapped();
	vector<String> getFunctions();
	FunctionMapping* getFunctionMapping(String name);
	void setParameters(sfc::SkillStep *step, map<String, String> &paras);
	String toStr();
};

} /* namespace aml */

#endif /* SRC_AML_SKILL_H_ */
