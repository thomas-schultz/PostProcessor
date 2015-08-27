/*
 * Sequence.cpp
 *
 *  Created on: 05.12.2014
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include "Sequence.h"
#include "SequenceStep.h"
#include "ParallelSequence.h"
#include "process/Flow.h"
#include "sfc/skillStep/SkillStep.h"
#include "sfc/transition/Transition.h"

using namespace std;

namespace process {

Sequence::Sequence() {
	this->type = UNDEF;
	this->timestamp = 0.0;
	this->flow = NULL;
}

process::Sequence::Sequence(Flow* flow, double time) {
	this->type = SEQ;
	this->timestamp = time;
	this->flow = flow;
}

Sequence::~Sequence() {
	for (list<Sequence*>::iterator it = sequentiel.begin(); it != sequentiel.end(); it++)
		delete (*it);
}

SEQUENCE Sequence::getType() {
	return this->type;
}

double Sequence::getTimestamp() {
	return this->timestamp;
}

void Sequence::appendSequence(Sequence* seq) {
	this->sequentiel.push_back(seq);
}

void Sequence::appendStep(int id) {
	SequenceStep *step = new SequenceStep((sfc::SkillStep*) this->flow->getObject(id), timestamp);
	this->sequentiel.push_back(step);
	flow->addStep(id);
}

int Sequence::connect(int id) {
	while (id > 0) {
		sfc::SFCObject *obj = flow->getObject(id);
		if (obj == NULL) {
			stringstream out;
			out << "Cannot connect sequence, no object with id " << id;
			CLI::printError(out.str());
			flow->error();
			break;
		}
		if (obj->isStep()) {
			appendStep(id);
			sfc::SkillStep *step = (sfc::SkillStep*) obj;
			id = step->getNext();
			timestamp += step->getDuration();
			step->setConnected(true);
			continue;
		}
		if (!obj->isTransition()) {
			stringstream out;
			out << "Cannot connect sequence, unknown Object " << id;
			CLI::printError(out.str());
			flow->error();
			break;
		}
		sfc::Transition *trans = (sfc::Transition*) obj;
		vector<int> ids = trans->getNexts();
		if (trans->getRefIds().size() == 1 && trans->getNexts().size() == 1) {
			// 1-1 Transition
			if (obj->isConnected())
				return 0;
			if (trans->getNexts().size() == 0) {
				stringstream out;
				out << "Missing successor for Transition " << id;
				CLI::printError(out.str());
				flow->error();
				return -1;
			}
			id = trans->getNexts().front();
			obj->setConnected(true);
		} else if (trans->getRefIds().size() == 1 && trans->getNexts().size() > 1) {
			// 1-M Transition
			if (obj->isConnected())
				return 0;
			ParallelSequence *parallel = new ParallelSequence(flow, timestamp);
			id = parallel->connect(ids);
			appendSequence(parallel);
			this->timestamp = parallel->timestamp;
			obj->setConnected(true);
		} else if (trans->getRefIds().size() > 1 && trans->getNexts().size() == 1) {
			// N-1 Transition
			if (trans->getNexts().size() == 0) {
				if (!obj->isConnected()) {
					stringstream out;
					out << "Missing successor for Convergence " << id;
					CLI::printError(out.str());
					flow->error();
				}
				obj->setConnected(true);
				return -1;
			} else {
				obj->setConnected(true);
				return trans->getNexts().front();
			}
		} else {
			// N-M Transition
			stringstream out;
			out << "Unexpected Transition " << id;
			CLI::printError(out.str());
			flow->error();
			return -1;
		}
	}
	return 0;
}

String Sequence::toStr(int indention) {
	stringstream s;
	for (list<Sequence*>::iterator it = sequentiel.begin(); it != sequentiel.end(); it++) {
		Sequence *seq = (*it);
		s << seq->toStr(indention) << endl;
	}
	return s.str();
}

} /* namespace process */
