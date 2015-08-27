/*
 * IRLFile.cpp
 *
 *  Created on: 24.01.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include <cstring>

#include "IRLFile.h"

#include "CLI.h"
#include "tools/XML.h"
#include "tools/TextFile.h"
#include "exceptions/Exception.h"

using namespace std;

namespace aml {

/**
 * Creates a new irl-file from given path.
 * @param path
 * throws Exception
 */
IRLFile::IRLFile(String path) {
	this->path = path;
	this->content = TextFile::readFromFile(path);
	char *cstr = new char[content.size() + 1];
	strcpy(cstr, content.c_str());
	try {
		this->xml.parse<0>(cstr);
		this->defs = XML::findNode(xml.first_node(), "def");
		this->srcs = XML::findNode(xml.first_node(), "src");
	} catch (rapidxml::parse_error &e) {
		throw except::InvalidFileException("Unable to parse IRL-File '"+ path + "'");
	}
}

/**
 * Destructor
 */
IRLFile::~IRLFile() {
	// TODO Auto-generated destructor stub
}

/**
 * Basic getter function.
 * @return
 */
String IRLFile::getPath() {
	return this->path;
}

/**
 * Extracts a string containing the whole function definition from the irl-file.
 * @param name
 * @return	empty string if not found
 * throws Exception
 */
String IRLFile::getFunDef(String name) {
	if (this->defs == NULL)
		throw except::InvalidFileException("IRL-File '"+ path + "' does not contain a valid definition section");
	rapidxml::xml_node<>* def = XML::findNode(defs, "definition", "name", name);
	if (def == NULL)
		return "";
	return def->value();
}

/**
 * Extracts a string containing the whole function call from the irl-file.
 * @param name
 * @return
 * throws Exception if not found
 */
String IRLFile::getFunCall(String name) {
	if (this->srcs == NULL)
		throw except::InvalidFileException("IRL-File '"+ path + "' does not contain a valid source section");
	rapidxml::xml_node<>* call = XML::findNode(srcs, "function-call", "name", name);
	if (call == NULL)
		throw except::Exception("IRL-File '" + path + "' does not contain function-call for '" + name + "'");
	return call->value();
}

/**
 * Returns a string representation of this irl-file.
 * @return
 */
String IRLFile::toStr() {
	return this->content;
}

} /* namespace aml */
