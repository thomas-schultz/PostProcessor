/*
 * Transition.cpp
 *
 *  Created on: 28.11.2014
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include <iostream>
#include <iomanip>
#include <sstream>

#include "Transition.h"

#include "exceptions/Exception.h"


namespace sfc {

static vector<String> transitions = { "Transition", "simultaneousDivergence", "simultaneousConvergence" };

/**
 * Creates a new transition with given name;
 * @param name
 */
Transition::Transition(String name) {
	this->obj = TRANS;
	this->name = name;
}

/**
 * Destructor
 */
Transition::~Transition() {
	// TODO Auto-generated destructor stub
}

/**
 * Extract all important information from the given xml-node.
 * @param node
 * throws Exception
 */
void Transition::extractData(rapidxml::xml_node<>* node) {
	this->local_id = SFC::getLocalId(node);
	this->ref_ids = SFC::getRefIds(node);
	if (!isSimple() && !isDivergence() && !isConvergence()) {
		stringstream ss;
		ss << "Invalid Transition, id=" << this->local_id;
		throw except::Exception(ss.str());
	}
}

/**
 * Checks if the transition is simple 1-to-1 transition.
 * @return
 */
bool Transition::isSimple() {
	if (name.getUpper() == transitions[0].getUpper())
		return true;
	return false;
}

/**
 * Checks if the transition is 1-to-N divergence.
 * @return
 */
bool Transition::isDivergence() {
	if (name.getUpper() == transitions[1].getUpper())
		return true;
	return false;
}

/**
 * Checks if the transition is N-to-1 convergence.
 * @return
 */
bool Transition::isConvergence() {
	if (name.getUpper() == transitions[2].getUpper())
		return true;
	return false;
}

/**
 * Returns a String representation of this Transition.
 * @return
 */
String Transition::toStr() {
	stringstream ss;
	ss << setw(sfc::PRINT_WIDTH) << left << "Transition";
	ss << "(" << name << ")" << endl;
	ss << setw(sfc::PRINT_WIDTH) << "";
	vector<int> pre = SFCObject::getRefIds();
	for (size_t i = 0; i < pre.size(); i++) {
		if (pre[i] >= 0)
			ss << pre[i];
		else
			ss << "(?)";
		if (i < pre.size() - 1)
			ss << ", ";
	}
	if (pre.size() == 0)
		ss << "(?)";
	ss << " -> [ " << local_id << " ] -> ";
	vector<int> next = SFCObject::getNexts();
	for (size_t i = 0; i < next.size(); i++) {
		if (next[i] >= 0)
			ss << next[i];
		else
			ss << "(?)";
		if (i < next.size() - 1)
			ss << ", ";
	}
	if (next.size() == 0)
		ss << "(?)";
	ss << endl;
	return ss.str();
}

} /* namespace sfc */
