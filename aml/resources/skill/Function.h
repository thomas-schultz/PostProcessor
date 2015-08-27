/*
 * Function.h
 *
 *  Created on: 24.01.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef SRC_AML_RESOURCES_FUNCTION_H_
#define SRC_AML_RESOURCES_FUNCTION_H_

#include "tools/String.h"
#include "rapidxml-1.13/rapidxml.hpp"

using namespace std;

namespace aml {

/**
 * Class that represents a Function in the Blackboard.
 */
class Function {
private:
	String name;
	String id;
	String description;
	String irl_path;
	String fun_name;

public:
	Function(rapidxml::xml_node<> *node);
	virtual ~Function();

	String getName();
	String getId();
	String getDescription();
	String getIRLPath();
	String getFunName();
	String toStr();
};

} /* namespace aml */

#endif /* SRC_AML_RESOURCES_FUNCTION_H_ */
