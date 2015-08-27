/*
 * SFCHelper.h
 *
 *  Created on: 28.12.2014
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef SRC_SFC_SFCHELPER_H_
#define SRC_SFC_SFCHELPER_H_

#include <iostream>
#include <cstring>
#include <vector>

#include "tools/String.h"
#include "tools/XML.h"
#include "rapidxml-1.13/rapidxml.hpp"

using namespace std;

/**
 * Helper class for all sfc-realted stuff, kind of a XML wrapper.
 */
class SFC {
public:

	/**
	 * Finds the first child node with given name.
	 * @param node
	 * @param name
	 * @return	NULL if not found
	 */
	static rapidxml::xml_node<>* findNode(rapidxml::xml_node<>* node, String name) {
		return XML::findNode(node, name);
	}

	/**
	 * Finds all child nodes with given name.
	 * @param node
	 * @param name
	 * @return	empty list if nothing found
	 */
	static list<rapidxml::xml_node<>*> findNodes(rapidxml::xml_node<>* node, String name) {
		return XML::findNodes(node, name);
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
		return XML::getAttribute(node, name, replmnt);
	}

	/**
	 * Extracts the local-id of the given xml-node.
	 * @param node
	 * @return	-1 if not found
	 */
	static int getLocalId(rapidxml::xml_node<>* node) {
		String lid = XML::getAttribute(node, "localId", "-1");
		return atoi(lid.c_str());
	}

	/**
	 * Extracts the frist reflocal-id of the given xml-node.
	 * @param node
	 * @return	-1 if not found
	 */
	static int getRefId(rapidxml::xml_node<>* node) {
		rapidxml::xml_node<> *connectionPointIn = XML::findNode(node, "connectionPointIn");
		rapidxml::xml_node<> *connection = XML::findNode(connectionPointIn, "connection");
		if (connection == NULL)
			return -1;
		String rid = XML::getAttribute(connection, "refLocalId", "-1");
		return atoi(rid.c_str());
	}

	/**
	 * Extracts a list of reflocal-id of the given xml-node.
	 * @param node
	 * @return	empty list if nothing found
	 */
	static vector<int> getRefIds(rapidxml::xml_node<>* node) {
		vector<int> ref_ids;
		list<rapidxml::xml_node<>*> connectionPointIn = XML::findNodes(node, "connectionPointIn");
		for (list<rapidxml::xml_node<>*>::iterator it = connectionPointIn.begin(); it != connectionPointIn.end();
				++it) {
			rapidxml::xml_node<> *curNode = (*it);
			rapidxml::xml_node<> *connection = XML::findNode(curNode, "connection");
			if (connection == NULL)
				continue;
			String rid = XML::getAttribute(connection, "refLocalId", "-1");
			ref_ids.push_back(atoi(rid.c_str()));
		}
		return ref_ids;
	}

	/**
	 * Extracts the duration field of the given xml-node.
	 * @param node
	 * @return	0.0 if not found
	 */
	static double getDuration(rapidxml::xml_node<>* node) {
		rapidxml::xml_node<>* time = XML::findNode(node, "addData");
		time = XML::findNode(time, "data");
		time = XML::findNode(time, "iwb");
		time = XML::findNode(time, "time");
		if (time == NULL)
			return 0.0;
		String dur = XML::getAttribute(time, "duration", "0.0");
		return std::strtod(dur.c_str(), NULL);
	}

	/**
	 * Extract the resource field of the given xml-node.
	 * @param node
	 * @return	empty string if not found
	 */
	static String getResource(rapidxml::xml_node<>* node) {
		rapidxml::xml_node<>* res = XML::findNode(node, "addData");
		res = XML::findNode(res, "data");
		res = XML::findNode(res, "iwb");
		res = XML::findNode(res, "ressources");
		res = XML::findNode(res, "ressource");
		if (res == NULL)
			return "";
		return XML::getAttribute(res, "name", "");
	}

	/**
	 * Extract the skillname field of the given xml-node.
	 * @param node
	 * @return	empty string if not found
	 */
	static String getSkillName(rapidxml::xml_node<>* node) {
		rapidxml::xml_node<>* skill = XML::findNode(node, "addData");
		skill = XML::findNode(skill, "data");
		skill = XML::findNode(skill, "iwb");
		skill = XML::findNode(skill, "skill");
		if (skill == NULL)
			return "";
		return XML::getAttribute(skill, "name", "");
	}

	/**
	 * Extract the content of the CDATA field of the given xml-node.
	 * @param node
	 * @return empty string if not found
	 */
	static String getCDATA(rapidxml::xml_node<>* node) {
		rapidxml::xml_node<>* data = XML::findNode(node, "condition");
		data = XML::findNode(data, "inline");
		data = XML::findNode(data, "ST");
		data = XML::findNode(data, "xhtml:p");
		if (data == NULL)
			return "";
		rapidxml::xml_node<>* cdata = data->first_node();
		if (cdata == NULL)
			return "";
		return cdata->value();
	}

};

#endif /* SRC_SFC_SFCHELPER_H_ */
