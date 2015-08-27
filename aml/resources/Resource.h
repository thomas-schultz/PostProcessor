/*
 * Resource.h
 *
 *  Created on: 24.01.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef SRC_AML_RESOURCES_RESOURCE_H_
#define SRC_AML_RESOURCES_RESOURCE_H_

#include <map>

#include "skill/Skill.h"
#include "skill/Function.h"
#include "aml/irl/IRLFile.h"
#include "tools/String.h"

using namespace std;

namespace aml {

class Blackboard;

class Skill;

/**
 * Class that represents a resource object in the Blackboard.
 */
class Resource {
private:
	String name;
	String id;
	Blackboard *blackboard;
	rapidxml::xml_node<> *node;
	map<String, Skill*> skills;
	map<String, Function*> functions;
	map<String, IRLFile*> irls;
	int errors;

public:
	Resource();
	Resource(Blackboard *blackboard, rapidxml::xml_node<> *node);
	virtual ~Resource();

	void error();
	int getErrors();
	String getName();
	String getId();
	Skill* getSkill(String id);
	Function* getFunction(String id);
	IRLFile* getIRLFile(String path);
	vector<String> getMappedIds(String id);
	String stStr();
};

} /* namespace aml */

#endif /* SRC_AML_RESOURCES_RESOURCE_H_ */
