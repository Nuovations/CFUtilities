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
 *      This file implements a unit test for CFUSetUnionSet.
 */

#include <CFUtilities/CFUtilities.hpp>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFUSetUnionSet :
    public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCFUSetUnionSet);
    CPPUNIT_TEST(TestNullNonMutable);
    CPPUNIT_TEST(TestNullMutable);
    CPPUNIT_TEST(TestNullBoth);
    CPPUNIT_TEST(TestNonNull);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestNullNonMutable(void);
    void TestNullMutable(void);
    void TestNullBoth(void);
    void TestNonNull(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUSetUnionSet);

void
TestCFUSetUnionSet :: TestNullNonMutable(void)
{
    CFMutableSetRef lMutableSetRef = NULL;
    CFSetRef        lSetRef        = NULL;

    lSetRef = CFSetCreate(kCFAllocatorDefault, NULL, 0, &kCFTypeSetCallBacks);
    CPPUNIT_ASSERT(lSetRef != NULL);

    CFUSetUnionSet(lMutableSetRef, lSetRef);
    CPPUNIT_ASSERT(lMutableSetRef == NULL);

    if (lSetRef != NULL) {
        CFRelease(lSetRef);
    }
}

void
TestCFUSetUnionSet :: TestNullMutable(void)
{
    CFMutableSetRef lMutableSetRef = NULL;
    CFSetRef        lSetRef        = NULL;

    lMutableSetRef = CFSetCreateMutable(kCFAllocatorDefault,
                                        0,
                                        &kCFTypeSetCallBacks);
    CPPUNIT_ASSERT(lMutableSetRef != NULL);

    CFUSetUnionSet(lMutableSetRef, lSetRef);
    CPPUNIT_ASSERT(lSetRef == NULL);

    if (lMutableSetRef != NULL) {
        CFRelease(lMutableSetRef);
    }
}

void
TestCFUSetUnionSet :: TestNullBoth(void)
{
    CFMutableSetRef lMutableSetRef = NULL;
    CFSetRef        lSetRef        = NULL;

    CFUSetUnionSet(lMutableSetRef, lSetRef);
    CPPUNIT_ASSERT(lMutableSetRef == NULL);
    CPPUNIT_ASSERT(lSetRef == NULL);
}

void
TestCFUSetUnionSet :: TestNonNull(void)
{
    CFStringRef     lAString          = CFSTR("A");
    CFStringRef     lBString          = CFSTR("B");
    CFStringRef     lCString          = CFSTR("C");
    CFStringRef     lDString          = CFSTR("D");
    const void *    lCFStringArray[2] = { lBString, lDString };
    CFMutableSetRef lMutableSetRef    = NULL;
    CFSetRef        lSetRef           = NULL;
    Boolean         lResult;

    lMutableSetRef = CFSetCreateMutable(kCFAllocatorDefault,
                                        0,
                                        &kCFTypeSetCallBacks);
    CPPUNIT_ASSERT(lMutableSetRef != NULL);

    CFSetAddValue(lMutableSetRef, lAString);
    CFSetAddValue(lMutableSetRef, lCString);

    lSetRef = CFSetCreate(kCFAllocatorDefault,
                          &lCFStringArray[0],
                          sizeof (lCFStringArray) / sizeof (lCFStringArray[0]),
                          &kCFTypeSetCallBacks);
    CPPUNIT_ASSERT(lSetRef != NULL);

    CFUSetUnionSet(lMutableSetRef, lSetRef);
    CPPUNIT_ASSERT(lMutableSetRef != NULL);
    CPPUNIT_ASSERT(lSetRef != NULL);

    lResult = CFSetContainsValue(lMutableSetRef, lAString);
    CPPUNIT_ASSERT(lResult == true);

    lResult = CFSetContainsValue(lMutableSetRef, lBString);
    CPPUNIT_ASSERT(lResult == true);

    lResult = CFSetContainsValue(lMutableSetRef, lCString);
    CPPUNIT_ASSERT(lResult == true);

    lResult = CFSetContainsValue(lMutableSetRef, lDString);
    CPPUNIT_ASSERT(lResult == true);

    if (lMutableSetRef != NULL) {
        CFRelease(lMutableSetRef);
    }

    if (lSetRef != NULL) {
        CFRelease(lSetRef);
    }
}
