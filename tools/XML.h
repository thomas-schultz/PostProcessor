/*
 * xml.h
 *
 *  Created on: 19.01.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 *
 *	Header file for handling all xml related stuff.
 */

#ifndef XML_XML_H_
#define XML_XML_H_

#include <iostream>
#include <list>
#include <cstring>
#include <fstream>

#include "String.h"
#include "TextFile.h"
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include "exceptions/Exception.h"

using namespace std;

class XML {

public:

	/**
	 * Finds the first child node with given name.
	 * @param node
	 * @param name
	 * @return	NULL if not found
	 */
	static rapidxml::xml_node<>* findNode(rapidxml::xml_node<>* node, String name) {
		return findNode(node, name, "", "");
	}

	/**
	 * Finds the first child node with given name and matching
	 * attribute, attr_name has the value attr_value.
	 * @param node
	 * @param name
	 * @param attr_name
	 * @param attr_value
	 * @return	NULL if not found
	 */
	static rapidxml::xml_node<>* findNode(rapidxml::xml_node<>* node, String name, String attr_name,
			String attr_value) {
		if (node == NULL)
			return NULL;
		rapidxml::xml_node<> *curNode = node->first_node();
		while (curNode != NULL) {
			String nodeName = String(curNode->name()).trim();
			if (!strcmp(nodeName.c_str(), name.c_str())) {
				if (attr_name.size() > 0) {
					String attr = getAttribute(curNode, attr_name, "");
					if (!strcmp(attr_value.c_str(), attr.c_str()))
						return curNode;
				} else {
					return curNode;
				}
			}
			curNode = curNode->next_sibling();
		}
		return NULL;
	}

	/**
	 * Finds all child nodes with given name.
	 * @param node
	 * @param name
	 * @return	empty list if nothing found
	 */
	static list<rapidxml::xml_node<>*> findNodes(rapidxml::xml_node<>* node, String name) {
		return findNodes(node, name, "", "");
	}

	/**
	 * Finds all child nodes with given name and matching
	 * attribute, attr_name has the value attr_value.
	 * @param node
	 * @param name
	 * @param attr_name
	 * @param attr_value
	 * @return	empty list if nothing found
	 */
	static list<rapidxml::xml_node<>*> findNodes(rapidxml::xml_node<>* node, String name, String attr_name,
			String attr_value) {
		list<rapidxml::xml_node<>*> list;
		if (node == NULL)
			return list;
		rapidxml::xml_node<> *curNode = node->first_node();
		while (curNode != NULL) {
			if (name.length() == 0) {
				list.push_back(curNode);
			} else if (!strcmp(curNode->name(), name.c_str())) {
				if (attr_name.size() > 0) {
					String attr = getAttribute(curNode, attr_name, "");
					if (!strcmp(attr_value.c_str(), attr.c_str()))
						list.push_back(curNode);
				} else {
					list.push_back(curNode);
				}
			}
			curNode = curNode->next_sibling();
		}
		return list;
	}

	/**
	 * Extracts the value of the node. If not present,
	 * the replacement string is returned.
	 * @param node
	 * @param replmnt
	 * @return
	 */
	static String getValue(rapidxml::xml_node<>* node, String replmnt) {
		if (node == NULL)
			return replmnt;
		return node->value();
	}

	/**
	 * Extracts the value of the first attributes which matches the name.
	 * If not present, the replacement string is returned.
	 * @param node
	 * @param name
	 * @param replmnt
	 * @return
	 */
	static String getAttribute(rapidxml::xml_node<>* node, String name, String replmnt) {
		if (node == NULL)
			return replmnt;
		rapidxml::xml_attribute<> *attr = node->first_attribute(name.c_str());
		if (attr == NULL)
			return replmnt;
		String value = attr->value();
		return value.trim();
	}

	/**
	 * Sets the value of the attributes which matches the name to given value
	 * @param doc
	 * @param node
	 * @param name
	 * @param value
	 */
	static void setAttribute(rapidxml::xml_document<> &doc, rapidxml::xml_node<>* node, String name, String value) {
		if (node == NULL)
			return;
		rapidxml::xml_attribute<> *attr = node->first_attribute(name.c_str());
		String *val = new String(value);
		if (attr == NULL) {
			rapidxml::xml_attribute<> *attr = doc.allocate_attribute(name.c_str(), val->c_str());
			node->append_attribute(attr);
			return;
		}
		attr->value(val->c_str());
		return;
	}

	/**
	 * Saves the XML file at path
	 * @param path
	 * @param str
	 * throws FileNotFoundException
	 */
	static void saveToFile(String path, String str) {
		TextFile::saveToFile(path, str);
	}

	/**
	 * Checks if the string value is of datatype tpye.
	 * @param type
	 * @param value
	 * @return
	 */
	static bool isValidDataType(String type, String value) {
		if (type == "") {
			return true;
		} else if (type == "xs:double") {
			double num;
			return (sscanf(value.c_str(), "%lg", &num) == 1);
		} else if (type == "xs:decimal") {
			int num;
			return (sscanf(value.c_str(), "%d", &num) == 1);
		} else if (type == "xs:boolean") {
			return (value.getUpper() == "TRUE" || value.getUpper() == "FALSE");
		}
		return false;
	}

};

#endif /* XML_XML_H_ */
