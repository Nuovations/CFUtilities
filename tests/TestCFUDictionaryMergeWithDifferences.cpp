/*
 *    Copyright (c) 2023 Nuovation System Designs, LLC
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
 *      This file implements a unit test for the C/C++ bindings for
 *      TestCFUDictionaryMergeWithDifferences.
 */

#include <CFUtilities/CFUtilities.hpp>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFUDictionaryMergeWithDifferences :
    public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCFUDictionaryMergeWithDifferences);
    CPPUNIT_TEST(TestNull);
    CPPUNIT_TEST(TestBaseWithNoDifferences);
    CPPUNIT_TEST(TestBaseWithAddedOnly);
    CPPUNIT_TEST(TestBaseWithCommonOnly);
    CPPUNIT_TEST(TestBaseWithRemovedOnly);
    CPPUNIT_TEST(TestBaseWithAddedAndCommonOnly);
    CPPUNIT_TEST(TestBaseWithAddedAndRemovedOnly);
    CPPUNIT_TEST(TestBaseWithCommonAndRemovedOnly);
    CPPUNIT_TEST(TestBaseWithCommonDifferentValues);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestNull(void);
    void TestBaseWithNoDifferences(void);
    void TestBaseWithAddedOnly(void);
    void TestBaseWithCommonOnly(void);
    void TestBaseWithRemovedOnly(void);
    void TestBaseWithAddedAndCommonOnly(void);
    void TestBaseWithAddedAndRemovedOnly(void);
    void TestBaseWithCommonAndRemovedOnly(void);
    void TestBaseWithCommonSameValues(void);
    void TestBaseWithCommonDifferentValues(void);

private:
    void TestDictionaryPopulateWithKeysAndValues(const void **inFirstKey,
                                                 const void **inFirstValue,
                                                 const size_t &inCount,
                                                 CFMutableDictionaryRef &inOutDictionary);
    void TestArrayCreateWithValues(const void **inFirstValue,
                                   const size_t &inCount,
                                   CFMutableArrayRef &outArray);
    void TestDictionaryCreateWithKeysAndValues(const void **inFirstKey,
                                               const void **inFirstValue,
                                               const size_t &inCount,
                                               CFMutableDictionaryRef &outDictionary);
    void TestSetup(const void **inFirstBaseDictionaryKey,
                   const void **inFirstBaseDictionaryValue,
                   const size_t &inBaseDictionaryCount,
                   CFMutableDictionaryRef &outBaseDictionaryRef,
                   const void **inFirstAddedDictionaryKey,
                   const void **inFirstAddedDictionaryValue,
                   const size_t &inAddedDictionaryCount,
                   CFMutableDictionaryRef &outAddedDictionaryRef,
                   const void **inFirstCommonDictionaryKey,
                   const void **inFirstCommonDictionaryValue,
                   const size_t &inCommonDictionaryCount,
                   CFMutableDictionaryRef &outCommonDictionaryRef,
                   const void **inFirstRemovedDictionaryKey,
                   const void **inFirstRemovedDictionaryValue,
                   const size_t &inRemovedDictionaryCount,
                   CFMutableDictionaryRef &outRemovedDictionaryRef,
                   const void **inFirstRemovedKeysArrayKey,
                   const size_t &inRemovedKeysArrayCount,
                   CFMutableArrayRef &outRemovedKeysArrayRef);
    void TestDictionaryKeysAndValues(CFDictionaryRef inDictionary,
                                     const void **inExpectedFirstKey,
                                     const void **inExpectedFirstValue,
                                     const size_t &inExpectedCount);
    void TestTeardown(CFMutableDictionaryRef &inBaseDictionaryRef,
                      CFMutableDictionaryRef &inAddedDictionaryRef,
                      CFMutableDictionaryRef &inCommonDictionaryRef,
                      CFMutableDictionaryRef &inRemovedDictionaryRef,
                      CFMutableArrayRef &inRemovedKeysArrayRef);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUDictionaryMergeWithDifferences);

void
TestCFUDictionaryMergeWithDifferences :: TestNull(void)
{
    CFMutableDictionaryRef lBaseDictionaryRef     = nullptr;
    CFMutableDictionaryRef lAddedDictionaryRef    = nullptr;
    CFMutableDictionaryRef lCommonDictionaryRef   = nullptr;
    CFMutableDictionaryRef lRemovedDictionaryRef  = nullptr;
    CFMutableArrayRef      lRemovedKeysArrayRef   = nullptr;
    Boolean                lStatus;

    // Test Setup

    TestSetup(nullptr,
              nullptr,
              0,
              lBaseDictionaryRef,
              nullptr,
              nullptr,
              0,
              lAddedDictionaryRef,
              nullptr,
              nullptr,
              0,
              lCommonDictionaryRef,
              nullptr,
              nullptr,
              0,
              lRemovedDictionaryRef,
              nullptr,
              0,
              lRemovedKeysArrayRef);

    // C Bindings

    lStatus = CFUDictionaryMergeWithDifferences(nullptr,
                                                lAddedDictionaryRef,
                                                lCommonDictionaryRef,
                                                lRemovedDictionaryRef);
    CPPUNIT_ASSERT(lStatus == false);

    lStatus = CFUDictionaryMergeWithDifferencesAndRemovedKeys(nullptr,
                                                              lAddedDictionaryRef,
                                                              lCommonDictionaryRef,
                                                              lRemovedKeysArrayRef);
    CPPUNIT_ASSERT(lStatus == false);

    // C++ Bindings

    lStatus = CFUDictionaryMerge(nullptr,
                                 lAddedDictionaryRef,
                                 lCommonDictionaryRef,
                                 lRemovedDictionaryRef);
    CPPUNIT_ASSERT(lStatus == false);

    lStatus = CFUDictionaryMerge(nullptr,
                                 lAddedDictionaryRef,
                                 lCommonDictionaryRef,
                                 lRemovedKeysArrayRef);
    CPPUNIT_ASSERT(lStatus == false);

    // Test Teardown

    TestTeardown(lBaseDictionaryRef,
                 lAddedDictionaryRef,
                 lCommonDictionaryRef,
                 lRemovedDictionaryRef,
                 lRemovedKeysArrayRef);
}

