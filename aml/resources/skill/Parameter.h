/*
 * Parameter.h
 *
 *  Created on: 24.02.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef SRC_AML_RESOURCES_PARAMETER_H_
#define SRC_AML_RESOURCES_PARAMETER_H_

#include "tools/String.h"

using namespace std;

namespace aml {

/**
 * Class that represents a Blackboard Skill parameter.
 */
class Parameter {
private:
	String name;
	String trans;
	int pos;

public:
	Parameter(String name, String trans);
	Parameter(String name, String trans, int pos);
	virtual ~Parameter();

	String getName();
	String getTranslation();
	int getPos();
};

} /* namespace aml */

#endif /* SRC_AML_RESOURCES_PARAMETER_H_ */
