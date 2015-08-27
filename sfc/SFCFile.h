/*
 * SFCFile.h
 *
 *  Created on: 28.11.2014
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef SFC_SFCFILE_H_
#define SFC_SFCFILE_H_

#include <list>
#include <map>

#include "SFCHelper.h"
#include "SFCObject.h"
#include "tools/String.h"
#include "rapidxml-1.13/rapidxml.hpp"

namespace process {
class Flow;
}

using namespace std;

namespace sfc {

/**
 * Class that represents a sfc-file.
 */
class SFCFile {
private:
	String path;
	rapidxml::xml_document<> xml;
	String company_name;
	String productName;
	String productVersion;
	String creationDateTime;
	map<int, sfc::SFCObject*> sfc_objects;
	int initial_step;
	int final_step;
	int errors;
	void addSFCObject(sfc::SFCObject *obj);
	void registerItem(rapidxml::xml_node<> *node, bool validate);

public:
	SFCFile();
	SFCFile(String path);
	virtual ~SFCFile();

	String getCompanyName();
	String getProductName();
	String getProductVersion();
	String getCreationDateTime();
	void readFromFile();
	void saveToFile();
	int getInitialStep();
	int getFinalStep();
	void error();
	int getErrors();

	map<int, sfc::SFCObject*>* getObjectMap();
	SFCObject* getObject(int id);

	void parse(bool validate);
	void connect();
	void printSteps();
	String toStr();
};

} /* namespace SFC */

#endif /* SFC_SFCFILE_H_ */
