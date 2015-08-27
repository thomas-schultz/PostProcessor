/*
 * CLI.h
 *
 *  Created on: 18.01.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef SRC_CLI_H_
#define SRC_CLI_H_

#include <iostream>
#include <sstream>
#include <vector>

#include "tools/String.h"

using namespace std;

/**
 * Class that handles the command line output.
 */
class CLI {

private:
	static bool fail;
	static bool verbose;

public:

	/**
	 * Sets the CLI to verbose mode, if true all warnings will be printed.
	 * @param verbose
	 */
	static void setVerbose(bool verbose_) {
		verbose = verbose_;
	}

	/**
	 * Prints the usage to command line.
	 */
	static void printUsage() {
		stringstream s;
		s << "Usage: PostProcessor [options] <blackboard-file> <program-name>" << endl;
		std::cout << s.str();
	}

	/**
	 * Prints the version and build number on the command line
	 * @param version: version string to print
	 * @param version: build number to print
	 */
	static void printVersion(String version, String build) {
		stringstream s;
		s << "PostProcessor v" << version  << ", Build: " << build << endl;
		std::cout << s.str();
	}

	/**
	 * Prints the complete help output, showing all options.
	 */
	static void printHelp() {
		stringstream s;
		s << "PostProcessor by Thomas Schultz (thomas.schultz@tum.de)" << endl;
		s << "Options:" << endl;
		s << "  --help \t prints this help menu" << endl;
		s << "  --warn \t prints all warnings" << endl;
		s << "  --info \t prints info about this version" << endl;
		s << "  --validate \t performs validation operations on the given sfc-file, includes --warn" << endl;
		s << "  --sfc \t use sfc-file instead of blackboard-file, includes --validate" << endl;
		s << "  -pB \t\t prints details from the blackboard file" << endl;
		s << "  -pS \t\t prints detailed list of skillsteps and transitions" << endl;
		s << "  -pF \t\t prints the sequence flow of the sfc-file" << endl;
		s << "  -pO \t\t prints total oder of execution" << endl;
		cout << s.str();
	}

	/**
	 * Prints string to command line without newline.
	 * @param str
	 */
	static void print(String str) {
		cout << str;
	}

	/**
	 *  Prints string to command line with newline.
	 * @param str
	 */
	static void printLine(String str) {
		cout << str << endl;
	}

	/**
	 *  Prints integer to command line without newline.
	 * @param i
	 */
	static void printLine(int i) {
		cout << i << endl;
	}

	/**
	 * Prints a Debug message.
	 * @param str
	 */
	static void debug(String str) {
#ifdef DEBUG
		cout << "DEBUG: " << str << endl;
#endif
	}

	/**
	 * Prints an error message to command line without newline.
	 * @param str
	 */
	static void printError(String str) {
		CLI::fail ^= true;
		cout << "ERROR:   " << str << endl;
	}

	/**
	 * Prints an error message containing one string str
	 * and one integer i to command line without newline.
	 * @param str
	 * @param i
	 */
	static void printError(String str, int i) {
		CLI::fail = true;
		cout << "ERROR:   " << str << i << endl;
	}

	/**
	 * Prints a warning message to command line without newline.
	 * @param str
	 */
	static void printWarn(String str) {
		if (verbose)
			cout << "WARNING: " << str << endl;
	}

	/**
	 * Prints a warning message containing one string str
	 * and one integer i to command line without newline.
	 * @param str
	 * @param i
	 */
	static void printWarn(String str, int i) {
		if (verbose)
			cout << "WARNING: " << str << i << endl;
	}

	/**
	 * Prints a vector of integer as "[ ... ]".
	 * @param a
	 */
	static void printVector(vector<int> a) {
		cout << "[ ";
		for (size_t i = 0; i < a.size(); i++) {
			cout << a[i];
			if (i != a.size() - 1)
				cout << " , ";
			else
				cout << " ]" << endl;
		}
	}

	/**
	 * Returns true if any error was reported during runtime.
	 * @return
	 */
	static bool hasErrors() {
		return CLI::fail;
	}

};

#endif /* SRC_CLI_H_ */
