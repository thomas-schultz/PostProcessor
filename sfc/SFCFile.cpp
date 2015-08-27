/*
 *SFCFile.h
 *
 *  Created on: 28.11.2014
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include <cstring>
#include <sstream>
#include <fstream>
#include <iterator>
#include <cstdio>

#include "SFCFile.h"

#include "SFCHelper.h"
#include "skillStep/StepSelector.h"
#include "transition/Transition.h"
#include "CLI.h"
#include "process/Flow.h"
#include "exceptions/Exception.h"
#include "tools/TextFile.h"
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"

using namespace std;

namespace sfc {

/**
 * Creates a new empty sfc-file object
 */
SFCFile::SFCFile() {
	this->company_name = "unknown";
	this->productName = "unknown";
	this->productVersion = "unknown";
	this->creationDateTime = "unknown";
	this->path = "";
	this->initial_step = -1;
	this->final_step = -1;
	this->errors = 0;
}

/**
 * Creates a new sfc-file object from given input file.
 * throws Exception
 */
SFCFile::SFCFile(String path) :
		SFCFile() {
	this->path = path;
	readFromFile();
}

/**
 * Destructor
 */
SFCFile::~SFCFile() {
	for (map<int, SFCObject*>::iterator it = sfc_objects.begin(); it != sfc_objects.end(); it++)
		delete ((*it).second);
}

/**
 * Increase the error log counter.
 */
void SFCFile::error() {
	this->errors++;
}

/**
 * Returns the error log counter.
 * @return
 */
int SFCFile::getErrors() {
	return this->errors;
}

/**
 * Basic getter function.
 * @return
 */
String SFCFile::getCompanyName() {
	return this->company_name;
}

/**
 * Basic getter function.
 * @return
 */
String SFCFile::getProductName() {
	return this->productName;
}

/**
 * Basic getter function.
 * @return
 */
String SFCFile::getProductVersion() {
	return this->productVersion;
}

/**
 * Basic getter function.
 * @return
 */
String SFCFile::getCreationDateTime() {
	return this->creationDateTime;
}

/**
 * Reads in the text file.
 * @return
 * throws FileNotFoundException
 * throws Exception
 * throws InvalidFileException
 */
void SFCFile::readFromFile() {
	String xml = TextFile::readFromFile(path);
	if (xml.size() == 0)
		throw new except::Exception("Cannot open sfc-file '" + path + "'!");

	char *cstr = new char[xml.size() + 1];
	strcpy(cstr, xml.c_str());
	try {
		this->xml.parse<0>(cstr);
	} catch (rapidxml::parse_error &e) {
		throw except::InvalidFileException("Parsing failed! Check your sfc-file.");
	}
}

/**
 * Saves the current state of the sfc file to text file.
 * @return
 */
void SFCFile::saveToFile() {
	TextFile::saveToFile(path.c_str(), this->toStr());
}

/**
 * Returns the id of the initial step of the sfc-file.
 * @return
 */
int SFCFile::getInitialStep() {
	return this->initial_step;
}

/**
 * Returns the id of the final step of the sfc-file.
 * @return
 */
int SFCFile::getFinalStep() {
	return this->final_step;
}

/**
 * Returns the complate map, which stores all sfc-object
 * inside this sfc-file.
 * @return
 */
map<int, SFCObject*>* SFCFile::getObjectMap() {
	return &this->sfc_objects;
}

/**
 * Returns the sfc-objcet with given id.
 * @param id
 * @return	NULL if not found
 */
SFCObject* SFCFile::getObject(int id) {
	map<int, SFCObject*>::iterator obj = sfc_objects.find(id);
	if (obj == sfc_objects.end())
		return NULL;
	else
		return obj->second;
}

/**
 * Parses the content of the sfc-file, generates the sfc-objects
 * from it and connects them in a double-linked list manner.
 * @param validate
 * throws InvalidFileException
 */
