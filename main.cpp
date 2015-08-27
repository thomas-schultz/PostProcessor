/*
 * main.cpp
 *
 *  Created on: 28.11.2014
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 *
 *	main program which does the command line parsing and
 *	starts the code generation
 */

#include <iostream>
#include <fstream>
#include <sstream>

#include "OS.h"
#include "CLI.h"
#include "aml/Blackboard.h"
#include "sfc/SFCFile.h"
#include "sfc/skillStep/StepSelector.h"
#include "process/Flow.h"
#include "tools/String.h"
#include "exceptions/Exception.h"


using namespace std;
using namespace sfc;
using namespace aml;
using namespace process;
using namespace except;

bool CLI::fail;
bool CLI::verbose;

static String version = "1.0.3";

std::String build_date() {
    return __DATE__;
}

static bool fileexists(String file) {
	ifstream infile(file.c_str());
	return infile.good();
}

int main(int argc, char* argv[]) {

	// Initialize program options
	String in_file = "";
	String out_file = "";
	bool sfc = 0;
	bool validate = 0;
	bool printAML = 0;
	bool printSFC = 0;
	bool printFlw = 0;
	bool printOrd = 0;


#ifdef DEBUG
	/*
	 * testing purpose only
	 */
	cout << ">> DEBUG-Mode active <<" << endl;
	CLI::printVersion(version, build_date());
	String filename, name, output;
	filename = "C:/Users/Thomas/workspace/PostProcessor/Blackboard.aml";
	//filename = "Planungsdaten/LIN_reduziert_final_2_Aufspgen.xml";
	if (!OS::isWindows()) {
		filename = "/home/thomas/workspace/PostProcessor/" + filename;
		name = "PostProcessor";
	} else {
		filename = "c:/Users/Thomas/workspace/PostProcessor/" + filename;
		name = "PostProcessor.exe";
	}
	output = "output";
	char *arg[2];
	arg[0] = &name[0];
	arg[1] = &filename[0];
	arg[2] = &output[0];
	argv = arg;
	argc = 3;

	sfc = 0;
	CLI::setVerbose(1);
	validate = 0;
	printAML = 0;
	printSFC = 0;
	printFlw = 0;
	printOrd = 0;
#endif

	// parse command line
	int n = 0;
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--help") == 0) {
			CLI::printHelp();
			return 0;
		} else if (strcmp(argv[i], "--warn") == 0) {
			CLI::setVerbose(true);
			continue;
		} else if (strcmp(argv[i], "--info") == 0) {
			sfc::StepSelector::init();
			CLI::printVersion(version, build_date());
			CLI::printLine("  Supported SkillSteps:\n  " + StepSelector::getSupportedSteps());
			return 0;
		} else if (strcmp(argv[i], "--validate") == 0) {
			CLI::setVerbose(true);
			validate = true;
			continue;
		} else if (strcmp(argv[i], "--sfc") == 0) {
			sfc = true;
			validate = true;
			CLI::setVerbose(true);
			continue;
		} else if (strcmp(argv[i], "-pB") == 0) {
			printAML = true;
			continue;
		} else if (strcmp(argv[i], "-pS") == 0) {
			printSFC = true;
			continue;
		} else if (strcmp(argv[i], "-pF") == 0) {
			printFlw = true;
			continue;
		} else if (strcmp(argv[i], "-pO") == 0) {
			printOrd = true;
			continue;
		} else {
			if (n == 0)
				in_file = argv[i];
			else if (n == 1)
				out_file = argv[i];
			else
				CLI::printUsage();
			n++;
		}
	}

	if (argc < 3) {
		CLI::printUsage();
		return (0);
	}

	if (in_file.size() == 0) {
		CLI::printLine("no file specified!");
	}
	stringstream out;
	Blackboard* blackboard = new Blackboard();
	SFCFile *sfc_file = new SFCFile();
	Flow *flow = new Flow();

	// parse the blackboard file if specified, otherwise skip
	if (!sfc) {
		if (!fileexists(in_file)) {
			CLI::printLine("blackboard-file '" + in_file + "' not found!");
			return -1;
		}
		try {
			blackboard = new Blackboard(in_file);
			blackboard->parse(validate);
			in_file = blackboard->getSFCPath();
		} catch (Exception &e) {
			CLI::printLine(e.getMessage());
			return -1;
		} catch (...) {
			CLI::printLine("AN Error occurred while parsing the blackboard");
			return -1;
		}
		if (validate && blackboard->getErrors() > 0)
			out << "Blackboard parsing failed! Total of " << blackboard->getErrors() << " errors detected." << endl;
	}
	if (printAML)
		blackboard->printResources();
	if (!fileexists(in_file)) {
		CLI::printLine("sfc-file '" + in_file + "' not found!");
		return -1;
	}

	// parse the sfc file
	try {
		sfc_file = new SFCFile(in_file);
		sfc_file->parse(validate);
	} catch (Exception &e) {
		CLI::printLine(e.getMessage());
		return 1;
	} catch (...) {
		CLI::printLine("An unknown error occurred while parsing the sfc-file");
		return -1;
	}
	if (validate && sfc_file->getErrors() > 0) {
		out << "SFC parsing failed! Total of " << sfc_file->getErrors() << " errors detected." << endl;
		if (sfc) {
			if (printSFC)
				sfc_file->printSteps();
			if (printFlw) {
				flow = new Flow(sfc_file, blackboard);
				flow->printFlow();
			}
			CLI::printLine(out.str());
			return -1;
		}
	}
	if (printSFC)
		sfc_file->printSteps();

	if (!validate && (blackboard->getErrors() > 0 || sfc_file->getErrors() > 0 || CLI::hasErrors())) {
		out << "Parsing failed" << endl;
		CLI::printLine(out.str());
		return -1;
	}
	CLI::printLine("Parsing completed");

	// creating the program flow with the given sfc and blackboard file
	flow = new Flow(sfc_file, blackboard);
	if (validate) {
		flow->check();
		if (flow->getErrors() > 0 || CLI::hasErrors())
			out << "Object linking failed! Total of " << flow->getErrors() << " errors detected." << endl;
	}

	// printing information, mainly for testing purpose
	if (printFlw)
		flow->printFlow();
	if (printOrd)
		flow->printOrder();

	if (validate) {
		if (blackboard->getErrors() + sfc_file->getErrors() + flow->getErrors() > 0 || CLI::hasErrors())
			out << "Input validation failed!" << endl;
		else
			out << "Input validation successful!" << endl;
		CLI::printLine("\n" + out.str());
		return 0;
	}

	// generate code from given input
	Program *prog = new Program(blackboard->getPath(), out_file);
	try {
		flow->process(prog);
		prog->saveToFile();
		blackboard->setCodeURI(out_file);
		blackboard->saveToFile();
	} catch (Exception &e) {
		CLI::printLine(e.getMessage());
		return -1;
	}
	return 0;
}
