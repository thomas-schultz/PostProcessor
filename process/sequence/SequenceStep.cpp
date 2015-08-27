/*
 * SequenceStep.cpp
 *
 *  Created on: 05.12.2014
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include <iomanip>
#include <sstream>

#include "SequenceStep.h"

namespace process {

SequenceStep::SequenceStep(sfc::SkillStep *step, double time) {
	this->type = STEP;
	this->step = step;
	this->timestamp = time;
}

SequenceStep::~SequenceStep() {
	// TODO Auto-generated destructor stub
}

int SequenceStep::getStepId() {
	return this->step->getId();
}

SEQUENCE SequenceStep::getType() {
	return this->type;
}

String SequenceStep::toStr(int indention) {
	stringstream s;
	for (int i = 0; i < indention; i++)
		s << process::INDENTION;
	s << "id=" << setw(4) << step->getId() << ", [@" << setw(6) << left << timestamp << " + " << setw(6) << right
			<< step->getDuration() << "], " << step->getName();
	return s.str();
}

} /* namespace process */
