/*
 * SFCObject.cpp
 *
 *  Created on: 13.12.2014
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include "SFCObject.h"

using namespace std;

namespace sfc {

/**
 * Creates a new empty sfc-object.
 */
SFCObject::SFCObject() {
	this->obj = VOID;
	this->local_id = -1;
	this->connected = false;
}

/**
 * Destructor
 */
SFCObject::~SFCObject() {
	// TODO Auto-generated destructor stub
}

/**
 * Checks if the object is properly connected.
 * @return
 */
bool SFCObject::isConnected() {
	return this->connected;
}

/**
 * Sets the connected status.
 * @param connected
 */
void SFCObject::setConnected(bool connected) {
	this->connected = connected;
}

/**
 * Checks if the object is of type SkillStep.
 * @return
 */
bool SFCObject::isStep() {
	return (obj == STEP);
}

/**
 * Checks if the object is of type SkillStep.
 * @return
 */
bool SFCObject::isTransition() {
	return (obj == TRANS);
}

/**
 * Returns the local-id of the object.
 * @return
 */
int SFCObject::getId() {
	return this->local_id;
}

/**
 * Adds a new local-id to the list of successors.
 * @param id
 */
void SFCObject::addNext(int id) {
	this->nexts.push_back(id);
}

/**
 * Return the list of reflocal-ids.
 * @return
 */
vector<int> SFCObject::getRefIds() {
	return this->ref_ids;
}

/**
 * Return the list of local-ids of all successors.
 * @return
 */
vector<int> SFCObject::getNexts() {
	return this->nexts;
}

/**
 * Returns a string representation of this object.
 * @return
 */
String SFCObject::toStr() {
	return "ERROR: Object is of type SFCObject!";
}

} /* namespace sfc */
