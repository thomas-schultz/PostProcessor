/*
 * TextFile.h
 *
 *  Created on: 22.02.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 *
 *	Header file for handling text files like the sfc and
 *	blackboard files
 */

#ifndef SRC_TOOLS_TEXTFILE_H_
#define SRC_TOOLS_TEXTFILE_H_

#include <iostream>
#include <fstream>
#include <cstdlib>

#include "String.h"
#include "OS.h"
#include "exceptions/Exception.h"

using namespace std;

static unsigned char utf8[] = { 239, 187, 191 };
static vector<String> umlauts_hex =
		{ "\xc3\x96", "\xc3\xb6", "\xc3\x84", "\xc3\xa4", "\xc3\x9c", "\xc3\xbc", "\xc3\x9f" };
static vector<String> umlauts_char = { "Ä", "ö", "A", "ä", "Ü", "ü", "ß" };

class TextFile {

public:

	/**
	 * Test if a text file has a Byte Order Mark at the beginning.
	 * @param path
	 * @return
	 */
	static bool isUTF8withBOM(String path) {
		FILE *fin;
		char character;
		int pos = 0;
		fin = fopen(path.c_str(), "r");
		if (fin == NULL)
			return false;
		while ((character = fgetc(fin)) != EOF && pos < 3) {
			if ((unsigned char) character != utf8[pos])
				return false;
			pos++;
		}
		fclose(fin);
		return true;
	}

	/**
	 * Reads the text file from given path and returns the content in a string.
	 * The path may contain slash or backslash but without spaces.
	 * @param path
	 * @return
	 * throws FileNotFoundException
	 */
	static String readFromFile(String path) {
		String content = "";
		ifstream file;
		if (OS::isLinux()) {
			path = path.subString('\\', '/');
			path = path.subString(' ', '\\ ');
		}
		file.open(path.c_str());
		if (file.is_open()) {
			String line;
			while (getline(file, line)) {
				content += line + "\n";
			}
			file.close();
		} else {
			throw except::FileNotFoundException("Cannot open file '" + path + "'!");
		}

		if (OS::isWindows()) {
			for (size_t i = 0; i < umlauts_hex.size(); i++) {
				String search = umlauts_hex[i];
				String replace = umlauts_char[i];
				content.replaceAll(search, replace);
			}
		}
		return content;
	}

	/**
	 * Saves the string content to the file at path in UTF8 with BOM.
	 * @param path
	 * @param content
	 * throws FileNotFoundException
	 */
	static void saveToUTF8File(String path, String content) {
		ofstream myfile(path.c_str());
		if (myfile.is_open()) {
			myfile << char(utf8[0]) << char(utf8[1]) << char(utf8[2]);
			myfile << content;
			myfile.close();
		} else {
			throw except::FileNotFoundException("Cannot write to file '" + path + "'!");
		}
		return;
	}

	/**
		 * Saves the string content to the file at path in UTF8 without BOM.
		 * @param path
		 * @param content
		 * throws FileNotFoundException
		 */
		static void saveToFile(String path, String content) {
			ofstream myfile(path.c_str());
			if (myfile.is_open()) {
				myfile << content;
				myfile.close();
			} else {
				throw except::FileNotFoundException("Cannot write to file '" + path + "'!");
			}
			return;
		}

};

#endif /* SRC_TOOLS_TEXTFILE_H_ */
