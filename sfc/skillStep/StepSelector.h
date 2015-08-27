/*
 * StepSelector.h
 *
 *  Created on: 28.11.2014
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include <map>
#include <vector>

#include "SkillStep.h"

#include "tools/String.h"

using namespace std;

namespace sfc {

/**
 * Class that handles the different SkillStep types.
 */
class StepSelector {

public:

	static map<String, vector<String>> steps;
	static vector<String> step_names;

	static void init();
	static void registerStep(String name, vector<String> vars);
	static SkillStep* getNewStep(String name);
	static String getSupportedSteps();
};

} /* namespace sfc */
