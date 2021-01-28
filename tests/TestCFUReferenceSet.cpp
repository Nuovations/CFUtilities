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
 *      This file implements a unit test for CFUReferenceSet.
 */

#include <CFUtilities/CFUtilities.hpp>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFUReferenceSet :
    public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCFUReferenceSet);
    CPPUNIT_TEST(TestNullLvalue);
    CPPUNIT_TEST(TestNullRvalue);
    CPPUNIT_TEST(TestNonNull);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestNullLvalue(void);
    void TestNullRvalue(void);
    void TestNonNull(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUReferenceSet);

void
TestCFUReferenceSet :: TestNullLvalue(void)
{
    CFBooleanRef lFirstReference  = NULL;
    CFBooleanRef lSecondReference = kCFBooleanTrue;

    CFUReferenceSet(lFirstReference, lSecondReference);
    CPPUNIT_ASSERT(lFirstReference == lSecondReference);
}

void
TestCFUReferenceSet :: TestNullRvalue(void)
{
    CFBooleanRef lFirstReference  = kCFBooleanFalse;
    CFBooleanRef lSecondReference = NULL;

    CFUReferenceSet(lFirstReference, lSecondReference);
    CPPUNIT_ASSERT(lFirstReference == lSecondReference);
}

void
TestCFUReferenceSet :: TestNonNull(void)
{
    CFBooleanRef lFirstReference  = kCFBooleanFalse;
    CFBooleanRef lSecondReference = kCFBooleanTrue;

    CFUReferenceSet(lFirstReference, lSecondReference);
    CPPUNIT_ASSERT(lFirstReference == lSecondReference);
}
