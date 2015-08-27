/*
 * String.h
 *
 *  Created on: 22.04.2015
 *      Author: Thomas
 */

#ifndef SRC_TOOLS_STRING_H_
#define SRC_TOOLS_STRING_H_

#include <string>
#include <vector>

namespace std {

class String: public string {
public:
	String();
	String(string str);
	String(const String& str);
	String(const char* str);
	String(char ch);

	String operator=(string str);
	String operator=(const char* str);

	bool contains(char ch);
	bool contains(String str);
	String &replaceAll(const String &search, const String &replace);
	String &ltrim();
	String &rtrim();
	String &trim();
	String subString(char first, char last);
	vector<String> split(char delimiter);
	String &toLower();
	String &toUpper();
	String getLower();
	String getUpper();
};

} /* namespace std */

#endif /* SRC_TOOLS_STRING_H_ */
