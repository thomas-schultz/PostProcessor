/*
 * Parameter.cpp
 *
 *  Created on: 24.02.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include <iostream>
#include <cstdlib>

#include "Parameter.h"

#include "tools/XML.h"
#include "exceptions/Exception.h"

using namespace std;

namespace aml {

/**
 * Creates a new parameter with name and corresponding mapped name.
 * @param name
 * @param trans
 */
Parameter::Parameter(String name, String trans) {
	this->name = name;
	this->trans = trans;
	this->pos = -1;
}

/**
 * Creates a new parameter with name and corresponding mapped name
 * and a position index, if it is a nested parameter.
 * @param name
 * @param trans
 * @param pos
 */
Parameter::Parameter(String name, String trans, int pos) :
		Parameter(name, trans) {
	this->pos = pos;
}

/**
 * Destructor
 */
Parameter::~Parameter() {
	// TODO Auto-generated destructor stub
}

/**
 * Basic getter function.
 * @return
 */
String Parameter::getName() {
	return this->name;
}

/**
 * Basic getter function.
 * @return
 */
String Parameter::getTranslation() {
	return this->trans;
}

/**
 * Basic getter function.
 * @return
 */
int aml::Parameter::getPos() {
	return this->pos;
}

} /* namespace aml */

