/*
 * Flow.h
 *
 *  Created on: 28.11.2014
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef PROCESS_FLOW_H_
#define PROCESS_FLOW_H_

#include <map>

#include "tools/String.h"
#include "CLI.h"
#include "Program.h"
#include "aml/Blackboard.h"
#include "sfc/SFCFile.h"
#include "sequence/Sequence.h"

namespace sfc {
class SFCObject;
class SkillStep;
}

using namespace std;

namespace process {

/**
 * Main class for processing the skillsteps from the given sfc-file
 * and to generate a sequential total order of steps.
 */
class Flow {
private:
	static int number;
	int errors;
	Sequence *sequence;
	sfc::SFCFile *sfc;
	aml::Blackboard *blackboard;
	map<int, sfc::SkillStep*> steps;

	void createSequence();

public:
	Flow();
	Flow(sfc::SFCFile *sfc, aml::Blackboard *board);
	virtual ~Flow();

	void error();
	int getErrors();

	int getInitialStep();
	sfc::SFCObject* getObject(int id);
	void addStep(int id);
	Sequence* getSequence();

	void check();
	void process(Program *prog);
	void printSteps();
	void printFlow();
	void printOrder();
};

} /* namespace sfc */

#endif /* PROCESS_FLOW_H_ */
