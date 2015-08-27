/*
 * Variable.cpp
 *
 *  Created on: 29.11.2014
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include <iomanip>
#include <sstream>
#include <cmath>

#include "Variable.h"

#include "exceptions/Exception.h"

namespace sfc {

static char in_bracket = '(';
static char out_bracket = ')';
static char param_split = ',';

/**
 * Creates a new variable with name and values.
 * @param name
 * @param values
 */
Variable::Variable(String name, String values) {
	this->name = name.toUpper();
	this->values = values.subString(in_bracket, out_bracket).split(param_split);
}

/**
 * Destructor
 */
Variable::~Variable() {
	// TODO Auto-generated destructor stub
}

/**
 * Basic getter function.
 * @return
 */
String Variable::getName() {
	return this->name;
}

/**
 * Checks if the list of values is empty.
 * @return
 */
bool Variable::isEmpty() {
	return (values.size() == 0);
}

/**
 * Checks if the variable is of type list.
 * @return
 */
bool Variable::isList() {
	return (values.size() > 1);
}

/**
 * Checks if the variable is of type single-value
 * @return
 */
bool Variable::isValue() {
	return (values.size() == 1);
}

/**
 * Returns the raw list of stored values.
 * @return
 */
vector<String> Variable::getDataList() {
	return this->values;
}

/**
 * Returns the content of the variable, if it is of type single-value
 * @return
 */
String Variable::getData() {
	if (isList())
		throw except::Exception("Variable is a list, not single value");
	if (isEmpty())
		throw except::Exception("Variable is empty");
	return this->values.front();
}

/**
 * Returns the content of the corresponding list position of the variable,
 * if it is of type list.
 * @param index
 * @return
 */
String Variable::getDataItem(int index) {
	if (isValue())
		throw except::Exception("Variable is a single value, not a list");
	if (isEmpty())
		throw except::Exception("Variable is empty");
	if (abs(index) >= (int) values.size())
		throw except::Exception("Index out of bound");
	return this->values[index];
}

/**
 * Returns a string representation of this variable.
 * @return
 */
String Variable::toStr() {
	stringstream ss;
	ss << setw(sfc::VAR_WIDTH + 8) << left << name + ":";
	if (isValue())
		ss << setw(sfc::VAR_WIDTH - 2) << "(Value) " << " = " << getData();
	if (isList()) {
		ss << setw(sfc::VAR_WIDTH - 2) << "(List)" << " = [ ";
		for (size_t i = 0; i < values.size(); i++) {
			ss << values[i];
			if (i < values.size() - 1)
				ss << ", ";
		}
		ss << " ]";
	}
	return ss.str();
}

} /* namespace sfc */
