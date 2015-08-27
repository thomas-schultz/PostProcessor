/*
 * OS.h
 *
 *  Created on: 22.02.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef SRC_OS_H_
#define SRC_OS_H_

using namespace std;

/**
 * Class file for determining the Operating System.
 */
class OS {

public:

	static bool isWindows() {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__CYGWIN__)
		return true;
#else
		return false;
#endif
	}

	static bool isLinux() {
		return !isWindows();
	}
};

#endif /* SRC_OS_H_ */
