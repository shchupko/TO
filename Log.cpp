/*
 * Log.cpp
 *
 *  Created on: Jun 14, 2017
 *      Author: user
 */

#include <stdio.h> // printf
#include <stdarg.h>  //va_start
#include "Log.h"

namespace Log {

#define MAX_MSG_LENGTH 256


Log::Log() {}
Log::~Log() {}

void Log::Debug(const char *fmt, ...){
    char buf[MAX_MSG_LENGTH];

    va_list args;
    va_start (args, fmt);
    vsprintf (buf, fmt, args);
    va_end (args);

   // printf("Debug:\t %s\n", buf);
    //PetscPrintf(PETSC_COMM_WORLD, "Debug: %s. %s line:%d, %s\n", buf, __FUNCTION__, __LINE__, __FILE__);
}

void Log::Error(const char *fmt, ...){
    char buf[MAX_MSG_LENGTH];

    va_list args;
    va_start (args, fmt);
    vsprintf (buf, fmt, args);
    va_end (args);

    printf("Error:\n\t %s\n", buf);
}

void Log::Info(const char *fmt, ...){
    char buf[MAX_MSG_LENGTH];

    va_list args;
    va_start (args, fmt);
    vsprintf (buf, fmt, args);
    va_end (args);

    printf("%s\n", buf);
}

void Log::Print(const char *valName, string val){
	Info("\t%s: %s", valName, val.c_str());
}

void Log::Print(const char *valName, const char* val){
	Info("\t%s: %s", valName, val);
}

void Log::Print(const char *valName, double val){
	Info("\t%s: %f", valName, val);
}

void Log::Print(const char *valName, unsigned int val){
	Info("\t%s: %i", valName, val);
}

void Log::Print(const char *valName, bool   val){
	Info("\t%s: %s", valName, val ? "true" : "false");
}
void Log::Print(const char *fmt, ...){
    char buf[MAX_MSG_LENGTH];

    va_list args;
    va_start (args, fmt);
    vsprintf (buf, fmt, args);
    va_end (args);

	Info("\t%s", buf);
}

} /* namespace TO */
