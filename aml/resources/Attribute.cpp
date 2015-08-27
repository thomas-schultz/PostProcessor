/*
 * Attribute.cpp
 *
 *  Created on: 07.04.2015
 *      Author: Thomas Schultz
 *		Mail:	thomas.schultz@mytum.de
 */

#include "Attribute.h"

#include "exceptions/Exception.h"
#include "tools/XML.h"

using namespace std;

namespace aml {

/**
 * Creates a new empty attribute.
 * @param name
 */
Attribute::Attribute(String name) {
	this->name = name;
}

/**
 * Creates a new attribute from given xml-node.
 * @param node
 */
Attribute::Attribute(rapidxml::xml_node<> *node) {
	if (node == NULL)
		return;
	this->name = XML::getAttribute(node, "Name", "");
	this->unit = XML::getAttribute(node, "Unit", "");
	this->datatype = XML::getAttribute(node, "AttributeDataType", "");
	rapidxml::xml_node<> *n = XML::findNode(node, "Description");
	this->description = XML::getValue(n, "");
	n = XML::findNode(node, "Value");
	this->value = XML::getValue(n, "");
	n = XML::findNode(node, "DefaultValue");
	this->defaultvalue = XML::getValue(n, "");
}

/**
 * Destructor
 */
Attribute::~Attribute() {
	// TODO Auto-generated destructor stub
}

/**
 * Basic getter function.
 * @return
 * throws Exception
 */
String Attribute::getName() {
	if (this->name.size() == 0)
		throw except::Exception("Attribute 'Name' not found");
	else
		return this->name;
}

/**
 * Basic getter function.
 * @return
 * throws Exception
 */
String Attribute::getDescription() {
	if (this->description.size() == 0)
		throw except::Exception("Attribute 'Description' not found");
	else
		return this->description;
}

/**
 * Basic getter function.
 * @return
 * throws Exception
 */
String Attribute::getValue() {
	if (this->value.size() == 0)
		throw except::Exception("Attribute 'Value' not found");
	else
		return this->value;
}

/**
 * Basic getter function.
 * @return
 * throws Exception
 */
String Attribute::getDefaultValue() {
	if (this->defaultvalue.size() == 0)
		throw except::Exception("Attribute 'DefaultValue' not found");
	else
		return this->defaultvalue;
}

/**
 * Basic getter function.
 * @return
 * throws Exception
 */
String Attribute::getUnit() {
	if (this->unit.size() == 0)
		throw except::Exception("Attribute 'Unit' not found");
	else
		return this->unit;
}

/**
 * Basic getter function.
 * @return
 * throws Exception
 */
String Attribute::getDataType() {
	if (this->datatype.size() == 0)
		throw except::Exception("Attribute 'DataType' not found");
	else
		return this->datatype;
}

/**
 * Basic setter function.
 * @return
 */
void Attribute::setDescription(String description) {
	this->description = description;
}

/**
 * Basic setter function.
 * @return
 */
void Attribute::setValue(String value) {
	this->value = value;
}

void Attribute::setDefaultvalue(String defaultvalue) {
	this->defaultvalue = defaultvalue;
}

/**
 * Basic setter function.
 * @return
 */
void Attribute::setUnit(String unit) {
	this->unit = unit;
}

/**
 * Basic setter function.
 * @return
 */
void Attribute::setDatatype(String datatype) {
	this->datatype = datatype;
}

} /* namespace aml */
