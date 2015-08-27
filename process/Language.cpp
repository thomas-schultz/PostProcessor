/*

 * Language.cpp
 *
 *  Created on: 29.04.2015
 *      Author: Thomas Schultz
 *		Mail:	thomas.schultz@mytum.de
 */

#include "Language.h"
#include "exceptions/Exception.h"

namespace process {

/**
 * Creates a new default language object
 */
Language::Language() {
	this->comment = "#";
	this->newline = "\n";
	this->split = ',';
	this->bracketA = '(';
	this->bracketB = ')';
	this->databracketA = '{';
	this->databracketB = '}';
	this->listbracketA = '[';
	this->listbracketB = ']';
}

/**
 * Creates a new language object from given name.
 * @param lang
 * throws Exception if not supported
 */
Language::Language(String lang) {
	if (lang.getUpper() == "KRL") {
		this->comment = ";";
		this->newline = "\n";
		this->split = ',';
		this->bracketA = '(';
		this->bracketB = ')';
		this->databracketA = '{';
		this->databracketB = '}';
		this->listbracketA = '[';
		this->listbracketB = ']';
	} else {
		throw except::Exception("Unsupported language '" + lang + "'");
	}
}

/**
 * Destructor
 */
Language::~Language() {
	// TODO Auto-generated destructor stub
}

/**
 * Basic getter function.
 * @return
 */
String Language::getCommentStr() {
	return this->comment;
}

/**
 * Basic getter function.
 * @return
 */
String Language::getNewLineStr() {
	return this->newline;
}


char Language::getSplitPrameterStr() {
	return this->split;
}

/**
 * Basic getter function.
 * @return
 */
char Language::getBracketA() {
	return this->bracketA;
}

/**
 * Basic getter function.
 * @return
 */
char Language::getBracketB() {
	return this->bracketB;
}

/**
 * Basic getter function.
 * @return
 */
char Language::getDataBracketA() {
	return this->databracketA;
}

/**
 * Basic getter function.
 * @return
 */
char Language::getDataBracketB() {
	return this->databracketB;
}

/**
 * Basic getter function.
 * @return
 */
char Language::getListBracketA() {
	return this->listbracketA;
}

/**
 * Basic getter function.
 * @return
 */
char Language::getListBracketB() {
	return this->listbracketB;
}

} /* namespace process */
