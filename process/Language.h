/*
 * Language.h
 *
 *  Created on: 29.04.2015
 *      Author: Thomas Schultz
 *		Mail:	thomas.schultz@mytum.de
 */

#ifndef SRC_PROCESS_LANGUAGE_H_
#define SRC_PROCESS_LANGUAGE_H_

#include "tools/String.h"

using namespace std;

namespace process {

/*
 *	Class for storing language related stuff
 */
class Language {
private:
	String newline;
	String comment;
	char split;
	char bracketA;
	char bracketB;
	char databracketA;
	char databracketB;
	char listbracketA;
	char listbracketB;

public:
	Language();
	Language(String lang);
	virtual ~Language();

	String getCommentStr();
	String getNewLineStr();
	char getSplitPrameterStr();

	char getBracketA();
	char getBracketB();
	char getDataBracketA();
	char getDataBracketB();
	char getListBracketA();
	char getListBracketB();
};

} /* namespace process */

#endif /* SRC_PROCESS_LANGUAGE_H_ */
