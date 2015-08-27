/*
 * Blackboard.h
 *
 *  Created on: 17.01.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef SRC_AML_BLACKBOARD_H_
#define SRC_AML_BLACKBOARD_H_

#include <list>
#include <map>

#include "resources/Resource.h"
#include "process/Language.h"
#include "tools/String.h"
#include "rapidxml-1.13/rapidxml.hpp"

using namespace std;

namespace aml {

/**
 *  Class that represents a blackboard-file.
 */
class Blackboard {
private:
	String path;
	process::Language* language;
	int errors;
	rapidxml::xml_document<> doc;
	map<String, Resource*> resources;

public:
	Blackboard();
	Blackboard(String path);
	virtual ~Blackboard();

	String getPath();
	void readFromFile();
	void saveToFile();
	void parse(bool validate);
	String getSFCPath();
	process::Language* getLanguage();
	Resource* getResource(String name);
	int getErrors();

	void setCodeURI(String path);
	void printResources();
};

} /* namespace aml */

#endif /* SRC_AML_BLACKBOARD_H_ */
