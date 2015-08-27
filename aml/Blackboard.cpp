/*
 * Blackboard.cpp
 *
 *  Created on: 17.01.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#include <fstream>
#include <list>

#include "Blackboard.h"

#include "CLI.h"
#include "tools/TextFile.h"
#include "tools/XML.h"
#include "exceptions/Exception.h"
#include "rapidxml-1.13/rapidxml.hpp"

using namespace std;

namespace aml {

/**
 * Creates an empty Blackboad object.
 */
Blackboard::Blackboard() {
	this->path = "";
	this->errors = 0;
	this->language = new process::Language();
}

/**
 * Creates a new Blackboard from give file.
 * @param path
 */
Blackboard::Blackboard(String path) {
	this->path = path;
	this->errors = 0;
	readFromFile();
}

/**
 * Destructor
 */
Blackboard::~Blackboard() {
	for (map<String, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
		delete ((*it).second);
}

String Blackboard::getPath() {
	vector<String> folders;
	if (this->path.contains("/"))
		folders = this->path.split('/');
	else if (this->path.contains("\\"))
		folders = this->path.split('\\');
	else {
		folders.push_back(this->path);
		return "";
	}
	string path;
	for (size_t i = 0; i < folders.size()-1; i++)
		path += folders[i] + "/";
	return path;
}


/**
 * Reads in all important information from the file.
 * throws InvalidFileException
 */
void Blackboard::readFromFile() {
	String xml = TextFile::readFromFile(path);
	if (xml.size() == 0)
		throw except::Exception("Cannot open blackboard '" + path + "'!");
	if (xml.find("InstanceHierarchy Name=\"Blackboard\"") == String::npos)
		throw except::InvalidFileException("File '" + path + "' does not seem to be a blackboard-file!");

	char *cstr = new char[xml.size() + 1];
	strcpy(cstr, xml.c_str());
	try {
		this->doc.parse<rapidxml::parse_no_data_nodes>(cstr);
	} catch (rapidxml::parse_error &e) {
		throw except::InvalidFileException("Parsing failed! Check your blackboard-file.");
	}
	return;
}

/**
 * Saves the current state of the Blackboard to file.
 */
void Blackboard::saveToFile() {
	String xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
	print(back_inserter(xml), doc, 0);
	XML::saveToFile(this->path, xml);
	return;
}

/**
 * Parses the Blackboard-file and creates the resource objects.
 * @param validate
 */
void Blackboard::parse(bool validate) {
	rapidxml::xml_node<> *node = XML::findNode(doc.first_node(), "InstanceHierarchy", "Name", "Blackboard");
	node = XML::findNode(node, "InternalElement", "Name", "Ressourcen");
	list<rapidxml::xml_node<>*> res = XML::findNodes(node, "InternalElement");
	for (list<rapidxml::xml_node<>*>::iterator it = res.begin(); it != res.end(); ++it) {
		rapidxml::xml_node<>* curNode = (*it);
		Resource *resource = new Resource(this, curNode);
		this->resources.insert(pair<String, Resource*>(resource->getName(), resource));
	}
	// Workaround, language string should be extracted from Blackboard
	String lang_name = "KRL";
	try {
		this->language = new process::Language(lang_name);
	} catch (except::Exception &e) {
		this->language = new process::Language();
		CLI::printWarn(e.getMessage() + ". Switching to default, please check you output!");
	}
	return;
}

/**
 * Returns the path of the sfc-file from the Blackboard.
 * @return
 */
String Blackboard::getSFCPath() {
	rapidxml::xml_node<> *pseq = XML::findNode(doc.first_node(), "InstanceHierarchy", "Name", "Blackboard");
	pseq = XML::findNode(pseq, "InternalElement", "Name", "Process Sequenz");
	rapidxml::xml_node<> *value = XML::findNode(pseq, "ExternalInterface", "Name", "SFC Benutzereingabe");
	value = XML::findNode(value, "Attribute", "Name", "refURI");
	value = XML::findNode(value, "Value");
	if (value == NULL)
		return "";
	return getPath() + value->value();
}

/**
 * Returns the language object.
 * @return
 */
process::Language* Blackboard::getLanguage() {
	return this->language;
}

/**
 * Returns the resource object with given name.
 * @param name
 * @return
 */
Resource* Blackboard::getResource(String name) {
	map<String, Resource*>::iterator obj = resources.find(name);
	if (obj == resources.end())
		return NULL;
	else
		return obj->second;
}

/**
 * Sets the path for the generated output file in the Blackboard.
 * @param path
 */
void Blackboard::setCodeURI(String path) {
	rapidxml::xml_node<> *node = XML::findNode(doc.first_node(), "InstanceHierarchy", "Name", "Blackboard");
	node = XML::findNode(node, "InternalElement", "Name", "Code");
	node = XML::findNode(node, "ExternalInterface", "Name", "KRL code");
	node = XML::findNode(node, "Attribute", "Name", "refURI");
	if (node == NULL)
		throw except::Exception("Could not write the reference to the output file to the Blackboard!");
	String refURI = "refURI";
	XML::setAttribute(doc, node, "Name", "refURI");
	XML::setAttribute(doc, node, "AttributeDataType", "xs:anyURI");
	node = XML::findNode(node, "Value");
	if (node == NULL) {
		// TODO add node and insert path
		throw except::Exception("No reference of the output file in the Blackboard!");
	}
	char *code = doc.allocate_string((path + ".src").c_str());
	node->value(code);
}

/**
 * Returns the error log count.
 * @return
 */
int Blackboard::getErrors() {
	int errors = 0;
	for (map<String, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
		errors += (*it).second->getErrors();
	return errors;
}

/**
 * Prints all information of this Blackboard.
 */
void Blackboard::printResources() {
	CLI::printLine("");
	for (map<String, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
		CLI::printLine((*it).second->stStr());
}

} /* namespace aml */
