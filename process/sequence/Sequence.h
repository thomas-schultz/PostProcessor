/*
 * Sequence.h
 *
 *  Created on: 05.12.2014
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef PROCESS_SEQUENCE_SEQUENCE_H_
#define PROCESS_SEQUENCE_SEQUENCE_H_

#include <list>

#include "tools/String.h"

using namespace std;

namespace process {

enum SEQUENCE {
	SEQ, PAR, STEP, UNDEF
};

const String INDENTION = " | ";

class Flow;

/**
 * Class for generating a step sequence.
 */
class Sequence {
protected:
	Sequence();
	SEQUENCE type;
	double timestamp;
	Flow *flow;
	list<Sequence*> sequentiel;

	void appendSequence(Sequence *seq);
	void appendStep(int id);

public:
	Sequence(Flow *flow, double time);
	virtual ~Sequence();

	double getTimestamp();
	int connect(int id);
	virtual SEQUENCE getType();
	virtual String toStr(int indention);
};

} /* namespace process */

#endif /* PROCESS_SEQUENCE_SEQUENCE_H_ */
