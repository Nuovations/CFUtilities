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
 *      This file implements a unit test for CFUSetIsEmptySet.
 */

#include <CFUtilities/CFUtilities.hpp>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFUSetIsEmptySet :
    public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCFUSetIsEmptySet);
    CPPUNIT_TEST(TestNull);
    CPPUNIT_TEST(TestEmpty);
    CPPUNIT_TEST(TestNonEmpty);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestNull(void);
    void TestEmpty(void);
    void TestNonEmpty(void);

private:
    void TestSet(CFSetRef aSet, bool aIsEmpty);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUSetIsEmptySet);

void
TestCFUSetIsEmptySet :: TestNull(void)
{
    const bool kIsEmpty = true;
    CFSetRef   lSetRef  = NULL;


    TestSet(lSetRef, kIsEmpty);
}

void
TestCFUSetIsEmptySet :: TestEmpty(void)
{
    const bool      kIsEmpty = true;
    CFMutableSetRef lSetRef  = NULL;


    lSetRef = CFSetCreateMutable(kCFAllocatorDefault, 0, &kCFTypeSetCallBacks);
    CPPUNIT_ASSERT(lSetRef != NULL);
    TestSet(lSetRef, kIsEmpty);

    CFRelease(lSetRef);
}

void
TestCFUSetIsEmptySet :: TestNonEmpty(void)
{
    const bool      kIsEmpty = true;
    CFMutableSetRef lSetRef  = NULL;


    lSetRef = CFSetCreateMutable(kCFAllocatorDefault, 0, &kCFTypeSetCallBacks);
    CPPUNIT_ASSERT(lSetRef != NULL);

    CFSetAddValue(lSetRef, kCFNull);

    TestSet(lSetRef, !kIsEmpty);

    CFRelease(lSetRef);
}

void
TestCFUSetIsEmptySet :: TestSet(CFSetRef aSet, bool aIsEmpty)
{
    bool lIsEmpty;


    lIsEmpty = CFUSetIsEmptySet(aSet);
    CPPUNIT_ASSERT(lIsEmpty == aIsEmpty);
}