void
TestCFUDictionaryMergeWithDifferences :: TestBaseWithNoDifferences(void)
{
    constexpr size_t       kBaseKeyCount              = 2;
    const void *           kBaseKeys[kBaseKeyCount]   = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4")
    };
    const void *           kBaseValues[kBaseKeyCount] = {
        CFSTR("Test Value 2"),
        CFSTR("Test Value 4")
    };
    CFMutableDictionaryRef lBaseDictionaryRef         = nullptr;
    CFMutableDictionaryRef lAddedDictionaryRef        = nullptr;
    CFMutableDictionaryRef lCommonDictionaryRef       = nullptr;
    CFMutableDictionaryRef lRemovedDictionaryRef      = nullptr;
    CFMutableArrayRef      lRemovedKeysArrayRef       = nullptr;
    Boolean                lStatus;

    // Test Setup

    TestSetup(&kBaseKeys[0],
              &kBaseValues[0],
              kBaseKeyCount,
              lBaseDictionaryRef,
              nullptr,
              nullptr,
              0,
              lAddedDictionaryRef,
              nullptr,
              nullptr,
              0,
              lCommonDictionaryRef,
              nullptr,
              nullptr,
              0,
              lRemovedDictionaryRef,
              nullptr,
              0,
              lRemovedKeysArrayRef);

    // C Bindings

    lStatus = CFUDictionaryMergeWithDifferences(lBaseDictionaryRef,
                                                nullptr,
                                                nullptr,
                                                nullptr);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // There were no differences applied, so the expected key/value
    // pairs should be exactly those of the base.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kBaseKeys[0],
                                &kBaseValues[0],
                                kBaseKeyCount);

    lStatus = CFUDictionaryMergeWithDifferencesAndRemovedKeys(lBaseDictionaryRef,
                                                              nullptr,
                                                              nullptr,
                                                              nullptr);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // There were no differences applied, so the expected key/value
    // pairs should be exactly those of the base.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kBaseKeys[0],
                                &kBaseValues[0],
                                kBaseKeyCount);

    // C++ Bindings

    lStatus = CFUDictionaryMerge(lBaseDictionaryRef,
                                 nullptr,
                                 nullptr,
                                 static_cast<CFMutableDictionaryRef>(nullptr));
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // There were no differences applied, so the expected key/value
    // pairs should be exactly those of the base.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kBaseKeys[0],
                                &kBaseValues[0],
                                kBaseKeyCount);

    lStatus = CFUDictionaryMerge(lBaseDictionaryRef,
                                 nullptr,
                                 nullptr,
                                 static_cast<CFMutableArrayRef>(nullptr));
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // There were no differences applied, so the expected key/value
    // pairs should be exactly those of the base.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kBaseKeys[0],
                                &kBaseValues[0],
                                kBaseKeyCount);

    // Test Teardown

    TestTeardown(lBaseDictionaryRef,
                 lAddedDictionaryRef,
                 lCommonDictionaryRef,
                 lRemovedDictionaryRef,
                 lRemovedKeysArrayRef);
}

void
TestCFUDictionaryMergeWithDifferences :: TestBaseWithAddedOnly(void)
{
    constexpr size_t       kBaseKeyCount                      = 2;
    const void *           kBaseKeys[kBaseKeyCount]           = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4")
    };
    const void *           kBaseValues[kBaseKeyCount]         = {
        CFSTR("Test Value 2"),
        CFSTR("Test Value 4")
    };
    CFMutableDictionaryRef lBaseDictionaryRef                 = nullptr;
    constexpr size_t       kAddedKeyCount                     = 2;
    const void *           kAddedKeys[kAddedKeyCount]         = {
        CFSTR("Test Key 1"),
        CFSTR("Test Key 3")
    };
    const void *           kAddedValues[kAddedKeyCount]       = {
        CFSTR("Test Value 1"),
        CFSTR("Test Value 3")
    };
    CFMutableDictionaryRef lAddedDictionaryRef                = nullptr;
    CFMutableDictionaryRef lCommonDictionaryRef               = nullptr;
    CFMutableDictionaryRef lRemovedDictionaryRef              = nullptr;
    CFMutableArrayRef      lRemovedKeysArrayRef               = nullptr;
    constexpr size_t       kExpectedKeyCount                  = 4;
    const void *           kExpectedKeys[kExpectedKeyCount]   = {
        CFSTR("Test Key 1"),
        CFSTR("Test Key 2"),
        CFSTR("Test Key 3"),
        CFSTR("Test Key 4")
    };
    const void *           kExpectedValues[kExpectedKeyCount] = {
        CFSTR("Test Value 1"),
        CFSTR("Test Value 2"),
        CFSTR("Test Value 3"),
        CFSTR("Test Value 4")
    };
    Boolean                lStatus;

    // Test Setup

    TestSetup(&kBaseKeys[0],
              &kBaseValues[0],
              kBaseKeyCount,
              lBaseDictionaryRef,
              &kAddedKeys[0],
              &kAddedValues[0],
              kAddedKeyCount,
              lAddedDictionaryRef,
              nullptr,
              nullptr,
              0,
              lCommonDictionaryRef,
              nullptr,
              nullptr,
              0,
              lRemovedDictionaryRef,
              nullptr,
              0,
              lRemovedKeysArrayRef);

    // C Bindings

    lStatus = CFUDictionaryMergeWithDifferences(lBaseDictionaryRef,
                                                lAddedDictionaryRef,
                                                nullptr,
                                                nullptr);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Only keys were added, so the expected key/value pairs should be
    // the union of the base and added keys.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kExpectedKeys[0],
                                &kExpectedValues[0],
                                kExpectedKeyCount);

    // Clear the base dictionary and reestablish the baseline pre-test
    // key/value pairs.

    CFDictionaryRemoveAllValues(lBaseDictionaryRef);

    TestDictionaryPopulateWithKeysAndValues(&kBaseKeys[0],
                                            &kBaseValues[0],
                                            kBaseKeyCount,
                                            lBaseDictionaryRef);

    lStatus = CFUDictionaryMergeWithDifferencesAndRemovedKeys(lBaseDictionaryRef,
                                                              lAddedDictionaryRef,
                                                              nullptr,
                                                              nullptr);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Only keys were added, so the expected key/value pairs should be
    // the union of the base and added keys.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kExpectedKeys[0],
                                &kExpectedValues[0],
                                kExpectedKeyCount);

    // C++ Bindings

    // Clear the base dictionary and reestablish the baseline pre-test
    // key/value pairs.

    CFDictionaryRemoveAllValues(lBaseDictionaryRef);

    TestDictionaryPopulateWithKeysAndValues(&kBaseKeys[0],
                                            &kBaseValues[0],
                                            kBaseKeyCount,
                                            lBaseDictionaryRef);

    lStatus = CFUDictionaryMerge(lBaseDictionaryRef,
                                 lAddedDictionaryRef,
                                 nullptr,
                                 static_cast<CFMutableDictionaryRef>(nullptr));
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Only keys were added, so the expected key/value pairs should be
    // the union of the base and added keys.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kExpectedKeys[0],
                                &kExpectedValues[0],
                                kExpectedKeyCount);

    // Clear the base dictionary and reestablish the baseline pre-test
    // key/value pairs.

    CFDictionaryRemoveAllValues(lBaseDictionaryRef);

    TestDictionaryPopulateWithKeysAndValues(&kBaseKeys[0],
                                            &kBaseValues[0],
                                            kBaseKeyCount,
                                            lBaseDictionaryRef);

    lStatus = CFUDictionaryMerge(lBaseDictionaryRef,
                                 lAddedDictionaryRef,
                                 nullptr,
                                 static_cast<CFMutableArrayRef>(nullptr));
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Only keys were added, so the expected key/value pairs should be
    // the union of the base and added keys.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kExpectedKeys[0],
                                &kExpectedValues[0],
                                kExpectedKeyCount);

    // Test Teardown

    TestTeardown(lBaseDictionaryRef,
                 lAddedDictionaryRef,
                 lCommonDictionaryRef,
                 lRemovedDictionaryRef,
                 lRemovedKeysArrayRef);
}

