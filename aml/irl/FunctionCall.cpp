/*
 * FunctionCall.cpp
 *
 *  Created on: 25.02.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>

#include "FunctionCall.h"

#include "CLI.h"
#include "exceptions/Exception.h"

namespace aml {

/**
 * Creates a new function-call-object.
 * @param str		string of the complete function call from an irl-file
 * @param mapping	FunctionMapping object
 */
FunctionCall::FunctionCall(String str, FunctionMapping* mapping, process::Language* lang) {
	this->call = str;
	this->mapping = mapping;
	this->language = lang;
	if (str.contains(lang->getBracketA()) && str.contains(lang->getBracketB())) {
		bracketA = language->getBracketA();
		bracketB = language->getBracketB();
		brackettype = 1;
	} else if (str.contains(lang->getDataBracketA()) && str.contains(lang->getDataBracketB())) {
		bracketA = language->getDataBracketA();
		bracketB = language->getDataBracketB();
		brackettype = 2;
	} else if (str.contains(lang->getListBracketA()) && str.contains(lang->getListBracketB())) {
		bracketA = language->getListBracketA();
		bracketB = language->getListBracketB();
		brackettype = 3;
	}
	str = str.subString(bracketA, bracketB);
	vector<String> ps = str.split(lang->getSplitPrameterStr());
	for (vector<String>::iterator it = ps.begin(); it != ps.end(); it++)
		this->param.push_back((*it).trim());
}

/**
 * Destructor
 */
FunctionCall::~FunctionCall() {
	// TODO Auto-generated destructor stub
}

/**
 * Constructs a complete function call, where all occurrences of parameters
 * are replaced by their corresponding value in the given parameter map.
 * @param ps
 * @return
 * throws Exception
 */
String FunctionCall::getFuntctionCall(map<String, String> ps) {
	if (mapping == NULL)
		throw except::Exception("No function mapping found");
	vector<String> out;
	for (size_t i = 0; i < param.size(); i++) {
		try {
			String p_name = param[i].getUpper();
			String p_map = mapping->getMappedName(p_name);
			map<String, String>::iterator obj = ps.find(p_map);
			if (obj == ps.end())
				throw except::Exception("Missing parameter " + p_name + "' in Function");
			if (this->brackettype == 1)
				out.push_back((*obj).second);
			else if (brackettype == 2)
				out.push_back(p_name + " " + (*obj).second);

		} catch (except::Exception &e) {
			throw except::Exception("Function Call failed: " + e.getMessage());
		}
	}
	stringstream result;
	result << call.subString(0, bracketA) << bracketA;
	for (size_t i = 0; i < out.size(); i++) {
		result << out[i];
		if (i < out.size() - 1)
			result << language->getSplitPrameterStr() << " ";
	}
	result << bracketB;
	return result.str();
}

} /* namespace aml */
