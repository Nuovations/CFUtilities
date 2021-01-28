/*
 *    Copyright (c) 2021 Nuovation System Designs, LLC
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 *    @file
 *      This file implements a unit test for CFUAbsoluteTimeGetPOSIXTime.
 */

#include <CFUtilities/CFUtilities.hpp>

#include <time.h>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFUAbsoluteTimeGetPOSIXTime :
    public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCFUAbsoluteTimeGetPOSIXTime);
    CPPUNIT_TEST(Test);
    CPPUNIT_TEST_SUITE_END();

public:
    void Test(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUAbsoluteTimeGetPOSIXTime);

void
TestCFUAbsoluteTimeGetPOSIXTime :: Test(void)
{
    const time_t         lPOSIXNow     = time(NULL);
    const time_t         lPOSIXEpsilon = 1;
    const CFAbsoluteTime lCFNow        = CFAbsoluteTimeGetCurrent();
    time_t               lResult;

    lResult = CFUAbsoluteTimeGetPOSIXTime(lCFNow);
    CPPUNIT_ASSERT(lResult > lPOSIXNow - lPOSIXEpsilon);
    CPPUNIT_ASSERT(lResult < lPOSIXNow + lPOSIXEpsilon);
}