void
TestCFUDictionaryMergeWithDifferences :: TestBaseWithCommonOnly(void)
{
    constexpr size_t       kBaseKeyCount                      = 2;
    const void *           kBaseKeys[kBaseKeyCount]           = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4")
    };
    const void *           kBaseValues[kBaseKeyCount]         = {
        CFSTR("Test Value 2"),
        CFSTR("Test Value 4")
    };
    CFMutableDictionaryRef lBaseDictionaryRef                 = nullptr;
    CFMutableDictionaryRef lAddedDictionaryRef                = nullptr;
    constexpr size_t       kCommonKeyCount                    = 2;
    const void *           kCommonKeys[kCommonKeyCount]       = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4")
    };
    const void *           kCommonValues[kCommonKeyCount]     = {
        CFSTR("Test Value 2"),
        CFSTR("Test Value 4")
    };
    CFMutableDictionaryRef lCommonDictionaryRef               = nullptr;
    CFMutableDictionaryRef lRemovedDictionaryRef              = nullptr;
    CFMutableArrayRef      lRemovedKeysArrayRef               = nullptr;
    Boolean                lStatus;

    // Test Setup

    TestSetup(&kBaseKeys[0],
              &kBaseValues[0],
              kBaseKeyCount,
              lBaseDictionaryRef,
              nullptr,
              nullptr,
              0,
              lAddedDictionaryRef,
              &kCommonKeys[0],
              &kCommonValues[0],
              kCommonKeyCount,
              lCommonDictionaryRef,
              nullptr,
              nullptr,
              0,
              lRemovedDictionaryRef,
              nullptr,
              0,
              lRemovedKeysArrayRef);

    // C Bindings

    lStatus = CFUDictionaryMergeWithDifferences(lBaseDictionaryRef,
                                                nullptr,
                                                lCommonDictionaryRef,
                                                nullptr);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // No keys were added or removed and the common dictionary is
    // identical to the base dictionary, so the expected key/value
    // pairs should be exactly that of the base.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kBaseKeys[0],
                                &kBaseValues[0],
                                kBaseKeyCount);

    // Clear the base dictionary and reestablish the baseline pre-test
    // key/value pairs.

    CFDictionaryRemoveAllValues(lBaseDictionaryRef);

    TestDictionaryPopulateWithKeysAndValues(&kBaseKeys[0],
                                            &kBaseValues[0],
                                            kBaseKeyCount,
                                            lBaseDictionaryRef);

    lStatus = CFUDictionaryMergeWithDifferencesAndRemovedKeys(lBaseDictionaryRef,
                                                              nullptr,
                                                              lCommonDictionaryRef,
                                                              nullptr);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // No keys were added or removed and the common dictionary is
    // identical to the base dictionary, so the expected key/value
    // pairs should be exactly that of the base.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kBaseKeys[0],
                                &kBaseValues[0],
                                kBaseKeyCount);

    // C++ Bindings

    // Clear the base dictionary and reestablish the baseline pre-test
    // key/value pairs.

    CFDictionaryRemoveAllValues(lBaseDictionaryRef);

    TestDictionaryPopulateWithKeysAndValues(&kBaseKeys[0],
                                            &kBaseValues[0],
                                            kBaseKeyCount,
                                            lBaseDictionaryRef);

    lStatus = CFUDictionaryMerge(lBaseDictionaryRef,
                                 nullptr,
                                 lCommonDictionaryRef,
                                 static_cast<CFMutableDictionaryRef>(nullptr));
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // No keys were added or removed and the common dictionary is
    // identical to the base dictionary, so the expected key/value
    // pairs should be exactly that of the base.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kBaseKeys[0],
                                &kBaseValues[0],
                                kBaseKeyCount);

    // Clear the base dictionary and reestablish the baseline pre-test
    // key/value pairs.

    CFDictionaryRemoveAllValues(lBaseDictionaryRef);

    TestDictionaryPopulateWithKeysAndValues(&kBaseKeys[0],
                                            &kBaseValues[0],
                                            kBaseKeyCount,
                                            lBaseDictionaryRef);

    lStatus = CFUDictionaryMerge(lBaseDictionaryRef,
                                 nullptr,
                                 lCommonDictionaryRef,
                                 static_cast<CFMutableArrayRef>(nullptr));
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // No keys were added or removed and the common dictionary is
    // identical to the base dictionary, so the expected key/value
    // pairs should be exactly that of the base.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kBaseKeys[0],
                                &kBaseValues[0],
                                kBaseKeyCount);

    // Test Teardown

    TestTeardown(lBaseDictionaryRef,
                 lAddedDictionaryRef,
                 lCommonDictionaryRef,
                 lRemovedDictionaryRef,
                 lRemovedKeysArrayRef);
}

