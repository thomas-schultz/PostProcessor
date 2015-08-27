/*
 * FileNotFoundException.cpp
 *
 *  Created on: 23.02.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include "Exception.h"

namespace except {

FileNotFoundException::FileNotFoundException(String str) {
	this->str = str;
}

FileNotFoundException::~FileNotFoundException() {
	// TODO Auto-generated destructor stub
}

} /* namespace except */