void SFCFile::parse(bool validate) {
	/*
	 * Retrieving the fileHeader with the information about the product
	 */
	rapidxml::xml_node<>* fileHeader = SFC::findNode(xml.first_node(), "fileHeader");
	this->company_name = SFC::getAttribute(fileHeader, "companyName", "unknown");
	this->productName = SFC::getAttribute(fileHeader, "productName", "unknown");
	this->productVersion = SFC::getAttribute(fileHeader, "productVersion", "unknown");
	this->creationDateTime = SFC::getAttribute(fileHeader, "creationDateTime", "unknown");

	/*
	 * Retrieving the first SFC part with all SkillSteps and Transitions
	 */
	rapidxml::xml_node<> *sfc = SFC::findNode(xml.first_node(), "types");
	sfc = SFC::findNode(sfc, "pous");
	sfc = SFC::findNode(sfc, "pou");
	sfc = SFC::findNode(sfc, "body");
	sfc = SFC::findNode(sfc, "SFC");
	if (sfc == NULL)
		throw except::InvalidFileException("File '" + path + "' does not seem to be a sfc-file!");

	rapidxml::xml_node<> *curNode = sfc->first_node();
	StepSelector::init();
	while (curNode != NULL) {
		registerItem(curNode, validate);
		curNode = curNode->next_sibling();
	}
	connect();
}

/**
 * Add a sfc-object to the sfc-object map.
 * @param obj
 */
void SFCFile::addSFCObject(SFCObject *obj) {
	int id = obj->getId();
	if (obj->isStep() && ((SkillStep*) obj)->isInitial()) {
		if (initial_step < 0) {
			this->initial_step = id;
		} else if (final_step < 0) {
			final_step = id;
		} else {
			stringstream out;
			out << "Additional initial Step found! (localId=" << id << ")";
			CLI::printWarn(out.str());
			error();
		}
	}
	sfc_objects.insert(std::pair<int, SFCObject*>(obj->getId(), obj));
}

/**
 * Registers a new sfc-object from a given xml-node. If the skip is
 * set to true, unknown step will be registered too.
 * @param node
 * @param skip
 */
void SFCFile::registerItem(rapidxml::xml_node<>* node, bool skip) {
	if (String(node->name()).getUpper() == "STEP") {
		String name = SFC::getAttribute(node, "name", "undef");
		SkillStep *step = StepSelector::getNewStep(name);
		if (step != NULL) {
			step->extractData(node);
			addSFCObject(step);
		} else {
			CLI::printWarn("Unknown SkillStep: " + name);
			if (skip) {
				SkillStep *step = new SkillStep();
				step->extractData(node);
				addSFCObject(step);
			} else {
				error();
			}
		}
	} else {
		String name = node->name();
		Transition *trans = new Transition(name);
		try {
			trans->extractData(node);
			addSFCObject(trans);
		} catch (except::Exception &e) {
			CLI::printWarn(e.getMessage());
			error();
			if (skip)
				addSFCObject(trans);
		}
	}
	return;
}

/**
 * Connects all sfc-objects in a double-linked list manner.
 */
void SFCFile::connect() {
	for (map<int, SFCObject*>::iterator it = sfc_objects.begin(); it != sfc_objects.end(); it++) {
		int local_id = (*it).first;
		SFCObject *obj = (*it).second;
		vector<int> predecessors = obj->getRefIds();
		for (vector<int>::iterator p = predecessors.begin(); p != predecessors.end(); p++) {
			int ref_id = *p;
			if (ref_id < 0)
				continue;
			SFCObject *pre = getObject(ref_id);
			if (pre == NULL) {
				stringstream out;
				out << "Object " << local_id << " points to missing object " << ref_id;
				CLI::printError(out.str());
				error();
				continue;
			}
			pre->addNext(local_id);
		}
	}
}

/**
 * Prints all information of this sfc-file. First all Transistions,
 * afterwards all SkillSteps are printed.
 */
void SFCFile::printSteps() {
	stringstream ss;
	ss << "\n" << company_name << ", " << productName << ", " << productVersion << ", " << creationDateTime;
	CLI::printLine(ss.str());
	list<String> s;
	for (map<int, SFCObject*>::iterator it = sfc_objects.begin(); it != sfc_objects.end(); it++) {
		if ((*it).second->isTransition())
			s.push_front((*it).second->toStr());
		if ((*it).second->isStep())
			s.push_back((*it).second->toStr());
	}
	for (list<String>::iterator it = s.begin(); it != s.end(); it++)
		CLI::print(*it);
}

/**
 * Returns a string of the sfc-file in xml style.
 * @return
 */
String SFCFile::toStr() {
	String str;
	print(back_inserter(str), xml, 0);
	return str;
}

} /* namespace sfc */
