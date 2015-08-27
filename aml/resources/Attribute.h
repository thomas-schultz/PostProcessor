/*
 * Attribute.h
 *
 *  Created on: 07.04.2015
 *      Author: Thomas Schultz
 *		Mail:	thomas.schultz@mytum.de
 */

#ifndef SRC_AML_RESOURCES_ATTRIBUTE_H_
#define SRC_AML_RESOURCES_ATTRIBUTE_H_

#include "tools/String.h"
#include "rapidxml-1.13/rapidxml.hpp"

using namespace std;

namespace aml {

/**
 * Class that represents an aml-attribute.
 */
class Attribute {
private:
	String name;
	String description;
	String value;
	String defaultvalue;
	String unit;
	String datatype;

public:
	Attribute(String name);
	Attribute(rapidxml::xml_node<> *node);
	virtual ~Attribute();

	String getName();
	String getDescription();
	String getValue();
	String getDefaultValue();
	String getUnit();
	String getDataType();
	void setDescription(String description);
	void setValue(String value);
	void setDefaultvalue(String defaultvalue);
	void setUnit(String unit);
	void setDatatype(String datatype);
};

} /* namespace aml */

#endif /* SRC_AML_RESOURCES_ATTRIBUTE_H_ */
