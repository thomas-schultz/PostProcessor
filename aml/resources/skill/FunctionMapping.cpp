/*
 * FunctionMapping.cpp
 *
 *  Created on: 26.02.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include <cstdlib>

#include "FunctionMapping.h"

#include "CLI.h"
#include "aml/resources/Attribute.h"
#include "tools/XML.h"
#include "exceptions/Exception.h"
#include "rapidxml-1.13/rapidxml.hpp"

using namespace std;

namespace aml {

/**
 * Creates a new FunctionMapping from given xml-node.
 * @param node
 */
FunctionMapping::FunctionMapping(rapidxml::xml_node<>* node) {
	this->id = XML::getAttribute(node, "ID", "");
	rapidxml::xml_node<>* attrib = XML::findNode(node, "Attribute", "Name", "Name");
	try {
		this->name = Attribute(attrib).getValue();
	} catch (except::Exception &e) {
		throw except::Exception("Missing function name in FunctionMapping");
	}
	attrib = XML::findNode(node, "Attribute", "Name", "Order");
	try {
		this->order = atoi(Attribute(attrib).getValue().c_str());
	} catch (except::Exception &e) {
		this->order = 0;
	}
	rapidxml::xml_node<>* mapping = XML::findNode(node, "Attribute", "Name", "Mapping");
	readInParamter(mapping, "");
}

/**
 * Destructor
 */
FunctionMapping::~FunctionMapping() {
	// TODO Auto-generated destructor stub
}

/**
 * Extracts all information from the xml-node. Function is recursive,
 * to generate global names for nested attributes.
 * @param node
 * @param name	global name prefix
 * @return
 */
bool FunctionMapping::readInParamter(rapidxml::xml_node<>* node, String name) {
	list<rapidxml::xml_node<>*> subattr = XML::findNodes(node, "Attribute");
	if (subattr.size() == 0)
		return false;
	for (list<rapidxml::xml_node<>*>::iterator it = subattr.begin(); it != subattr.end(); ++it) {
		rapidxml::xml_node<> *cur = (*it);
		String map_name = XML::getAttribute(cur, "Name", "");
		String full_name = name + map_name;
		if (readInParamter(cur, full_name + "."))
			continue;
		rapidxml::xml_node<> *val = XML::findNode(cur, "Value");
		if (val != NULL)
			map_name = val->value();
		this->parameters.insert(pair<String, String>(map_name.toUpper(), full_name));
	}
	return true;
}

/**
 * Basic getter function.
 * @return
 */
String FunctionMapping::getId() {
	return this->id;
}

/**
 * Basic getter function.
 * @return
 */
String FunctionMapping::getName() {
	return this->name;
}

/**
 * Basic getter function.
 * @return
 */
int FunctionMapping::getOrder() {
	return this->order;
}

/**
 * Returns a list of all parameter of this mapping.
 * @return
 */
vector<String> FunctionMapping::getParameterNames() {
	vector<String> names;
	for (map<String, String>::iterator it = parameters.begin(); it != parameters.end(); ++it)
		names.push_back((*it).first);
	return names;
}

/**
 * Returns the mapped name of the parameter with given name.
 * @param name
 * @return
 * throws Exception	if not found
 */
String FunctionMapping::getMappedName(String name) {
	map<String, String>::iterator obj = parameters.find(name);
	if (obj == parameters.end())
		throw except::Exception("Cannot find corresponding parameter of '" + name + "'");
	return obj->second;
}

} /* namespace aml */
