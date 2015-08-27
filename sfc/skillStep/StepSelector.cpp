/*
 * StepSelector.cpp
 *
 *  Created on: 15.04.2015
 *      Author: Thomas Schultz
 *		Mail:	thomas.schultz@mytum.de
 */

#include <sstream>

#include "StepSelector.h"

#include "CLI.h"

using namespace std;

namespace sfc {

map<String, vector<String>> StepSelector::steps;
vector<String> StepSelector::step_names;

/**
 * Initialize the StepSelector with the list of supported SkillSTeps.
 */
void StepSelector::init() {
	registerStep("Start", vector<String> { });
	registerStep("End", vector<String> { });
	registerStep("Wait", vector<String> { });
	registerStep("P2P", vector<String> { "Frame", "Velocity", "JointConfig" });
	registerStep("LIN", vector<String> { "Frame", "Velocity", "JointConfig" });
	registerStep("CIRC", vector<String> { "Frame", "Velocity", "JointConfig" });
	registerStep("Weld_On", vector<String> { "Turn_ON" });
	registerStep("Gas", vector<String> { "Volumenstrom" });
	registerStep("Power", vector<String> { "Spannung", "Strom" });
	registerStep("ElektrodeVorschub", vector<String> { "Foerdergeschwindigkeit" });
}

/**
 * Registers a new SkillStep type by name and list of required variable names.
 * @param name
 * @param vars
 */
void StepSelector::registerStep(String name, vector<String> vars) {
	steps.insert(pair<String, vector<String>>(name, vars));
	step_names.push_back(name);
}

/**
 * Returns an SkillStep object according to the given name.
 * @param name
 * @return	NULL if not supported
 */
SkillStep* StepSelector::getNewStep(String name) {
	for (map<String, vector<String>>::iterator it = steps.begin(); it != steps.end(); ++it) {
		String type = (*it).first;
		if (name.getUpper() == type.getUpper())
			return new SkillStep(type, (*it).second);
	}
	return NULL;
}

/**
 * Returns a list of supported SkillStep types. Will only work
 * if the StepSelector is already initialized.
 * @return
 */
String StepSelector::getSupportedSteps() {
	stringstream ss;
	for (size_t i = 0; i < step_names.size(); i++) {
		ss << step_names[i];
		if (i < step_names.size() - 1)
			ss << ", ";
	}
	return ss.str();
}

} /* namespace sfc */
