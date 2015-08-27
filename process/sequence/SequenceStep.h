/*
 * SequenceStep.h
 *
 *  Created on: 05.12.2014
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef PROCESS_SEQUENCE_SEQUENCESTEP_H_
#define PROCESS_SEQUENCE_SEQUENCESTEP_H_

#include "Sequence.h"

#include "sfc/skillStep/SkillStep.h"
#include "tools/String.h"

namespace process {

/**
 * Class for one sequence step.
 */
class SequenceStep: public Sequence {
protected:
	sfc::SkillStep *step;

public:
	SequenceStep(sfc::SkillStep *step, double time);
	virtual ~SequenceStep();

	int getStepId();
	virtual SEQUENCE getType();
	virtual String toStr(int indention);
};

} /* namespace process */

#endif /* PROCESS_SEQUENCE_SEQUENCESTEP_H_ */
