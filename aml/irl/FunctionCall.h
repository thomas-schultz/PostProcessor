/*
 * FunctionCall.h
 *
 *  Created on: 25.02.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef SRC_AML_RESOURCES_IRL_FUNCTIONCALL_H_
#define SRC_AML_RESOURCES_IRL_FUNCTIONCALL_H_

#include <string>
#include <map>

#include "process/Language.h"
#include "tools/String.h"
#include "aml/resources/skill/FunctionMapping.h"

using namespace std;

namespace aml {

/**
 * Class that maps the parameter to generate complete function calls.
 */
class FunctionCall {
private:
	String call;
	FunctionMapping *mapping;
	vector<String> param;
	process::Language* language;
	int brackettype;
	char bracketA;
	char bracketB;

public:
	FunctionCall(String str, FunctionMapping* mapping, process::Language* lang);
	virtual ~FunctionCall();

	String getFuntctionCall(map<String, String> ps);
};

} /* namespace aml */

#endif /* SRC_AML_RESOURCES_IRL_FUNCTIONCALL_H_ */