void
TestCFUDictionaryMergeWithDifferences :: TestBaseWithRemovedOnly(void)
{
    constexpr size_t       kBaseKeyCount                      = 2;
    const void *           kBaseKeys[kBaseKeyCount]           = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4")
    };
    const void *           kBaseValues[kBaseKeyCount]         = {
        CFSTR("Test Value 2"),
        CFSTR("Test Value 4")
    };
    CFMutableDictionaryRef lBaseDictionaryRef                 = nullptr;
    CFMutableDictionaryRef lAddedDictionaryRef                = nullptr;
    CFMutableDictionaryRef lCommonDictionaryRef               = nullptr;
    constexpr size_t       kRemovedKeyCount                   = 2;
    const void *           kRemovedKeys[kRemovedKeyCount]     = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4")
    };
    const void *           kRemovedValues[kRemovedKeyCount]   = {
        CFSTR("Test Value 2"),
        CFSTR("Test Value 4")
    };
    CFMutableDictionaryRef lRemovedDictionaryRef              = nullptr;
    CFMutableArrayRef      lRemovedKeysArrayRef               = nullptr;
    Boolean                lStatus;

    // Test Setup

    TestSetup(&kBaseKeys[0],
              &kBaseValues[0],
              kBaseKeyCount,
              lBaseDictionaryRef,
              nullptr,
              nullptr,
              0,
              lAddedDictionaryRef,
              nullptr,
              nullptr,
              0,
              lCommonDictionaryRef,
              &kRemovedKeys[0],
              &kRemovedValues[0],
              kRemovedKeyCount,
              lRemovedDictionaryRef,
              &kRemovedKeys[0],
              kRemovedKeyCount,
              lRemovedKeysArrayRef);

    // C Bindings

    lStatus = CFUDictionaryMergeWithDifferences(lBaseDictionaryRef,
                                                nullptr,
                                                nullptr,
                                                lRemovedDictionaryRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Only keys were removed, so the expected key/value pairs should
    // be zero.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                nullptr,
                                nullptr,
                                0);

    // Clear the base dictionary and reestablish the baseline pre-test
    // key/value pairs.

    CFDictionaryRemoveAllValues(lBaseDictionaryRef);

    TestDictionaryPopulateWithKeysAndValues(&kBaseKeys[0],
                                            &kBaseValues[0],
                                            kBaseKeyCount,
                                            lBaseDictionaryRef);

    lStatus = CFUDictionaryMergeWithDifferencesAndRemovedKeys(lBaseDictionaryRef,
                                                              nullptr,
                                                              nullptr,
                                                              lRemovedKeysArrayRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Only keys were removed, so the expected key/value pairs should
    // be zero.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                nullptr,
                                nullptr,
                                0);

    // C++ Bindings

    // Clear the base dictionary and reestablish the baseline pre-test
    // key/value pairs.

    CFDictionaryRemoveAllValues(lBaseDictionaryRef);

    TestDictionaryPopulateWithKeysAndValues(&kBaseKeys[0],
                                            &kBaseValues[0],
                                            kBaseKeyCount,
                                            lBaseDictionaryRef);

    lStatus = CFUDictionaryMerge(lBaseDictionaryRef,
                                 nullptr,
                                 nullptr,
                                 lRemovedDictionaryRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Only keys were removed, so the expected key/value pairs should
    // be zero.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                nullptr,
                                nullptr,
                                0);

    // Clear the base dictionary and reestablish the baseline pre-test
    // key/value pairs.

    CFDictionaryRemoveAllValues(lBaseDictionaryRef);

    TestDictionaryPopulateWithKeysAndValues(&kBaseKeys[0],
                                            &kBaseValues[0],
                                            kBaseKeyCount,
                                            lBaseDictionaryRef);

    lStatus = CFUDictionaryMerge(lBaseDictionaryRef,
                                 nullptr,
                                 nullptr,
                                 lRemovedKeysArrayRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Only keys were removed, so the expected key/value pairs should
    // be zero.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                nullptr,
                                nullptr,
                                0);

    // Test Teardown

    TestTeardown(lBaseDictionaryRef,
                 lAddedDictionaryRef,
                 lCommonDictionaryRef,
                 lRemovedDictionaryRef,
                 lRemovedKeysArrayRef);
}

void
TestCFUDictionaryMergeWithDifferences :: TestBaseWithAddedAndCommonOnly(void)
{
    constexpr size_t       kBaseKeyCount                      = 2;
    const void *           kBaseKeys[kBaseKeyCount]           = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4")
    };
    const void *           kBaseValues[kBaseKeyCount]         = {
        CFSTR("Test Value 2"),
        CFSTR("Test Value 4")
    };
    CFMutableDictionaryRef lBaseDictionaryRef                 = nullptr;
    constexpr size_t       kAddedKeyCount                     = 2;
    const void *           kAddedKeys[kAddedKeyCount]         = {
        CFSTR("Test Key 1"),
        CFSTR("Test Key 3")
    };
    const void *           kAddedValues[kAddedKeyCount]       = {
        CFSTR("Test Value 1"),
        CFSTR("Test Value 3")
    };
    CFMutableDictionaryRef lAddedDictionaryRef                = nullptr;
    constexpr size_t       kCommonKeyCount                    = 2;
    const void *           kCommonKeys[kCommonKeyCount]       = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4")
    };
    const void *           kCommonValues[kCommonKeyCount]     = {
        CFSTR("Test Value 2"),
        CFSTR("Test Value 4")
    };
    CFMutableDictionaryRef lCommonDictionaryRef               = nullptr;
    CFMutableDictionaryRef lRemovedDictionaryRef              = nullptr;
    CFMutableArrayRef      lRemovedKeysArrayRef               = nullptr;
    constexpr size_t       kExpectedKeyCount                  = 4;
    const void *           kExpectedKeys[kExpectedKeyCount]   = {
        CFSTR("Test Key 1"),
        CFSTR("Test Key 2"),
        CFSTR("Test Key 3"),
        CFSTR("Test Key 4")
    };
    const void *           kExpectedValues[kExpectedKeyCount] = {
        CFSTR("Test Value 1"),
        CFSTR("Test Value 2"),
        CFSTR("Test Value 3"),
        CFSTR("Test Value 4")
    };
    Boolean                lStatus;

    // Test Setup

    TestSetup(&kBaseKeys[0],
              &kBaseValues[0],
              kBaseKeyCount,
              lBaseDictionaryRef,
              &kAddedKeys[0],
              &kAddedValues[0],
              kAddedKeyCount,
              lAddedDictionaryRef,
              &kCommonKeys[0],
              &kCommonValues[0],
              kCommonKeyCount,
              lCommonDictionaryRef,
              nullptr,
              nullptr,
              0,
              lRemovedDictionaryRef,
              nullptr,
              0,
              lRemovedKeysArrayRef);

    // C Bindings

    lStatus = CFUDictionaryMergeWithDifferences(lBaseDictionaryRef,
                                                lAddedDictionaryRef,
                                                lCommonDictionaryRef,
                                                nullptr);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Keys were added and the common dictionary is identical to the
    // base dictionary, so the expected key/value pairs should be the
    // union of the base and added keys.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kExpectedKeys[0],
                                &kExpectedValues[0],
                                kExpectedKeyCount);

    // Clear the base dictionary and reestablish the baseline pre-test
    // key/value pairs.

    CFDictionaryRemoveAllValues(lBaseDictionaryRef);

    TestDictionaryPopulateWithKeysAndValues(&kBaseKeys[0],
                                            &kBaseValues[0],
                                            kBaseKeyCount,
                                            lBaseDictionaryRef);

    lStatus = CFUDictionaryMergeWithDifferencesAndRemovedKeys(lBaseDictionaryRef,
                                                              lAddedDictionaryRef,
                                                              lCommonDictionaryRef,
                                                              nullptr);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Keys were added and the common dictionary is identical to the
    // base dictionary, so the expected key/value pairs should be the
    // union of the base and added keys.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kExpectedKeys[0],
                                &kExpectedValues[0],
                                kExpectedKeyCount);

    // C++ Bindings

    // Clear the base dictionary and reestablish the baseline pre-test
    // key/value pairs.

    CFDictionaryRemoveAllValues(lBaseDictionaryRef);

    TestDictionaryPopulateWithKeysAndValues(&kBaseKeys[0],
                                            &kBaseValues[0],
                                            kBaseKeyCount,
                                            lBaseDictionaryRef);

    lStatus = CFUDictionaryMerge(lBaseDictionaryRef,
                                 lAddedDictionaryRef,
                                 lCommonDictionaryRef,
                                 static_cast<CFMutableDictionaryRef>(nullptr));
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Keys were added and the common dictionary is identical to the
    // base dictionary, so the expected key/value pairs should be the
    // union of the base and added keys.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kExpectedKeys[0],
                                &kExpectedValues[0],
                                kExpectedKeyCount);

    // Clear the base dictionary and reestablish the baseline pre-test
    // key/value pairs.

    CFDictionaryRemoveAllValues(lBaseDictionaryRef);

    TestDictionaryPopulateWithKeysAndValues(&kBaseKeys[0],
                                            &kBaseValues[0],
                                            kBaseKeyCount,
                                            lBaseDictionaryRef);

    lStatus = CFUDictionaryMerge(lBaseDictionaryRef,
                                 lAddedDictionaryRef,
                                 lCommonDictionaryRef,
                                 static_cast<CFMutableArrayRef>(nullptr));
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Keys were added and the common dictionary is identical to the
    // base dictionary, so the expected key/value pairs should be the
    // union of the base and added keys.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kExpectedKeys[0],
                                &kExpectedValues[0],
                                kExpectedKeyCount);

    // Test Teardown

    TestTeardown(lBaseDictionaryRef,
                 lAddedDictionaryRef,
                 lCommonDictionaryRef,
                 lRemovedDictionaryRef,
                 lRemovedKeysArrayRef);
}

