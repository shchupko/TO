/*
 * Log.h
 *
 *  Created on: Jun 14, 2017
 *      Author: user
 */

#ifndef LOG_H_
#define LOG_H_

#include <string>

namespace Log {

using namespace std;

class Log {
public:
	Log();
	virtual ~Log();

	void Error(const char *fmt, ...);
	void Info(const char *fmt, ...);
	void Debug(const char *fmt, ...);

	void Print(const char *valName, string val);
	void Print(const char *valName, const char* val);
	void Print(const char *valName, double val);
	void Print(const char *valName, unsigned int val);
	void Print(const char *valName, bool   val);
	void Print(const char *fmt, ...);
};


} /* namespace Log */

#endif /* LOG_H_ */
