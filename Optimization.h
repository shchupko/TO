/*
 * Optimization.h
 *
 *  Created on: Jun 20, 2017
 *      Author: user
 */

#ifndef OPTIMIZATION_H_
#define OPTIMIZATION_H_

//namespace TO {

#include "GUIXml.h"
#include "LoadCases.h"

class Optimization : public ParseXML {
public:
	Optimization();
	~Optimization();
	void Loop(LoadCases & LC, GUIXml & GUIXml);
};

//} /* namespace TO */

#endif /* OPTIMIZATION_H_ */
