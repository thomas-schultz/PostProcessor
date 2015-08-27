/*
 * ParallelSequence.h
 *
 *  Created on: 05.12.2014
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef PROCESS_SEQUENCE_PARALLELSEQUENCE_H_
#define PROCESS_SEQUENCE_PARALLELSEQUENCE_H_

#include <vector>

#include "Sequence.h"

#include "tools/String.h"

namespace process {

/**
 * Class for a parallel sequence.
 */
class ParallelSequence: public Sequence {
protected:
	Flow *flow;
	vector<Sequence*> parallel;

public:
	ParallelSequence(Flow *flow, double time);
	virtual ~ParallelSequence();

	int connect(vector<int> ids);
	virtual SEQUENCE getType();
	virtual String toStr(int indention);
};

} /* namespace process */

#endif /* PROCESS_SEQUENCE_PARALLELSEQUENCE_H_ */
