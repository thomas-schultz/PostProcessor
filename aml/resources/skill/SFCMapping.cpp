/*
 * SkillMapping.cpp
 *
 *  Created on: 24.02.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include <cstdlib>

#include "SFCMapping.h"

#include "exceptions/Exception.h"
#include "aml/resources/Attribute.h"
#include "tools/XML.h"

using namespace std;

namespace aml {

/**
 * Creates a new sfc-mapping from given xml-node.
 * @param node
 */
SFCMapping::SFCMapping(rapidxml::xml_node<> *node) {
	readInParamter(node, "", "");
}

/**
 * Destructor
 */
SFCMapping::~SFCMapping() {
	for (map<String, Parameter*>::iterator it = parameters.begin(); it != parameters.end(); ++it)
		delete ((*it).second);
}

/**
 * Extracts all information from the xml-node. Function is recursive,
 * to handle global names for nested attributes.
 * @param node
 * @param name
 * @param trans
 * @return
 */
bool SFCMapping::readInParamter(rapidxml::xml_node<>* node, String name, String trans) {
	list<rapidxml::xml_node<>*> subattr = XML::findNodes(node, "Attribute");
	if (subattr.size() == 0)
		return false;
	for (list<rapidxml::xml_node<>*>::iterator it = subattr.begin(); it != subattr.end(); ++it) {
		rapidxml::xml_node<> *cur = (*it);
		Attribute attrib = Attribute(cur);
		String aname = name + attrib.getName();
		String value = attrib.getValue();
		if (readInParamter(cur, aname + ".", value))
			continue;
		Parameter *par;
		if (trans.size() == 0) {
			par = new Parameter(aname, value, -1);
		} else {
			int index = atoi(value.c_str());
			par = new Parameter(aname, trans, index);
		}
		this->parameters.insert(pair<String, Parameter*>(par->getName(), par));
	}
	return true;
}

/**
 * Returns the mapped parameter name for a given parameter.
 * @param name	name inside the Blackboard
 * @return		name inside the sfc-file
 * throws Exception if not found
 */
String SFCMapping::getMappedName(String name) {
	map<String, Parameter*>::iterator obj = parameters.find(name);
	if (obj == parameters.end())
		throw except::Exception("Cannot map attribute to sfc, '" + name + "' not found");
	return obj->second->getTranslation();
}

/**
 * Returns the position index for a given parameter for nested parameters.
 * @param name	name inside the Blackboard
 * @return		pos inside the variable list in the sfc-file
 * throws Exception if not found
 */
int SFCMapping::getMappedPos(String name) {
	map<String, Parameter*>::iterator obj = parameters.find(name);
	if (obj == parameters.end())
		throw except::Exception("Cannot map attribute position, '" + name + "' not found");
	return obj->second->getPos();
}

} /* namespace aml */
