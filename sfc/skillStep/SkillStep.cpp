/*
 * SkillStep.cpp
 *
 *  Created on: 28.11.2014
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include "SkillStep.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <iterator>

#include "CLI.h"
#include "sfc/SFCFile.h"
#include "rapidxml-1.13/rapidxml.hpp"

namespace sfc {

/**
 * Creates a new SkillStep.
 */
SkillStep::SkillStep() {
	this->name = "undef";
	this->obj = STEP;
	this->local_id = -1;
	this->initial = false;
	this->final = false;
	this->duration = 0.0;
	this->resource = "";
	this->skill = "";
	this->ref_ids.push_back(-1);
	this->nexts.push_back(-1);
}

/**
 * Creates a new SkillStep with given name and given list of required
 * variable to read from the xml-node.
 * @param name
 * @param vars
 */
SkillStep::SkillStep(String name, vector<String> vars) :
		SkillStep() {
	this->name = name;
	this->needed_vars = vars;
}

/**
 * Destructor
 */
SkillStep::~SkillStep() {
	for (map<String, Variable*>::iterator it = vars.begin(); it != vars.end(); ++it)
		delete ((*it).second);
}

/**
 * Basic getter function.
 * @return
 */
String SkillStep::getName() {
	return this->name;
}

/**
 * Returns the first reflocal-id in the list.
 * @return	-1 if the list is empty
 */
int SkillStep::getRefId() {
	if (this->ref_ids.size() != 1)
		return -1;
	else
		return this->ref_ids.front();
}

/**
 * Returns the first local-id in the successor list.
 * @return	-1 if the list is empty
 */
int SkillStep::getNext() {
	if (this->nexts.size() != 1)
		return -1;
	else
		return this->nexts.front();
}

/**
 * Basic getter function.
 * @return
 */
String SkillStep::getResource() {
	return this->resource;
}

/**
 * Basic getter function.
 * @return
 */
String SkillStep::getSkill() {
	return this->skill;
}

/**
 * Basic getter function.
 * @return
 */
double SkillStep::getDuration() {
	return this->duration;
}

/**
 * Basic getter function.
 * @return
 */
bool SkillStep::isInitial() {
	return this->initial;
}

/**
 * Basic getter function.
 * @return
 */
bool SkillStep::isFinal() {
	return this->final;
}

/**
 * Adds a new local-id to the list of successors.
 * @param id
 */
void SkillStep::addNext(int id) {
	if (nexts.size() == 1 && nexts.front() < 0) {
		nexts.clear();
	} else if (nexts.size() != 0) {
		CLI::printError("Multiple successors for SkillStep, id=", local_id);
	}
	this->nexts.push_back(id);
}

/**
 * Extracts all important data from the given xml-node.
 * @param node
 */
void SkillStep::extractData(rapidxml::xml_node<>* node) {
	this->name = SFC::getAttribute(node, "name", "");
	this->local_id = SFC::getLocalId(node);
	this->ref_ids = SFC::getRefIds(node);
	if (name.getUpper() == "START") {
		this->initial = true;
		if (SFC::getAttribute(node, "initialStep", "false") != "true")
			CLI::printWarn("Start-Step without initialStep flag, id=", local_id);
	}
	if (name.getUpper() == "END") {
		this->final = true;
		if (SFC::getAttribute(node, "initialStep", "false") != "true")
			CLI::printWarn("End-Step without initialStep flag, id=", local_id);
	}
	this->duration = SFC::getDuration(node);
	this->resource = SFC::getResource(node);
	this->skill = SFC::getSkillName(node);
	if (this->ref_ids.size() != 1 && !this->isInitial())
		CLI::printError("SkillStep with multiple predeccesors, id=", local_id);

	list<rapidxml::xml_node<>*> vars = SFC::findNodes(node, "Variable");
	for (list<rapidxml::xml_node<>*>::iterator it = vars.begin(); it != vars.end(); ++it) {
		String name = SFC::getAttribute((*it), "name", "undef");
		String value = SFC::getAttribute((*it), "Value", "");
		if (!this->isRegisteredVariable(name))
			continue;
		Variable *var = new Variable(name, value);
		addVariable(var);
	}
	for (size_t i = 0; i < needed_vars.size(); i++) {
		if (getVariable(needed_vars[i]) == NULL)
			CLI::printWarn("Missing variable '" + needed_vars[i] + "' in SkillStep, id=", local_id);
	}
}

/**
 * Registers a new required variable by name.
 * @param name
 */
void SkillStep::registerVariable(String name) {
	this->needed_vars.push_back(name);
}

/**
 * Checks if the given variable name is in the list of required variable names.
 * @param name
 * @return
 */
bool SkillStep::isRegisteredVariable(String name) {
	for (size_t i = 0; i < needed_vars.size(); i++) {
		if (name.getUpper() == needed_vars[i].getUpper())
			return true;
	}
	return false;
}

/**
 * Adds a variable to the list of available variables.
 * @param var
 */
void SkillStep::addVariable(Variable *var) {
	this->vars.insert(pair<String, Variable*>(var->getName(), var));
}

/**
 * Returns the variable with the given name from the list of available variables.
 * @param name
 * @return	NULL if not found
 */
Variable* SkillStep::getVariable(String name) {
	map<String, Variable*>::iterator obj = vars.find(name.getUpper());
	if (obj == vars.end())
		return NULL;
	else
		return obj->second;
}

/**
 * Returns a string representation of this SkillStep.
 * @return
 */
String SkillStep::toStr() {
	stringstream ss;
	if (initial)
		ss << setw(sfc::PRINT_WIDTH) << left << "Step: *";
	else
		ss << setw(sfc::PRINT_WIDTH) << left << "Step: ";
	ss << getName() << endl;
	ss << setw(sfc::PRINT_WIDTH) << "";
	if (getRefId() < 0)
		ss << "(?)";
	else
		ss << getRefId();
	ss << " -> [ " << local_id << " ] -> ";
	vector<int> next = SFCObject::getNexts();
	for (size_t i = 0; i < next.size(); i++) {
		if (next[i] >= 0)
			ss << next[i];
		else
			ss << "(?)";
		if (i < next.size() - 1)
			ss << ", ";
	}
	ss << endl;
	ss << setw(sfc::PRINT_WIDTH) << "" << setw(sfc::PRINT_WIDTH) << "Duration:" << duration << " seconds" << endl;
	if (resource.size() > 0)
		ss << setw(sfc::PRINT_WIDTH) << "" << setw(sfc::PRINT_WIDTH) << "Resource:" << resource << endl;
	if (skill.size() > 0)
		ss << setw(sfc::PRINT_WIDTH) << "" << setw(sfc::PRINT_WIDTH) << "Skill:" << skill << endl;
	if (vars.size() > 0) {
		for (map<String, Variable*>::iterator it = vars.begin(); it != vars.end(); ++it)
			ss << setw(sfc::PRINT_WIDTH) << "" << (*it).second->toStr() << endl;
	}
	return ss.str();
}

} /* namespace SFC */
