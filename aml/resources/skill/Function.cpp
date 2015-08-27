/*
 * Function.cpp
 *
 *  Created on: 24.01.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include <sstream>
#include <iomanip>

#include "Function.h"

#include "CLI.h"
#include "aml/resources/Attribute.h"
#include "tools/XML.h"
#include "exceptions/Exception.h"

namespace aml {

/**
 * Creates a new Function from a given xml-node.
 * @param node
 */
Function::Function(rapidxml::xml_node<> *node) {
	this->name = XML::getAttribute(node, "Name", "");
	this->id = String(XML::getAttribute(node, "ID", "")).subString('{', '}');
	Attribute desc = Attribute(XML::findNode(node, "Attribute", "Name", "Description"));
	try {
		this->description = desc.getValue();
	} catch (except::Exception &e) {
		this->description = "";
	}
	rapidxml::xml_node<> *attribs = XML::findNode(node, "ExternalInterface", "Name", "FunctionDataInterface");
	Attribute refuri = Attribute(XML::findNode(attribs, "Attribute", "Name", "refURI"));
	try {
		this->irl_path = refuri.getValue().replaceAll("\\", "/");
	} catch (except::Exception &e) {
		CLI::printWarn("Missing IRL-file in mapping for '" + name + "'");
	}
	Attribute name = Attribute(XML::findNode(attribs, "Attribute", "Name", "Name"));
	try {
		this->fun_name = name.getValue();
	} catch (except::Exception &e) {
		CLI::printWarn("Missing value for function name for '" + this->name + "'");
	}
}

/**
 * Destructor
 */
Function::~Function() {
	// TODO Auto-generated destructor stub
}

/**
 * Basic getter function.
 * @return
 */
String Function::getName() {
	return this->name;
}

/**
 * Basic getter function.
 * @return
 */
String Function::getId() {
	return this->id;
}

/**
 * Basic getter function.
 * @return
 */
String Function::getDescription() {
	return this->description;
}

/**
 * Basic getter function.
 * @return
 */
String Function::getIRLPath() {
	return this->irl_path;
}

/**
 * Basic getter function.
 * @return
 */
String Function::getFunName() {
	return this->fun_name;
}

/**
 * Returns a string representation of this Function.
 * @return
 */
String Function::toStr() {
	stringstream ss;
	ss << endl << setw(16) << "" << "-> IRL-File:  \t" << irl_path;
	return ss.str();
}

} /* namespace aml */
