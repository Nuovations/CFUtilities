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
 *      This file implements a unit test for CFUTreeContextInit.
 */

#include <CFUtilities/CFUtilities.hpp>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFUTreeContextInit :
    public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCFUTreeContextInit);
    CPPUNIT_TEST(TestNull);
    CPPUNIT_TEST(TestNonNull);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestNull(void);
    void TestNonNull(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUTreeContextInit);

void
TestCFUTreeContextInit :: TestNull(void)
{
    CFUTreeContextInit(NULL);
}

void
TestCFUTreeContextInit :: TestNonNull(void)
{
    CFTreeContext lContext;

    CFUTreeContextInit(&lContext);
    CPPUNIT_ASSERT(lContext.version == 0);
}
