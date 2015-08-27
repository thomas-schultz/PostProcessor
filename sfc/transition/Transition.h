/*
 * Transition.h
 *
 *  Created on: 28.11.2014
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef SFC_TRANSITION_
#define SFC_TRANSITION_

#include <vector>

#include "sfc/SFCObject.h"
#include "sfc/SFCFile.h"
#include "tools/String.h"
#include "rapidxml-1.13/rapidxml.hpp"

using namespace std;

using namespace std;

namespace sfc {

/**
 * Transition class, subtype of a sfc-object.
 */
class SFCFile;

class Transition: public SFCObject {
protected:
	String name;

public:
	Transition(String name);
	virtual ~Transition();

	virtual void extractData(rapidxml::xml_node<> *node);
	bool isSimple();
	bool isDivergence();
	bool isConvergence();

	virtual String toStr();
};

} /* namespace sfc */

#endif /* SFC_TRANSITION_ */
