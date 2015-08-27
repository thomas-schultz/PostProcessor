/*
 * Resource.cpp
 *
 *  Created on: 24.01.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include <sstream>
#include <iomanip>

#include "Resource.h"

#include "aml/Blackboard.h"
#include "CLI.h"
#include "skill/Skill.h"
#include "tools/XML.h"
#include "exceptions/Exception.h"
#include "rapidxml-1.13/rapidxml.hpp"

namespace aml {

/**
 * Creates a new empty resoruce.
 */
Resource::Resource() {
	this->name = "";
	this->id = "";
	this->blackboard = NULL;
	this->node = NULL;
	this->errors = 0;
}

/**
 * Creates a new resource from a given xml-node.
 * @param node
 */
Resource::Resource(Blackboard *blackboard, rapidxml::xml_node<> *node) {
	this->name = XML::getAttribute(node, "Name", "");
	this->id = String(XML::getAttribute(node, "ID", "")).subString('{', '}');
	this->blackboard = blackboard;
	this->node = node;
	this->errors = 0;

	this->functions.clear();
	rapidxml::xml_node<>* functions = XML::findNode(node, "InternalElement", "Name", "FunctionDescription");
	list<rapidxml::xml_node<>*> fun_list = XML::findNodes(functions, "InternalElement");
	this->functions.clear();
	for (list<rapidxml::xml_node<>*>::iterator it = fun_list.begin(); it != fun_list.end(); ++it) {
		try {
			Function *fun = new Function(*it);
			this->functions.insert(pair<String, Function*>(fun->getName(), fun));
		} catch (except::Exception &e) {
			CLI::printWarn(e.getMessage() + " in '" + name + "'");
			continue;
		}
	}

	rapidxml::xml_node<> *skills = XML::findNode(node, "InternalElement", "Name", "Skills");
	list<rapidxml::xml_node<>*> skill_list = XML::findNodes(skills, "");
	this->skills.clear();
	for (list<rapidxml::xml_node<>*>::iterator it = skill_list.begin(); it != skill_list.end(); ++it) {
		try {
			Skill *skill = new Skill(this, *it);
			this->skills.insert(pair<String, Skill*>(skill->getName(), skill));
			vector<String> funs = skill->getFunctions();
			for (size_t i = 0; i < funs.size(); i++) {
				Function *fun = getFunction(funs[i]);
				if (fun == NULL)
					continue;
				try {
					IRLFile *irl = new IRLFile(blackboard->getPath() + fun->getIRLPath());
					this->irls.insert(pair<String, IRLFile*>(fun->getIRLPath(), irl));
				} catch (except::Exception &e) {
					CLI::printWarn(
							"Could not open IRL-file for function: '" + fun->getName() + "' in " + this->name);
				}
			}
		} catch (except::Exception &e) {
			CLI::printWarn(e.getMessage() + " in '" + name + "'");
			continue;
		}
	}
}

/**
 * Destructor
 */
Resource::~Resource() {
	for (map<String, Skill*>::iterator it = skills.begin(); it != skills.end(); ++it)
		delete ((*it).second);
	for (map<String, Function*>::iterator it = functions.begin(); it != functions.end(); ++it)
		delete ((*it).second);
	for (map<String, IRLFile*>::iterator it = irls.begin(); it != irls.end(); ++it)
		delete ((*it).second);
}

/**
 * Increase the error log counter.
 */
void Resource::error() {
	this->errors++;
}

/**
 * Returns the error log counter.
 * @return
 */
int Resource::getErrors() {
	return this->errors;
}

/**
 * Basic getter function.
 * @return
 */
String Resource::getName() {
	return this->name;
}

/**
 * Basic getter function.
 * @return
 */
String Resource::getId() {
	return this->id;
}

/**
 * Returns the skill with the given name.
 * @param name
 * @return	NULL if not found
 */
Skill* Resource::getSkill(String name) {
	map<String, Skill*>::iterator obj = skills.find(name);
	if (obj == skills.end())
		return NULL;
	else
		return obj->second;
}

/**
 * Returns the Function with the given name.
 * @param name
 * @return	NULL if not found
 */
Function* Resource::getFunction(String name) {
	map<String, Function*>::iterator obj = functions.find(name);
	if (obj == functions.end())
		return NULL;
	else
		return obj->second;
}

/**
 * Returns the irl-file with given path.
 * @param path
 * @return	NULL if not found
 */
IRLFile* Resource::getIRLFile(String path) {
	map<String, IRLFile*>::iterator obj = irls.find(path);
	if (obj == irls.end())
		return NULL;
	else
		return obj->second;
}

/**
 * Returns a string representation of this resource.
 * @return
 */
String Resource::stStr() {
	stringstream ss;
	int shift = 8, type = 8, def = 42;
	ss << setw(3 * shift) << left << name << "{" + id + "}";
	for (map<String, Skill*>::iterator it = skills.begin(); it != skills.end(); ++it) {
		ss << endl << setw(shift) << "" << setw(type) << left << "(Skill)" << setw(def) << (*it).second->getName()
				<< "{" + (*it).second->getId() + "}";
		ss << (*it).second->toStr();
	}
	for (map<String, Function*>::iterator it = functions.begin(); it != functions.end(); ++it) {
		ss << endl << setw(shift) << "" << setw(type) << left << "(Func)" << setw(def) << (*it).second->getName()
				<< "{" + (*it).second->getId() + "}";
		ss << (*it).second->toStr();
	}
	return ss.str();
}

} /* namespace aml */