void
TestCFUDictionaryMergeWithDifferences :: TestBaseWithAddedAndRemovedOnly(void)
{
    constexpr size_t       kBaseKeyCount                      = 2;
    const void *           kBaseKeys[kBaseKeyCount]           = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4")
    };
    const void *           kBaseValues[kBaseKeyCount]         = {
        CFSTR("Test Value 2"),
        CFSTR("Test Value 4")
    };
    CFMutableDictionaryRef lBaseDictionaryRef                 = nullptr;
    constexpr size_t       kAddedKeyCount                     = 2;
    const void *           kAddedKeys[kAddedKeyCount]         = {
        CFSTR("Test Key 1"),
        CFSTR("Test Key 3")
    };
    const void *           kAddedValues[kAddedKeyCount]       = {
        CFSTR("Test Value 1"),
        CFSTR("Test Value 3")
    };
    CFMutableDictionaryRef lAddedDictionaryRef                = nullptr;
    CFMutableDictionaryRef lCommonDictionaryRef               = nullptr;
    constexpr size_t       kRemovedKeyCount                   = 2;
    const void *           kRemovedKeys[kRemovedKeyCount]     = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4")
    };
    const void *           kRemovedValues[kRemovedKeyCount]   = {
        CFSTR("Test Value 2"),
        CFSTR("Test Value 4")
    };
    CFMutableDictionaryRef lRemovedDictionaryRef              = nullptr;
    CFMutableArrayRef      lRemovedKeysArrayRef               = nullptr;
    Boolean                lStatus;

    // Test Setup

    TestSetup(&kBaseKeys[0],
              &kBaseValues[0],
              kBaseKeyCount,
              lBaseDictionaryRef,
              &kAddedKeys[0],
              &kAddedValues[0],
              kAddedKeyCount,
              lAddedDictionaryRef,
              nullptr,
              nullptr,
              0,
              lCommonDictionaryRef,
              &kRemovedKeys[0],
              &kRemovedValues[0],
              kRemovedKeyCount,
              lRemovedDictionaryRef,
              &kRemovedKeys[0],
              kRemovedKeyCount,
              lRemovedKeysArrayRef);

    // C Bindings

    lStatus = CFUDictionaryMergeWithDifferences(lBaseDictionaryRef,
                                                lAddedDictionaryRef,
                                                nullptr,
                                                lRemovedDictionaryRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Keys were both added and removed, so the expected key/value
    // pairs should be the the added key/value pairs.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kAddedKeys[0],
                                &kAddedValues[0],
                                kAddedKeyCount);

    // Clear the base dictionary and reestablish the baseline pre-test
    // key/value pairs.

    CFDictionaryRemoveAllValues(lBaseDictionaryRef);

    TestDictionaryPopulateWithKeysAndValues(&kBaseKeys[0],
                                            &kBaseValues[0],
                                            kBaseKeyCount,
                                            lBaseDictionaryRef);

    lStatus = CFUDictionaryMergeWithDifferencesAndRemovedKeys(lBaseDictionaryRef,
                                                              lAddedDictionaryRef,
                                                              nullptr,
                                                              lRemovedKeysArrayRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Keys were both added and removed, so the expected key/value
    // pairs should be the the added key/value pairs.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kAddedKeys[0],
                                &kAddedValues[0],
                                kAddedKeyCount);

    // C++ Bindings

    // Clear the base dictionary and reestablish the baseline pre-test
    // key/value pairs.

    CFDictionaryRemoveAllValues(lBaseDictionaryRef);

    TestDictionaryPopulateWithKeysAndValues(&kBaseKeys[0],
                                            &kBaseValues[0],
                                            kBaseKeyCount,
                                            lBaseDictionaryRef);

    lStatus = CFUDictionaryMerge(lBaseDictionaryRef,
                                 lAddedDictionaryRef,
                                 nullptr,
                                 lRemovedDictionaryRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Keys were both added and removed, so the expected key/value
    // pairs should be the the added key/value pairs.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kAddedKeys[0],
                                &kAddedValues[0],
                                kAddedKeyCount);

    // Clear the base dictionary and reestablish the baseline pre-test
    // key/value pairs.

    CFDictionaryRemoveAllValues(lBaseDictionaryRef);

    TestDictionaryPopulateWithKeysAndValues(&kBaseKeys[0],
                                            &kBaseValues[0],
                                            kBaseKeyCount,
                                            lBaseDictionaryRef);

    lStatus = CFUDictionaryMerge(lBaseDictionaryRef,
                                 lAddedDictionaryRef,
                                 nullptr,
                                 lRemovedKeysArrayRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Keys were both added and removed, so the expected key/value
    // pairs should be the the added key/value pairs.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kAddedKeys[0],
                                &kAddedValues[0],
                                kAddedKeyCount);

    // Test Teardown

    TestTeardown(lBaseDictionaryRef,
                 lAddedDictionaryRef,
                 lCommonDictionaryRef,
                 lRemovedDictionaryRef,
                 lRemovedKeysArrayRef);
}

