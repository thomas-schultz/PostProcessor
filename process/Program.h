/*
 * Program.h
 *
 *  Created on: 25.02.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef SRC_PROCESS_PROGRAM_H_
#define SRC_PROCESS_PROGRAM_H_

#include <map>
#include <list>

#include "Language.h"
#include "tools/String.h"

using namespace std;

namespace process {

/**
 * Class that represents the generated code.
 */
class Program {
private:
	String path;
	String name;
	map<String, String> defs;
	list<String> header;
	list<String> calls;
	list<String> dat;
	Language* language;

public:
	Program(String path, String name);
	virtual ~Program();

	String getPath();
	void setLanguage(Language* lang);
	void addHeaderInfo(String header);
	void addDef(String name, String def);
	void addCall(String call);
	void addDat(String dat);
	String getDat();
	void saveToFile();
};

} /* namespace process */

#endif /* SRC_PROCESS_PROGRAM_H_ */
