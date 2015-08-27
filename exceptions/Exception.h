/*
 * Ecxeption.h
 *
 *  Created on: 22.02.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef SRC_EXCEPTIONS_EXCEPTION_H_
#define SRC_EXCEPTIONS_EXCEPTION_H_

#include <exception>

#include "tools/String.h"

using namespace std;

namespace except {

/**
 * Super-Class for all Exceptions.
 */
class Exception: public exception {
protected:
	String str;
	bool overwrite;
public:
	Exception();
	Exception(String str);
	Exception(String str, bool overwrite);
	virtual ~Exception();

	String getMessage();
	bool willOverwrite();
};

class FileNotFoundException: public Exception {
public:
	FileNotFoundException(String str);
	virtual ~FileNotFoundException();
};

class InvalidFileException: public Exception {
public:
	InvalidFileException(String str);
	virtual ~InvalidFileException();
};

} /* namespace exp */

#endif /* SRC_EXCEPTIONS_EXCEPTION_H_ */