void
TestCFUDictionaryMergeWithDifferences :: TestBaseWithCommonAndRemovedOnly(void)
{
    constexpr size_t       kBaseKeyCount                      = 4;
    const void *           kBaseKeys[kBaseKeyCount]           = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4"),
        CFSTR("Test Key 5"),
        CFSTR("Test Key 6")
    };
    const void *           kBaseValues[kBaseKeyCount]         = {
        CFSTR("Test Value 2"),
        CFSTR("Test Value 4"),
        CFSTR("Test Value 5"),
        CFSTR("Test Value 6")
    };
    CFMutableDictionaryRef lBaseDictionaryRef                 = nullptr;
    CFMutableDictionaryRef lAddedDictionaryRef                = nullptr;
    constexpr size_t       kCommonKeyCount                    = 2;
    const void *           kCommonKeys[kCommonKeyCount]       = {
        CFSTR("Test Key 5"),
        CFSTR("Test Key 6")
    };
    const void *           kCommonValues[kCommonKeyCount]     = {
        CFSTR("Test Value 5"),
        CFSTR("Test Value 6")
    };
    CFMutableDictionaryRef lCommonDictionaryRef               = nullptr;
    constexpr size_t       kRemovedKeyCount                   = 2;
    const void *           kRemovedKeys[kRemovedKeyCount]     = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4")
    };
    const void *           kRemovedValues[kRemovedKeyCount]   = {
        CFSTR("Test Value 2"),
        CFSTR("Test Value 4")
    };
    CFMutableDictionaryRef lRemovedDictionaryRef              = nullptr;
    CFMutableArrayRef      lRemovedKeysArrayRef               = nullptr;
    Boolean                lStatus;

    // Test Setup

    TestSetup(&kBaseKeys[0],
              &kBaseValues[0],
              kBaseKeyCount,
              lBaseDictionaryRef,
              nullptr,
              nullptr,
              0,
              lAddedDictionaryRef,
              &kCommonKeys[0],
              &kCommonValues[0],
              kCommonKeyCount,
              lCommonDictionaryRef,
              &kRemovedKeys[0],
              &kRemovedValues[0],
              kRemovedKeyCount,
              lRemovedDictionaryRef,
              &kRemovedKeys[0],
              kRemovedKeyCount,
              lRemovedKeysArrayRef);

    // C Bindings

    lStatus = CFUDictionaryMergeWithDifferences(lBaseDictionaryRef,
                                                nullptr,
                                                lCommonDictionaryRef,
                                                lRemovedDictionaryRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Keys were removed and the common dictionary is identical to the
    // base dictionary, so the expected key/value pairs should
    // be exaclty the common key/value pairs.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kCommonKeys[0],
                                &kCommonValues[0],
                                kCommonKeyCount);

    // Clear the base dictionary and reestablish the baseline pre-test
    // key/value pairs.

    CFDictionaryRemoveAllValues(lBaseDictionaryRef);

    TestDictionaryPopulateWithKeysAndValues(&kBaseKeys[0],
                                            &kBaseValues[0],
                                            kBaseKeyCount,
                                            lBaseDictionaryRef);

    lStatus = CFUDictionaryMergeWithDifferencesAndRemovedKeys(lBaseDictionaryRef,
                                                              nullptr,
                                                              lCommonDictionaryRef,
                                                              lRemovedKeysArrayRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Keys were removed and the common dictionary is identical to the
    // base dictionary, so the expected key/value pairs should
    // be exaclty the common key/value pairs.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kCommonKeys[0],
                                &kCommonValues[0],
                                kCommonKeyCount);

    // C++ Bindings

    // Clear the base dictionary and reestablish the baseline pre-test
    // key/value pairs.

    CFDictionaryRemoveAllValues(lBaseDictionaryRef);

    TestDictionaryPopulateWithKeysAndValues(&kBaseKeys[0],
                                            &kBaseValues[0],
                                            kBaseKeyCount,
                                            lBaseDictionaryRef);

    lStatus = CFUDictionaryMerge(lBaseDictionaryRef,
                                 nullptr,
                                 lCommonDictionaryRef,
                                 lRemovedDictionaryRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Keys were removed and the common dictionary is identical to the
    // base dictionary, so the expected key/value pairs should
    // be exaclty the common key/value pairs.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kCommonKeys[0],
                                &kCommonValues[0],
                                kCommonKeyCount);

    // Clear the base dictionary and reestablish the baseline pre-test
    // key/value pairs.

    CFDictionaryRemoveAllValues(lBaseDictionaryRef);

    TestDictionaryPopulateWithKeysAndValues(&kBaseKeys[0],
                                            &kBaseValues[0],
                                            kBaseKeyCount,
                                            lBaseDictionaryRef);

    lStatus = CFUDictionaryMerge(lBaseDictionaryRef,
                                 nullptr,
                                 lCommonDictionaryRef,
                                 lRemovedKeysArrayRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Keys were removed and the common dictionary is identical to the
    // base dictionary, so the expected key/value pairs should
    // be exaclty the common key/value pairs.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kCommonKeys[0],
                                &kCommonValues[0],
                                kCommonKeyCount);

    // Test Teardown

    TestTeardown(lBaseDictionaryRef,
                 lAddedDictionaryRef,
                 lCommonDictionaryRef,
                 lRemovedDictionaryRef,
                 lRemovedKeysArrayRef);
}

