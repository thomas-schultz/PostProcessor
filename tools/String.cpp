/*
 * String.cpp
 *
 *  Created on: 22.04.2015
 *      Author: Thomas
 */

#include <iostream>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

#include "String.h"

namespace std {

String::String() :
		string() {
}

String::String(string str) :
		string(str) {
}

String::String(const String& str) :
		string(str) {
}

String::String(const char* str) :
		string(str) {
}

String::String(char ch) :
		string(1, ch) {
}

String String::operator=(string str) {
	return String(str);
}

String String::operator=(const char* str) {
	return this->assign(str);
}

/**
 * Checks if the string contains this character.
 * @param str
 * @return
 */
bool std::String::contains(char ch) {
	return (this->find(ch) != this->npos);
}

/**
 * Checks if the string contains the substring.
 * @param str
 * @return
 */
bool std::String::contains(String str) {
	return (this->find(str) != this->npos);
}

/**
 * Replace all occurrence of search with replace.
 * @param search	search string
 * @param replace	replacement string
 * @return
 */
String& String::replaceAll(const String& search, const String& replace) {
	for (size_t pos = 0;; pos += replace.length()) {
		// Locate the substring to replace
		pos = this->find(search, pos);
		if (pos == string::npos)
			break;
		// Replace by erasing and inserting
		this->erase(pos, search.length());
		this->insert(pos, replace);
	}
	return *this;
}

/**
 * Trims string from start.
 * @return
 */
String& String::ltrim() {
	this->erase(this->begin(), find_if(this->begin(), this->end(), not1(ptr_fun<int, int>(isspace))));
	return *this;
}

/**
 * Trims string from end.
 * @return
 */
String& String::rtrim() {
	this->erase(find_if(this->rbegin(), this->rend(), not1(ptr_fun<int, int>(isspace))).base(), this->end());
	return *this;
}

/**
 * Trims string from both sides.
 * @return
 */
String& String::trim() {
	this->ltrim();
	this->rtrim();
	return *this;
}

/**
 * Returns the string between the two characters.
 * @param first	first character of substring
 * @param last	last charter of substring
 * @return	unmodified string if not possible
 */
String String::subString(char first, char last) {
	int a = -1;
	int z = this->size();
	if (first != 0)
		a = this->find(first);
	if (last != 0)
		z = this->find(last);
	if (a >= z)
		return *this;
	return this->substr(a + 1, z - a - 1);
}

/**
 * Splits the string at every delimiter character into a vector of strings.
 * @param delimiter	split character
 * @return	vector of strings
 */
vector<String> String::split(char delimiter) {
	String str = this->trim();
	vector<String> split;
	while (true) {
		size_t splitpoint = str.find(delimiter);
		if (splitpoint == str.npos) {
			if (str.size() > 0)
				split.push_back(str);
			break;
		}
		String item = str.substr(0, splitpoint);
		split.push_back(item.trim());
		str = String(str.substr(splitpoint + 1, str.npos));
	}
	return split;
}

/**
 * Converts all characters inside string to lower case.
 * @return
 */
String& String::toLower() {
	transform(this->begin(), this->end(), this->begin(), ::tolower);
	return *this;
}

/**
 * Converts all characters inside string to upper case.
 * @return
 */
String& String::toUpper() {
	transform(this->begin(), this->end(), this->begin(), ::toupper);
	return *this;
}

/**
 * Returns a new string with all characters are in lower case.
 * @return
 */
String String::getLower() {
	string str = *this;
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

/**
 * Returns a new string with all characters are in upper case.
 * @return
 */
String String::getUpper() {
	string str = *this;
	transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}

} /* namespace std */
