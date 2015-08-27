/*
 * InvalidFileException.cpp
 *
 *  Created on: 23.02.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include "Exception.h"

namespace except {

InvalidFileException::InvalidFileException(String str) {
	this->str = str;
}

InvalidFileException::~InvalidFileException() {
	// TODO Auto-generated destructor stub
}

} /* namespace except */