void
TestCFUDictionaryMergeWithDifferences :: TestBaseWithCommonDifferentValues(void)
{
    constexpr size_t       kBaseKeyCount                      = 4;
    const void *           kBaseKeys[kBaseKeyCount]           = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4"),
        CFSTR("Test Key 5"),
        CFSTR("Test Key 6")
    };
    const void *           kBaseValues[kBaseKeyCount]         = {
        CFSTR("Test Value 2"),
        CFSTR("Test Value 4"),
        CFSTR("Test Value 5"),
        CFSTR("Test Value 6")
    };
    CFMutableDictionaryRef lBaseDictionaryRef                 = nullptr;
    constexpr size_t       kAddedKeyCount                     = 2;
    const void *           kAddedKeys[kAddedKeyCount]         = {
        CFSTR("Test Key 1"),
        CFSTR("Test Key 3")
    };
    const void *           kAddedValues[kAddedKeyCount]       = {
        CFSTR("Test Value 1"),
        CFSTR("Test Value 3")
    };
    CFMutableDictionaryRef lAddedDictionaryRef                = nullptr;
    constexpr size_t       kCommonKeyCount                    = 2;
    const void *           kCommonKeys[kCommonKeyCount]       = {
        CFSTR("Test Key 5"),
        CFSTR("Test Key 6")
    };
    const void *           kCommonValues[kCommonKeyCount]     = {
        CFSTR("Test Value 7"),
        CFSTR("Test Value 8")
    };
    CFMutableDictionaryRef lCommonDictionaryRef               = nullptr;
    constexpr size_t       kRemovedKeyCount                   = 2;
    const void *           kRemovedKeys[kRemovedKeyCount]     = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4")
    };
    const void *           kRemovedValues[kRemovedKeyCount]   = {
        CFSTR("Test Value 2"),
        CFSTR("Test Value 4")
    };
    CFMutableDictionaryRef lRemovedDictionaryRef              = nullptr;
    CFMutableArrayRef      lRemovedKeysArrayRef               = nullptr;
    constexpr size_t       kExpectedKeyCount                  = 4;
    const void *           kExpectedKeys[kExpectedKeyCount]   = {
        CFSTR("Test Key 1"),
        CFSTR("Test Key 3"),
        CFSTR("Test Key 5"),
        CFSTR("Test Key 6")
    };
    const void *           kExpectedValues[kExpectedKeyCount] = {
        CFSTR("Test Value 1"),
        CFSTR("Test Value 3"),
        CFSTR("Test Value 7"),
        CFSTR("Test Value 8")
    };
    Boolean                lStatus;

    // Test Setup

    TestSetup(&kBaseKeys[0],
              &kBaseValues[0],
              kBaseKeyCount,
              lBaseDictionaryRef,
              &kAddedKeys[0],
              &kAddedValues[0],
              kAddedKeyCount,
              lAddedDictionaryRef,
              &kCommonKeys[0],
              &kCommonValues[0],
              kCommonKeyCount,
              lCommonDictionaryRef,
              &kRemovedKeys[0],
              &kRemovedValues[0],
              kRemovedKeyCount,
              lRemovedDictionaryRef,
              &kRemovedKeys[0],
              kRemovedKeyCount,
              lRemovedKeysArrayRef);

    // C Bindings

    lStatus = CFUDictionaryMergeWithDifferences(lBaseDictionaryRef,
                                                lAddedDictionaryRef,
                                                lCommonDictionaryRef,
                                                lRemovedDictionaryRef);
    CPPUNIT_ASSERT(lStatus == true);

    CFShow(lBaseDictionaryRef);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Keys were added, removed, and updated through common key/value
    // pair conflicts, so the expected key/value pairs should be
    // exaclty the those added plus those common key/value pairs
    // changed.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kExpectedKeys[0],
                                &kExpectedValues[0],
                                kExpectedKeyCount);

    // Clear the base dictionary and reestablish the baseline pre-test
    // key/value pairs.

    CFDictionaryRemoveAllValues(lBaseDictionaryRef);

    TestDictionaryPopulateWithKeysAndValues(&kBaseKeys[0],
                                            &kBaseValues[0],
                                            kBaseKeyCount,
                                            lBaseDictionaryRef);

    lStatus = CFUDictionaryMergeWithDifferencesAndRemovedKeys(lBaseDictionaryRef,
                                                              lAddedDictionaryRef,
                                                              lCommonDictionaryRef,
                                                              lRemovedKeysArrayRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Keys were added, removed, and updated through common key/value
    // pair conflicts, so the expected key/value pairs should be
    // exaclty the those added plus those common key/value pairs
    // changed.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kExpectedKeys[0],
                                &kExpectedValues[0],
                                kExpectedKeyCount);

    // C++ Bindings

    // Clear the base dictionary and reestablish the baseline pre-test
    // key/value pairs.

    CFDictionaryRemoveAllValues(lBaseDictionaryRef);

    TestDictionaryPopulateWithKeysAndValues(&kBaseKeys[0],
                                            &kBaseValues[0],
                                            kBaseKeyCount,
                                            lBaseDictionaryRef);

    lStatus = CFUDictionaryMerge(lBaseDictionaryRef,
                                 lAddedDictionaryRef,
                                 lCommonDictionaryRef,
                                 lRemovedDictionaryRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Keys were added, removed, and updated through common key/value
    // pair conflicts, so the expected key/value pairs should be
    // exaclty the those added plus those common key/value pairs
    // changed.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kExpectedKeys[0],
                                &kExpectedValues[0],
                                kExpectedKeyCount);

    // Clear the base dictionary and reestablish the baseline pre-test
    // key/value pairs.

    CFDictionaryRemoveAllValues(lBaseDictionaryRef);

    TestDictionaryPopulateWithKeysAndValues(&kBaseKeys[0],
                                            &kBaseValues[0],
                                            kBaseKeyCount,
                                            lBaseDictionaryRef);

    lStatus = CFUDictionaryMerge(lBaseDictionaryRef,
                                 lAddedDictionaryRef,
                                 lCommonDictionaryRef,
                                 lRemovedKeysArrayRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Validate the base dictionary keys and values are as expected
    // after the merge.
    //
    // Keys were added, removed, and updated through common key/value
    // pair conflicts, so the expected key/value pairs should be
    // exaclty the those added plus those common key/value pairs
    // changed.

    TestDictionaryKeysAndValues(lBaseDictionaryRef,
                                &kExpectedKeys[0],
                                &kExpectedValues[0],
                                kExpectedKeyCount);

    // Test Teardown

    TestTeardown(lBaseDictionaryRef,
                 lAddedDictionaryRef,
                 lCommonDictionaryRef,
                 lRemovedDictionaryRef,
                 lRemovedKeysArrayRef);
}

