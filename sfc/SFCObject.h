/*
 * SFCObject.h
 *
 *  Created on: 13.12.2014
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef SRC_SFC_SFCOBJECTS_SFCOBJECT_H_
#define SRC_SFC_SFCOBJECTS_SFCOBJECT_H_

#include <vector>

#include "tools/String.h"

using namespace std;

namespace sfc {

const int PRINT_WIDTH = 12;

enum OBJECT {
	STEP, TRANS, VOID
};

/**
 * Superclass for all sfc-objects.
 */
class SFCObject {
protected:
	OBJECT obj;
	int local_id;
	vector<int> ref_ids;
	vector<int> nexts;
	bool connected;

public:
	SFCObject();
	virtual ~SFCObject();

	bool isConnected();
	void setConnected(bool connected);
	bool isStep();
	bool isTransition();
	virtual void addNext(int id);

	int getId();
	vector<int> getRefIds();
	vector<int> getNexts();
	virtual String toStr();
};

} /* namespace sfc */

#endif /* SRC_SFC_SFCOBJECTS_SFCOBJECT_H_ */
