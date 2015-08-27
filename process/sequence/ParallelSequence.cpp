/*
 * ParallelSequence.cpp
 *
 *  Created on: 05.12.2014
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include <iostream>
#include <sstream>

#include "ParallelSequence.h"

namespace process {

ParallelSequence::ParallelSequence(Flow *flow, double time) {
	this->type = PAR;
	this->timestamp = time;
	this->flow = flow;

}

ParallelSequence::~ParallelSequence() {
	for (vector<Sequence*>::iterator it = parallel.begin(); it != parallel.end(); it++)
		delete (*it);
}

SEQUENCE ParallelSequence::getType() {
	return this->type;
}

int ParallelSequence::connect(vector<int> ids) {
	int next = -1;
	int max_timestamp = 0.0;
	for (size_t i = 0; i < ids.size(); i++) {
		Sequence *seq = new Sequence(flow, timestamp);
		next = seq->connect(ids[i]);
		if (seq->getTimestamp() > max_timestamp)
			max_timestamp = seq->getTimestamp();
		parallel.push_back(seq);
	}
	this->timestamp = max_timestamp;
	return next;
}

String process::ParallelSequence::toStr(int indention) {
	stringstream s;
	for (int i = 0; i < indention; i++)
		s << process::INDENTION;
	s << "{" << endl;
	for (vector<Sequence*>::iterator it = parallel.begin(); it != parallel.end(); it++) {
		s << (*it)->toStr(indention + 1);
		if (it != parallel.end() - 1) {
			for (int i = 0; i < indention; i++)
				s << process::INDENTION;
			s << " +" << endl;
		}
	}
	for (int i = 0; i < indention; i++)
		s << process::INDENTION;
	s << "}";
	return s.str();
}

} /* namespace process */

