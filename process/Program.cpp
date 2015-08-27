/*
 * Program.cpp
 *
 *  Created on: 25.02.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include "Program.h"

#include "CLI.h"
#include "tools/TextFile.h"
#include "exceptions/Exception.h"

using namespace std;

namespace process {

/**
 * Creates a new program with given path.
 * @param path
 */
Program::Program(String path,  String name) {
	this->path = String(path + name);
	this->name = name;
	this->language = new Language();
}

/**
 * Destructor
 */
Program::~Program() {
	// TODO Auto-generated destructor stub
}

String Program::getPath() {
	return this->path;
}

/**
 * Adds a info header in front of the actual code part.
 * @param header
 */
void Program::addHeaderInfo(String header) {
	this->header.push_back(header);
}

/**
 * Adds a function definition to the program. Every definition
 * will only be added only once, even if it is added more often.
 * @param name
 * @param def
 */
void Program::addDef(String name, String def) {
	if (def.size() == 0 || defs.find(name) != defs.end())
		return;
	def.replaceAll("\t", "");
	this->defs.insert(pair<String, String>(name, def));
}

/**
 * Adds a function call to the program. Function calls will
 * be added each time.
 * @param call
 */
void Program::addCall(String call) {
	call.replaceAll("\t", "");
	call.replaceAll("\n", "");
	this->calls.push_back(call);
}

void Program::addDat(String dat) {
}

String Program::getDat() {
	String datfile;
	String krlheader = String(
			"&ACCESS RVP" + language->getNewLineStr() + "&PARAM EDITMASK = *" + language->getNewLineStr());
	krlheader += "&COMMENT ";
	for (list<String>::iterator it = header.begin(); it != header.end(); ++it)
		krlheader += (*it) + " ";
	datfile += krlheader + language->getNewLineStr();
	datfile += "DEFDAT " + name + language->getNewLineStr();
	for (list<String>::iterator it = dat.begin(); it != dat.end(); ++it)
		datfile += "\t" + (*it) + language->getNewLineStr();
	datfile += "ENDDAT " + language->getNewLineStr();
	return datfile;
}

void Program::setLanguage(Language* lang) {
	if (lang != NULL)
		this->language = lang;
}

/**
 * Saves the program to file.
 */
void Program::saveToFile() {
	String prog;
	String krlheader = String("&ACCESS RVP" + language->getNewLineStr());
	krlheader += "&PARAM EDITMASK = *" + language->getNewLineStr();
	krlheader += "&COMMENT ";
	for (list<String>::iterator it = header.begin(); it != header.end(); ++it)
		krlheader += (*it) + " ";
	String ini = String(
			"\tGLOBAL INTERRUPT DECL 3 WHEN $STOPMESS==TRUE DO IR_STOPM ( )" + language->getNewLineStr()
					+ "\tINTERRUPT ON 3" + language->getNewLineStr() + "\tBAS (#INITMOV,0)"
					+ language->getNewLineStr());
	String move = String(
			"\t$BWDSTART = FALSE" + language->getNewLineStr() + "\tPDAT_ACT=PDEFAULT" + language->getNewLineStr()
					+ "\tFDAT_ACT=FHOME" + language->getNewLineStr() + "\tBAS (#PTP_PARAMS,100 )" + language->getNewLineStr() + "\tPTP $H_POS" + language->getNewLineStr());
	prog += krlheader + language->getNewLineStr();

	// Main Function
	prog += "DEF " + name + "( )" + language->getNewLineStr();
	// Init declaration
	prog += language->getNewLineStr() + ini + language->getNewLineStr();
	// Move HomePos
	prog += move + language->getNewLineStr();

	//TODO
	// Workaround for Base and Tooloffset
	prog += "\t$BASE = {X 0, Y 0, Z 0, A 0, B 0, C 0}" + language->getNewLineStr();
	prog += "\t$TOOL = {X 0, Y 0, Z 0, A 0, B 0, C 0}" + language->getNewLineStr();
	prog += language->getNewLineStr();

	// Instructions
	for (list<String>::iterator it = calls.begin(); it != calls.end(); ++it)
		prog += "\t" + (*it) + language->getNewLineStr();
	prog += "END" + language->getNewLineStr();
	// Function definitions
	for (map<String, String>::iterator it = defs.begin(); it != defs.end(); ++it)
			prog += (*it).second;

	try {
		TextFile::saveToFile(this->path + ".src", prog);
		TextFile::saveToFile(this->path + ".dat", getDat());
	} catch (except::Exception &e) {
		CLI::printLine("Failed to save the program output!");
		return;
	}
	CLI::printLine("Program was written to '" + path + ".src'");
}

} /* namespace process */
