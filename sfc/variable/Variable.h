/*
 * Variable.h
 *
 *  Created on: 29.11.2014
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef SFC_VARIABLE_H_
#define SFC_VARIABLE_H_

#include <vector>

#include "tools/String.h"

using namespace std;

namespace sfc {

const int VAR_WIDTH = 12;

/**
 * Class for variables in the sfc-file.
 */
class Variable {
protected:
	String name;
	vector<String> values;

public:
	Variable(String name, String values);
	virtual ~Variable();

	String getName();
	bool isEmpty();
	bool isList();
	bool isValue();

	vector<String> getDataList();
	String getData();
	String getDataItem(int index);

	virtual String toStr();
};

} /* namespace sfc */

#endif /* SFC_VARIABLE_H_ */
