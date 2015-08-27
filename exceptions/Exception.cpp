/*
 * Ecxeption.cpp
 *
 *  Created on: 22.02.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include "exceptions/Exception.h"

namespace except {

/**
 * Creates a new basic instance of an Exception.
 */
Exception::Exception() {
	this->str = "";
	this->overwrite = false;
}

/**
 * Creates a new instance of an Exception with a message.
 * @param str
 */
Exception::Exception(String str) :
		Exception() {
	this->str = str;
	this->overwrite = false;
}

/**
 * Creates a new instance of an Exception with a message and
 * a override flag.
 * @param str
 * @param overwrite
 */
Exception::Exception(String str, bool overwrite) {
	this->str = str;
	this->overwrite = overwrite;
}

/**
 * Destructor
 */
Exception::~Exception() {
	// TODO Auto-generated destructor stub
}

/**
 * Returns the message.
 * @return
 */
String Exception::getMessage() {
	return this->str;
}

/**
 * Returns the overwrite flag.
 * @return
 */
bool Exception::willOverwrite() {
	return this->overwrite;
}

} /* namespace exp */

