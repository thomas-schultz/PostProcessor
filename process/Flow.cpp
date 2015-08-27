/*
 * Flow.cpp
 *
 *  Created on: 28.11.2014
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include <iostream>
#include <iomanip>
#include <ios>
#include <list>
#include <map>

#include "Flow.h"
#include "aml/Blackboard.h"
#include "aml/irl/IRLFile.h"
#include "aml/irl/FunctionCall.h"
#include "sfc/skillStep/SkillStep.h"
#include "sfc/transition/Transition.h"
#include "exceptions/Exception.h"

using namespace std;

namespace process {

int Flow::number;

/**
 * Creates a new empty Flow.
 */
Flow::Flow() {
	Flow::number = 0;
	this->sfc = NULL;
	this->blackboard = NULL;
	this->sequence = NULL;
	this->errors = 0;
}

/**
 * Creates a new Flow from a given sfc-input and the Blackboard.
 * @param sfc_file
 * @param board
 */
Flow::Flow(sfc::SFCFile *sfc_file, aml::Blackboard *board) :
		Flow() {
	this->sfc = sfc_file;
	this->blackboard = board;
}

/**
 * Destructor
 */
Flow::~Flow() {
	for (map<int, sfc::SkillStep*>::iterator it = steps.begin(); it != steps.end(); it++)
		delete ((*it).second);
}

/**
 * Increases the error log count.
 */
void Flow::error() {
	errors++;
}

/**
 * Returns the error log count.
 * @return
 */
int Flow::getErrors() {
	return errors;
}

/**
 * Returns sfc-object with the given id.
 * @param id
 * @return	NULL if not found
 */
sfc::SFCObject* Flow::getObject(int id) {
	return sfc->getObject(id);
}

/**
 * Adds a step to the current flow. Each step is assigned a successive number
 * to store them in total order.
 * @param id
 */
void Flow::addStep(int id) {
	this->steps.insert(pair<int, sfc::SkillStep*>(++Flow::number, (sfc::SkillStep*) this->getObject(id)));
}

/**
 * Creates a sequence of steps to display the sfc-structure
 * in a more user-friendly way.
 */
void Flow::createSequence() {
	this->sequence = new Sequence(this, 0.0);
	if (sfc->getInitialStep() < 0) {
		CLI::printError("No initial step found!");
		error();
	}
	sequence->connect(sfc->getInitialStep());
}

/**
 * Performs a check on the structure, if every step is
 * properly linked with others.
 */
void Flow::check() {
	if (this->sequence == NULL)
		createSequence();
	for (map<int, sfc::SFCObject*>::iterator it = sfc->getObjectMap()->begin(); it != sfc->getObjectMap()->end();
			it++) {
		int id = (*it).first;
		sfc::SFCObject *obj = (*it).second;
		if (!obj->isConnected()) {
			error();
			stringstream out;
			out << "Found unlinked sfc object with id " << id;
			CLI::printWarn(out.str());
		}
	}
}

/**
 * Processes every step to generate code. For each step, the corresponding Skill
 * inside the according resource is picked. Then, the list of Functions for this
 * skill is processed in the order they are specified in the FunctionMapping. The
 * skill-parameters are mapped to the function-parameter and a new function call
 * is generated. If the function needs a further definition part, it will be added
 * to the output file only once.
 * @param prog
 */
void Flow::process(Program *prog) {
	if (blackboard == NULL) {
		CLI::printLine("No Blackboard found, aborting!");
		return;
	}
	prog->setLanguage(blackboard->getLanguage());
	if (this->sequence == NULL)
		createSequence();
	prog->addHeaderInfo("Company name: " + sfc->getCompanyName());
	prog->addHeaderInfo("Product name: " + sfc->getProductName());
	prog->addHeaderInfo("Product version: " + sfc->getProductVersion());
	prog->addHeaderInfo("Creation time: " + sfc->getCreationDateTime());

	for (std::map<int, sfc::SkillStep*>::iterator it = steps.begin(); it != steps.end(); ++it) {
		sfc::SkillStep *step = (*it).second;
		if (step->isInitial())
			continue;
		if (step->isFinal())
			break;
		aml::Resource *resource = new aml::Resource();
		aml::Skill *skill;
		try {
			resource = this->blackboard->getResource(step->getResource());
			String skill_name = step->getSkill();
			if (skill_name.size() == 0) {
				stringstream ss;
				ss << "Missing skillname for Step (id=" << step->getId() << ") '" << step->getName() << "'";
				throw except::Exception(ss.str());
			}
			skill = resource->getSkill(skill_name);
			if (skill == NULL) {
				stringstream ss;
				ss << "No such Skill '" << skill_name << "' for Step (id=" << step->getId() << ") '" << step->getName()
						<< "'";
				throw except::Exception(ss.str());
			}
			if (!skill->isMapped())
				throw except::Exception("Trying to process unmapped Skill '" + skill->getName() + "'");
			map<String, String> parameter;
			skill->setParameters(step, parameter);
			vector<String> funs = skill->getFunctions();
			for (size_t i = 0; i < funs.size(); i++) {
				String fun_name = funs[i];
				aml::Function *fun = resource->getFunction(fun_name);
				aml::IRLFile *irl = resource->getIRLFile(fun->getIRLPath());
				if (irl == NULL)
					throw except::Exception(
							"Processing failed: '" + fun->getName() + "' in '" + skill->getName()
									+ "': Invalid IRL-File");
				String fun_def;
				String fun_call;
				try {
					fun_def = irl->getFunDef(fun->getFunName());
					fun_call = irl->getFunCall(fun->getFunName());
				} catch (except::Exception &e) {
					throw except::Exception(
							"Assembling of code-line for function '" + fun_name + "' for Step '" + skill->getName()
									+ "' failed: " + e.getMessage());
				}
				// insert the parameter into function call;
				aml::FunctionCall *fun_call_param = new aml::FunctionCall(fun_call,
						skill->getFunctionMapping(fun->getName()), blackboard->getLanguage());
				try {
					prog->addDef(fun_name, fun_def);
					prog->addCall(fun_call_param->getFuntctionCall(parameter));
					delete (fun_call_param);
				} catch (except::Exception &e) {
					throw except::Exception(e.getMessage() + " for '" + fun_name + "'");
				}
			}
		} catch (except::Exception &e) {
			if (resource != NULL && !e.willOverwrite())
				CLI::printError(e.getMessage() + " in '" + resource->getName() + "'");
			else
				CLI::printError(e.getMessage());
			throw except::Exception("Processing aborted!");
		}
	}
}

/**
 * Prints the sfc-structure in a user-friendly way.
 */
void Flow::printFlow() {
	if (this->sequence == NULL)
		createSequence();
	CLI::print("\n" + this->sequence->toStr(0));
}

/**
 * Prints the sequential order of steps, in which they
 * are going to be processed.
 */
void Flow::printOrder() {
	CLI::printLine("");
	for (std::map<int, sfc::SkillStep*>::iterator it = steps.begin(); it != steps.end(); ++it) {
		stringstream out;
		out << " #" << it->first;
		out << " \t" << "(id=" << it->second->getId() << ") \t" << it->second->getName();
		CLI::printLine(out.str());
	}
}

} /* namespace process */
