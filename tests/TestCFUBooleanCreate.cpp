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
 *      This file implements a unit test for CFUBooleanCreate.
 */

#include <CFUtilities/CFUtilities.hpp>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFUBooleanCreate :
    public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCFUBooleanCreate);
    CPPUNIT_TEST(TestFalse);
    CPPUNIT_TEST(TestTrue);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestFalse(void);
    void TestTrue(void);

private:
    void TestBoolean(const bool & aBoolean);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUBooleanCreate);

void
TestCFUBooleanCreate :: TestFalse(void)
{
    const bool kTestValue = false;

    TestBoolean(kTestValue);
}

void
TestCFUBooleanCreate :: TestTrue(void)
{
    const bool kTestValue = true;

    TestBoolean(kTestValue);
}

void
TestCFUBooleanCreate :: TestBoolean(const bool & aBoolean)
{
    CFBooleanRef lBooleanRef = NULL;
    bool         lStatus;
    Boolean      lValue;

    lBooleanRef = CFUBooleanCreate(aBoolean);
    CPPUNIT_ASSERT(lBooleanRef != NULL);

    lStatus = CFUIsTypeID(lBooleanRef, CFBooleanGetTypeID());
    CPPUNIT_ASSERT(lStatus == true);

    lValue = CFBooleanGetValue(lBooleanRef);
    CPPUNIT_ASSERT(lValue == aBoolean);

    CFRelease(lBooleanRef);
}