void
TestCFUDictionaryMergeWithDifferences :: TestArrayCreateWithValues(const void **inFirstValue,
                                                                   const size_t &inCount,
                                                                   CFMutableArrayRef &outArray)
{
    CPPUNIT_ASSERT(outArray == nullptr);

    outArray = CFArrayCreateMutable(kCFAllocatorDefault,
                                    0,
                                    &kCFTypeArrayCallBacks);
    CPPUNIT_ASSERT(outArray != nullptr);

    for (size_t i = 0; i < inCount; i++)
    {
        CFArrayInsertValueAtIndex(outArray, static_cast<CFIndex>(i), inFirstValue[i]);
    }
}

void
TestCFUDictionaryMergeWithDifferences :: TestDictionaryPopulateWithKeysAndValues(const void **inFirstKey,
                                                                                 const void **inFirstValue,
                                                                                 const size_t &inCount,
                                                                                 CFMutableDictionaryRef &inOutDictionary)
{
    CPPUNIT_ASSERT(inOutDictionary != nullptr);

    for (size_t i = 0; i < inCount; i++)
    {
        CFDictionaryAddValue(inOutDictionary, inFirstKey[i], inFirstValue[i]);
    }
}

void
TestCFUDictionaryMergeWithDifferences :: TestDictionaryCreateWithKeysAndValues(const void **inFirstKey,
                                                                               const void **inFirstValue,
                                                                               const size_t &inCount,
                                                                               CFMutableDictionaryRef &outDictionary)
{
    CPPUNIT_ASSERT(outDictionary == nullptr);

    outDictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                              0,
                                              &kCFTypeDictionaryKeyCallBacks,
                                              &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(outDictionary != nullptr);

    TestDictionaryPopulateWithKeysAndValues(inFirstKey,
                                            inFirstValue,
                                            inCount,
                                            outDictionary);
}

void
TestCFUDictionaryMergeWithDifferences :: TestSetup(const void **inFirstBaseDictionaryKey,
                                                   const void **inFirstBaseDictionaryValue,
                                                   const size_t &inBaseDictionaryCount,
                                                   CFMutableDictionaryRef &outBaseDictionaryRef,
                                                   const void **inFirstAddedDictionaryKey,
                                                   const void **inFirstAddedDictionaryValue,
                                                   const size_t &inAddedDictionaryCount,
                                                   CFMutableDictionaryRef &outAddedDictionaryRef,
                                                   const void **inFirstCommonDictionaryKey,
                                                   const void **inFirstCommonDictionaryValue,
                                                   const size_t &inCommonDictionaryCount,
                                                   CFMutableDictionaryRef &outCommonDictionaryRef,
                                                   const void **inFirstRemovedDictionaryKey,
                                                   const void **inFirstRemovedDictionaryValue,
                                                   const size_t &inRemovedDictionaryCount,
                                                   CFMutableDictionaryRef &outRemovedDictionaryRef,
                                                   const void **inFirstRemovedKeysArrayKey,
                                                   const size_t &inRemovedKeysArrayCount,
                                                   CFMutableArrayRef &outRemovedKeysArrayRef)
{
    TestDictionaryCreateWithKeysAndValues(inFirstBaseDictionaryKey,
                                          inFirstBaseDictionaryValue,
                                          inBaseDictionaryCount,
                                          outBaseDictionaryRef);

    TestDictionaryCreateWithKeysAndValues(inFirstAddedDictionaryKey,
                                          inFirstAddedDictionaryValue,
                                          inAddedDictionaryCount,
                                          outAddedDictionaryRef);

    TestDictionaryCreateWithKeysAndValues(inFirstCommonDictionaryKey,
                                          inFirstCommonDictionaryValue,
                                          inCommonDictionaryCount,
                                          outCommonDictionaryRef);

    TestDictionaryCreateWithKeysAndValues(inFirstRemovedDictionaryKey,
                                          inFirstRemovedDictionaryValue,
                                          inRemovedDictionaryCount,
                                          outRemovedDictionaryRef);

    TestArrayCreateWithValues(inFirstRemovedKeysArrayKey,
                              inRemovedKeysArrayCount,
                              outRemovedKeysArrayRef);
}

void
TestCFUDictionaryMergeWithDifferences :: TestDictionaryKeysAndValues(CFDictionaryRef inDictionary,
                                                           const void **inExpectedFirstKey,
                                                           const void **inExpectedFirstValue,
                                                           const size_t &inExpectedCount)
{
    CFIndex lDictionaryCount;

    CPPUNIT_ASSERT(inDictionary != nullptr);

    lDictionaryCount = CFDictionaryGetCount(inDictionary);
    CPPUNIT_ASSERT(static_cast<size_t>(lDictionaryCount) == inExpectedCount);

    for (size_t i = 0; i < inExpectedCount; i++)
    {
        CFStringRef            lStringValue;
        CFComparisonResult     lComparisonResult;

        lStringValue = reinterpret_cast<CFStringRef>(
            CFDictionaryGetValue(inDictionary, inExpectedFirstKey[i]));
        CPPUNIT_ASSERT(lStringValue != NULL);

        lComparisonResult =
            CFStringCompare(lStringValue,
                            reinterpret_cast<CFStringRef>(inExpectedFirstValue[i]),
                            0);
        CPPUNIT_ASSERT(lComparisonResult == kCFCompareEqualTo);
    }
}

void
TestCFUDictionaryMergeWithDifferences :: TestTeardown(CFMutableDictionaryRef &inBaseDictionaryRef,
                                                      CFMutableDictionaryRef &inAddedDictionaryRef,
                                                      CFMutableDictionaryRef &inCommonDictionaryRef,
                                                      CFMutableDictionaryRef &inRemovedDictionaryRef,
                                                      CFMutableArrayRef &inRemovedKeysArrayRef)
{
    CPPUNIT_ASSERT(inBaseDictionaryRef    != nullptr);
    CPPUNIT_ASSERT(inAddedDictionaryRef   != nullptr);
    CPPUNIT_ASSERT(inCommonDictionaryRef  != nullptr);
    CPPUNIT_ASSERT(inRemovedDictionaryRef != nullptr);
    CPPUNIT_ASSERT(inRemovedKeysArrayRef  != nullptr);

    CFRelease(inBaseDictionaryRef);
    CFRelease(inAddedDictionaryRef);
    CFRelease(inCommonDictionaryRef);
    CFRelease(inRemovedDictionaryRef);
    CFRelease(inRemovedKeysArrayRef);
}
