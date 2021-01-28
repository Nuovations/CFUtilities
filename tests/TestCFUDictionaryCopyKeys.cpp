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
 *      This file implements a unit test for CFUDictionaryCopyKeys.
 */

#include <CFUtilities/CFUtilities.hpp>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFUDictionaryCopyKeys :
    public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCFUDictionaryCopyKeys);
    CPPUNIT_TEST(TestNull);
    CPPUNIT_TEST(TestUnpopulated);
    CPPUNIT_TEST(TestPopulated);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestNull(void);
    void TestUnpopulated(void);
    void TestPopulated(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUDictionaryCopyKeys);

void
TestCFUDictionaryCopyKeys :: TestNull(void)
{
    CFDictionaryRef lDictionaryRef = NULL;
    CFArrayRef      lKeyArrayRef   = NULL;

    lKeyArrayRef = CFUDictionaryCopyKeys(lDictionaryRef);
    CPPUNIT_ASSERT(lKeyArrayRef == NULL);
}

void
TestCFUDictionaryCopyKeys :: TestUnpopulated(void)
{
    const size_t    lKeyCount      = 0;
    CFDictionaryRef lDictionaryRef = NULL;
    CFArrayRef      lKeyArrayRef   = NULL;
    CFIndex         lKeyArrayCount;

    lDictionaryRef = CFDictionaryCreate(kCFAllocatorDefault,
                                        NULL,
                                        NULL,
                                        lKeyCount,
                                        &kCFTypeDictionaryKeyCallBacks,
                                        &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(lDictionaryRef != NULL);

    lKeyArrayRef = CFUDictionaryCopyKeys(lDictionaryRef);
    CPPUNIT_ASSERT(lKeyArrayRef != NULL);

    lKeyArrayCount = CFArrayGetCount(lKeyArrayRef);
    CPPUNIT_ASSERT(lKeyArrayCount == lKeyCount);

    if (lDictionaryRef != NULL) {
        CFRelease(lDictionaryRef);
    }

    if (lKeyArrayRef != NULL) {
        CFRelease(lKeyArrayRef);
    }
}

void
TestCFUDictionaryCopyKeys :: TestPopulated(void)
{
    CFDictionaryRef lDictionaryRef     = NULL;
    const size_t    lKeyCount          = 4;
    const void *    lKeys[lKeyCount]   = {
        CFSTR("Test Key 1"),
        CFSTR("Test Key 2"),
        CFSTR("Test Key 3"),
        CFSTR("Test Key 4")
    };
    const void *    lValues[lKeyCount] = {
        CFSTR("Test Value 1"),
        CFSTR("Test Value 2"),
        CFSTR("Test Value 3"),
        CFSTR("Test Value 4")
    };
    CFArrayRef      lKeyArrayRef       = NULL;
    CFIndex         lKeyArrayCount;
    Boolean         lResult;

    lDictionaryRef = CFDictionaryCreate(kCFAllocatorDefault,
                                        &lKeys[0],
                                        &lValues[0],
                                        lKeyCount,
                                        &kCFTypeDictionaryKeyCallBacks,
                                        &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(lDictionaryRef != NULL);

    lKeyArrayRef = CFUDictionaryCopyKeys(lDictionaryRef);
    CPPUNIT_ASSERT(lKeyArrayRef != NULL);

    lKeyArrayCount = CFArrayGetCount(lKeyArrayRef);
    CPPUNIT_ASSERT(lKeyArrayCount == lKeyCount);

    lResult = CFArrayContainsValue(lKeyArrayRef,
                                   CFRangeMake(0, lKeyCount),
                                   lKeys[0]);
    CPPUNIT_ASSERT(lResult == true);

    lResult = CFArrayContainsValue(lKeyArrayRef,
                                   CFRangeMake(0, lKeyCount),
                                   lKeys[1]);
    CPPUNIT_ASSERT(lResult == true);

    lResult = CFArrayContainsValue(lKeyArrayRef,
                                   CFRangeMake(0, lKeyCount),
                                   lKeys[2]);
    CPPUNIT_ASSERT(lResult == true);

    lResult = CFArrayContainsValue(lKeyArrayRef,
                                   CFRangeMake(0, lKeyCount),
                                   lKeys[3]);
    CPPUNIT_ASSERT(lResult == true);

    if (lDictionaryRef != NULL) {
        CFRelease(lDictionaryRef);
    }

    if (lKeyArrayRef != NULL) {
        CFRelease(lKeyArrayRef);
    }
}
