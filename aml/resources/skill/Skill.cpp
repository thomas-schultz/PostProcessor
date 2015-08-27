/*
 * Skill.cpp
 *
 *  Created on: 24.01.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include <sstream>
#include <iomanip>

#include "Skill.h"

#include "CLI.h"
#include "sfc/variable/Variable.h"
#include "exceptions/Exception.h"
#include "tools/XML.h"

namespace aml {

/**
 * Creates a new Skill from given xml-node.
 * @param resource
 * @param node
 */
Skill::Skill(Resource *resource, rapidxml::xml_node<> *node) {
	this->name = XML::getAttribute(node, "Name", "");
	this->id = String(XML::getAttribute(node, "ID", "")).subString('{', '}');
	this->resource = resource;
	this->mapped = false;
	try {
		this->sfc_mapping = new SFCMapping(node);
		this->mapped = true;
	} catch (except::Exception &e) {
		CLI::printWarn("Invalid SFC Mapping for '" + name + "' in '" + resource->getName() + "': " + e.getMessage());
	}
	list<rapidxml::xml_node<>*> fun_maps = XML::findNodes(node, "ExternalInterface", "RefBaseClassPath",
			"IWB StateModelInterfaceLib/SkillMapping");
	for (list<rapidxml::xml_node<>*>::iterator it = fun_maps.begin(); it != fun_maps.end(); ++it) {
		FunctionMapping *fun_map = new FunctionMapping(*it);
		this->func_mapping.insert(pair<String, FunctionMapping*>(fun_map->getName(), fun_map));
	}
	readInParamter(node, "");
}

/**
 * Destructor
 */
Skill::~Skill() {
	delete (this->sfc_mapping);
}

/**
 * Basic getter function.
 * @return
 */
String Skill::getName() {
	return this->name;
}

/**
 * Basic getter function.
 * @return
 */
String Skill::getId() {
	return this->id;
}

/**
 * Checks if the skill is properly mapped.
 * @return
 */
bool Skill::isMapped() {
	return this->mapped;
}

/**
 * Returns a list of mapped function names inside this skill.
 * @return	empty list if nothing found
 */
vector<String> Skill::getFunctions() {
	map<int, String> funs;
	for (map<String, FunctionMapping*>::iterator it = func_mapping.begin(); it != func_mapping.end(); ++it) {
		FunctionMapping *mapping = (*it).second;
		funs.insert(pair<int, String>(mapping->getOrder(), mapping->getName()));
	}
	vector<String> names;
	for (map<int, String>::iterator it = funs.begin(); it != funs.end(); ++it)
		names.push_back((*it).second);
	return names;
}

/**
 * Extracts all parameters information from the xml-node. Function is recursive,
 * to handle global names for nested attributes.
 * @param node
 * @param name
 * @return
 */
bool Skill::readInParamter(rapidxml::xml_node<>* node, String name) {
	list<rapidxml::xml_node<>*> subattr = XML::findNodes(node, "Attribute");
	if (subattr.size() == 0)
		return false;
	for (list<rapidxml::xml_node<>*>::iterator it = subattr.begin(); it != subattr.end(); ++it) {
		rapidxml::xml_node<> *cur = (*it);
		Attribute *attrib = new Attribute(cur);
		String full_name = name + attrib->getName();
		if (readInParamter(cur, full_name + "."))
			continue;
		this->parameter.insert(pair<String, Attribute*>(full_name, attrib));
	}
	return true;
}

/**
 * Returns the FunctionMapping object with given name.
 * @param name
 * @return	NULL if not found
 */
FunctionMapping* Skill::getFunctionMapping(String name) {
	map<String, FunctionMapping*>::iterator obj = func_mapping.find(name);
	if (obj == func_mapping.end())
		return NULL;
	else
		return obj->second;
}

/**
 * Sets the parameter of a given sfc SkillStep object and stores it in
 * the given parameter map. At the end every parameter in the Blackboard
 * has a value from the sfc SkillStep objects variables.
 * @param step
 * @param paras
 * throws Exception
 */
void Skill::setParameters(sfc::SkillStep* step, map<String, String> &paras) {
	for (map<String, Attribute*>::iterator it = parameter.begin(); it != parameter.end(); ++it) {
		String par_name = (*it).first;
		Attribute *attr = (*it).second;
		String type = "";
		try {
			type = attr->getDataType();
		} catch (except::Exception &e) {
			CLI::printWarn(
					"Datatype check is skipped, missing type for '" + par_name + "' of '" + name + "' in '"
							+ resource->getName() + "'");
		}
		try {
			String trans = sfc_mapping->getMappedName(par_name);
			int pos = sfc_mapping->getMappedPos(par_name);
			sfc::Variable *var = step->getVariable(trans);
			String value;
			if (pos >= 0)
				value = var->getDataItem(pos);
			else
				value = var->getData();
			if (!XML::isValidDataType(type, value)) {
				stringstream ss;
				ss << "Datatype missmatch for step '" << step->getName() << "' (id=" << step->getId() << "): \""
						<< value << "\" is not of type " << type;
				throw except::Exception(ss.str(), true);
			}
			paras.insert(pair<String, String>(par_name, value));
		} catch (except::Exception &e) {
			if (!e.willOverwrite())
				throw except::Exception(e.getMessage() + " for '" + name + "'");
			else
				throw except::Exception(e.getMessage(), e.willOverwrite());
		}
	}
}

/**
 * Returns a string representation of this skill.
 * @return
 */
String Skill::toStr() {
	stringstream ss;
	for (map<String, FunctionMapping*>::iterator it = func_mapping.begin(); it != func_mapping.end(); ++it) {
		FunctionMapping *mapping = (*it).second;
		ss << endl << setw(16) << "" << "-> " << mapping->getName() + " (";
		vector<String> names = mapping->getParameterNames();
		for (size_t i = 0; i < names.size(); i++) {
			ss << mapping->getMappedName(names[i]);
			if (i < names.size() - 1)
				ss << ", ";
		}
		ss << ")";
	}
	return ss.str();
}

} /* namespace aml */
