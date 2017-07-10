/*
 * ParseXML_test.cpp
 *
 *  Created on: Jun 12, 2017
 *      Author: user
 */

#include "ParseXML.h"
#include <cxxtest/TestSuite.h>

class MyTest : public CxxTest::TestSuite
    {
    public:
        void testMethod( void )
        {
            TS_ASSERT( 1 + 1 > 1 );
            TS_ASSERT_EQUALS( 1 + 1, 2 );
        }
    };
