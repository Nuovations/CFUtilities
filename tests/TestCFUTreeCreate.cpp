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
 *      This file implements a unit test for CFUTreeCreate.
 */

#include <CFUtilities/CFUtilities.hpp>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFUTreeCreate :
    public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCFUTreeCreate);
    CPPUNIT_TEST(Test);
    CPPUNIT_TEST_SUITE_END();

public:
    void Test(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUTreeCreate);

void
TestCFUTreeCreate :: Test(void)
{
    CFStringRef lString      = CFSTR("Test String");
    CFTreeRef   lCreatedTree = NULL;
    CFTreeRef   lTree;
    CFIndex     lChildCount;

    lCreatedTree = CFUTreeCreate(lString);
    CPPUNIT_ASSERT(lCreatedTree != NULL);

    lTree = CFTreeGetParent(lCreatedTree);
    CPPUNIT_ASSERT(lTree == NULL);

    lTree = CFTreeGetNextSibling(lCreatedTree);
    CPPUNIT_ASSERT(lTree == NULL);

    lTree = CFTreeGetFirstChild(lCreatedTree);
    CPPUNIT_ASSERT(lTree == NULL);

    lChildCount = CFTreeGetChildCount(lCreatedTree);
    CPPUNIT_ASSERT(lChildCount == 0);

    if (lCreatedTree != NULL) {
        CFRelease(lCreatedTree);
    }
}
