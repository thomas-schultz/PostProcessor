/*
 * IRLFile.h
 *
 *  Created on: 24.01.2015
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef SRC_AML_IRL_IRLFILE_H_
#define SRC_AML_IRL_IRLFILE_H_

#include "tools/String.h"
#include "rapidxml-1.13/rapidxml.hpp"

using namespace std;

namespace aml {

/**
 * Class that represents an irl-file.
 */
class IRLFile {
private:
	String path;
	String content;
	rapidxml::xml_document<> xml;
	rapidxml::xml_node<>* defs;
	rapidxml::xml_node<>* srcs;

public:
	IRLFile(String path);
	virtual ~IRLFile();

	String getPath();
	String getFunDef(String name);
	String getFunCall(String name);
	String toStr();
};

} /* namespace aml */

#endif /* SRC_AML_IRL_IRLFILE_H_ */
