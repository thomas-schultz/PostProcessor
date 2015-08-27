/*
 * SkillStep.h
 *
 *  Created on: 28.11.2014
 *      Author: Thomas Schultz
 *      Mail:	thomas.schultz@mytum.de
 */

#ifndef SFC_STEPS_SKILLSTEP_H_
#define SFC_STEPS_SKILLSTEP_H_

#include <string>
#include <list>
#include <vector>

#include "sfc/SFCObject.h"
#include "sfc/SFCFile.h"
#include "sfc/variable/Variable.h"
#include "tools/String.h"
#include "rapidxml-1.13/rapidxml.hpp"

using namespace std;

namespace sfc {

class SFCFile;

/**
 * SkillStep class, subtype of a sfc-object.
 */
class SkillStep: public SFCObject {
protected:
	String name;
	bool initial;
	bool final;
	String resource;
	String skill;
	double duration;
	vector<String> needed_vars;
	map<String, Variable*> vars;

public:
	SkillStep();
	SkillStep(String name, vector<String> vars);
	virtual ~SkillStep();

	String getName();
	int getRefId();
	int getNext();
	bool isInitial();
	bool isFinal();
	String getResource();
	String getSkill();
	double getDuration();

	void addNext(int id);
	virtual void extractData(rapidxml::xml_node<> *node);
	void registerVariable(String name);
	bool isRegisteredVariable(String name);
	void addVariable(Variable *var);
	Variable* getVariable(String name);
	virtual String toStr();
};

} /* namespace SFC */

#endif /* SFC_STEPS_SKILLSTEP_H_ */
