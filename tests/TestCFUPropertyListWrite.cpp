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
 *      This file implements a unit test for
 *      CFUPropertyListWriteToFile and CFUPropertyListWriteToURL.
 */

#include <CFUtilities/CFUtilities.hpp>

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFUPropertyListWrite :
    public CppUnit::TestFixture
{
public:
    ~TestCFUPropertyListWrite(void) = default;

protected:
    TestCFUPropertyListWrite(void) = default;

    void SetUp(void);
    void TearDown(void);

    void SetUpDictionary(void);
    void SetUpTemporaryPath(void);

private:
    static void NameTemporary(char * aPathBuffer, const char * aPathPattern);

protected:
    char            mValidPropertyListTemporaryPath[PATH_MAX];
    CFDictionaryRef mDictionaryRef;
};

class TestCFUPropertyListWriteToFile :
    public TestCFUPropertyListWrite
{
    CPPUNIT_TEST_SUITE(TestCFUPropertyListWriteToFile);
    CPPUNIT_TEST(TestNull);
    CPPUNIT_TEST(TestInvalidCStringFormat);
    CPPUNIT_TEST(TestNonNullCStringReadableXML);
    CPPUNIT_TEST(TestNonNullCStringReadableBinary);
    CPPUNIT_TEST(TestNonNullCStringWritableXML);
    CPPUNIT_TEST(TestNonNullCStringWritableBinary);
    CPPUNIT_TEST(TestInvalidCFStringFormat);
    CPPUNIT_TEST(TestNonNullCFStringXML);
    CPPUNIT_TEST(TestNonNullCFStringBinary);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestNull(void);
    void TestInvalidCStringFormat(void);
    void TestNonNullCStringReadableXML(void);
    void TestNonNullCStringReadableBinary(void);
    void TestNonNullCStringWritableXML(void);
    void TestNonNullCStringWritableBinary(void);
    void TestInvalidCFStringFormat(void);
    void TestNonNullCFStringXML(void);
    void TestNonNullCFStringBinary(void);

    void setUp(void);
    void tearDown(void);

private:
    void TestNonNullCString(const bool &                 inWritable,
                            const CFPropertyListFormat & inFormat);
    void TestNonNullCFString(const CFPropertyListFormat & inFormat);
};

class TestCFUPropertyListWriteToURL :
    public TestCFUPropertyListWrite
{
    CPPUNIT_TEST_SUITE(TestCFUPropertyListWriteToURL);
    CPPUNIT_TEST(TestNull);
    CPPUNIT_TEST(TestInvalidFormat);
    CPPUNIT_TEST(TestNonNullXML);
    CPPUNIT_TEST(TestNonNullBinary);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestNull(void);
    void TestInvalidFormat(void);
    void TestNonNullXML(void);
    void TestNonNullBinary(void);

    void setUp(void);
    void tearDown(void);

private:
    void TestNonNull(const CFPropertyListFormat & inFormat);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUPropertyListWriteToFile);
CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUPropertyListWriteToURL);

void
TestCFUPropertyListWrite :: SetUp(void)
{
    SetUpDictionary();
    SetUpTemporaryPath();
}

void
TestCFUPropertyListWrite :: SetUpDictionary(void)
{
    const int              lInteger           = 42;
    const float            lReal              = 3.14f;
    CFMutableDictionaryRef lMutableDictionary = NULL;
    CFNumberRef            lNumber            = NULL;

    lMutableDictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                   0,
                                                   &kCFTypeDictionaryKeyCallBacks,
                                                   &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(lMutableDictionary != NULL);

    // Add a Boolean key/value pair

    CFDictionarySetValue(lMutableDictionary, CFSTR("Boolean"), kCFBooleanTrue);

    // Add a string key/value pair

    CFDictionarySetValue(lMutableDictionary, CFSTR("String"), CFSTR("String"));

    // Add an integer key/value pair

    lNumber = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &lInteger);
    CPPUNIT_ASSERT(lNumber != NULL);

    CFDictionarySetValue(lMutableDictionary, CFSTR("Integer"), lNumber);

    CFRelease(lNumber);

    // Add a real key/value pair

    lNumber = CFNumberCreate(kCFAllocatorDefault, kCFNumberFloatType, &lReal);
    CPPUNIT_ASSERT(lNumber != NULL);

    CFDictionarySetValue(lMutableDictionary, CFSTR("Real"), lNumber);

    CFRelease(lNumber);

    mDictionaryRef = lMutableDictionary;
}

void
TestCFUPropertyListWrite :: SetUpTemporaryPath(void)
{
    NameTemporary(mValidPropertyListTemporaryPath,
                  "/tmp/cfu-test-write-plistXXXXXX");
}

void
TestCFUPropertyListWrite :: NameTemporary(char *       aPathBuffer,
                                          const char * aPathPattern)
{
    int lStatus;

    // Create, close, and immediately unlink a file such that we have a
    // randomly-named and likely non-existent file.

    aPathBuffer[0] = '\0';
    strcat(aPathBuffer, aPathPattern);

    lStatus = mkstemp(aPathBuffer);
    CPPUNIT_ASSERT(lStatus > 0);

    close(lStatus);

    lStatus = unlink(aPathBuffer);
    CPPUNIT_ASSERT(lStatus == 0);
}

void
TestCFUPropertyListWrite :: TearDown(void)
{
    // For NULL parameter tests, there may not be a file at this path,
    // so ignore the return status of unlink.

    unlink(mValidPropertyListTemporaryPath);

    if (mDictionaryRef != NULL) {
        CFRelease(mDictionaryRef);
    }
}

void
TestCFUPropertyListWriteToFile :: setUp(void)
{
    TestCFUPropertyListWrite::SetUp();
}

void
TestCFUPropertyListWriteToFile :: tearDown(void)
{
    TestCFUPropertyListWrite::TearDown();
}

void
TestCFUPropertyListWriteToFile :: TestNull(void)
{
    const CFPropertyListFormat kFormat       = kCFPropertyListXMLFormat_v1_0;
    const bool                 kWritable     = true;
    CFPropertyListRef          lPropertyList = NULL;
    bool                       lStatus;

    // NULL-terminated C string path

    lStatus = CFUPropertyListWriteToFile(static_cast<const char *>(0),
                                         !kWritable,
                                         kFormat,
                                         lPropertyList,
                                         NULL);
    CPPUNIT_ASSERT(lStatus == false);

    lStatus = CFUPropertyListWriteToFile("/tmp/test.plist",
                                         !kWritable,
                                         kFormat,
                                         NULL,
                                         NULL);
    CPPUNIT_ASSERT(lStatus == false);

    lStatus = CFUPropertyListWriteToFile(static_cast<const char *>(0),
                                         !kWritable,
                                         kFormat,
                                         NULL,
                                         NULL);
    CPPUNIT_ASSERT(lStatus == false);

    // CoreFoundation String path

    lStatus = CFUPropertyListWriteToFile(static_cast<CFStringRef>(0),
                                         kFormat,
                                         &lPropertyList,
                                         NULL);
    CPPUNIT_ASSERT(lStatus == false);

    lStatus = CFUPropertyListWriteToFile(CFSTR("/tmp/test.plist"),
                                         kFormat,
                                         NULL,
                                         NULL);
    CPPUNIT_ASSERT(lStatus == false);

    lStatus = CFUPropertyListWriteToFile(static_cast<CFStringRef>(0),
                                         kFormat,
                                         NULL,
                                         NULL);
    CPPUNIT_ASSERT(lStatus == false);
}

void
TestCFUPropertyListWriteToFile :: TestInvalidCStringFormat(void)
{
    const CFPropertyListFormat kInvalidFormat = static_cast<CFPropertyListFormat>(400);
    const bool                 kWritable      = true;
    CFPropertyListRef          lPropertyList  = mDictionaryRef;
    CFStringRef                lError         = NULL;
    bool                       lStatus;

    lStatus = CFUPropertyListWriteToFile(mValidPropertyListTemporaryPath,
                                         kWritable,
                                         kInvalidFormat,
                                         lPropertyList,
                                         &lError);
    CPPUNIT_ASSERT(lStatus != true);
    CPPUNIT_ASSERT(lError == NULL);

    if (lError != NULL) {
        CFRelease(lError);
    }
}

void
TestCFUPropertyListWriteToFile :: TestNonNullCStringReadableXML(void)
{
    const bool                 kWritable = true;
    const CFPropertyListFormat kFormat   = kCFPropertyListXMLFormat_v1_0;

    TestNonNullCString(!kWritable, kFormat);
}

void
TestCFUPropertyListWriteToFile :: TestNonNullCStringReadableBinary(void)
{
    const bool                 kWritable = true;
    const CFPropertyListFormat kFormat   = kCFPropertyListBinaryFormat_v1_0;

    TestNonNullCString(!kWritable, kFormat);
}

void
TestCFUPropertyListWriteToFile :: TestNonNullCStringWritableXML(void)
{
    const bool                 kWritable = true;
    const CFPropertyListFormat kFormat   = kCFPropertyListXMLFormat_v1_0;

    TestNonNullCString(kWritable, kFormat);
}

void
TestCFUPropertyListWriteToFile :: TestNonNullCStringWritableBinary(void)
{
    const bool                 kWritable = true;
    const CFPropertyListFormat kFormat   = kCFPropertyListBinaryFormat_v1_0;

    TestNonNullCString(kWritable, kFormat);
}

void
TestCFUPropertyListWriteToFile :: TestNonNullCString(const bool & inWritable,
                                                     const CFPropertyListFormat & inFormat)
{
    const int         lOpenFlags    = (inWritable ? O_RDWR : O_RDONLY);
    CFPropertyListRef lPropertyList = mDictionaryRef;
    CFStringRef       lError        = NULL;
    bool              lStatus;
    int               lOpenStatus;

    lStatus = CFUPropertyListWriteToFile(mValidPropertyListTemporaryPath,
                                         inWritable,
                                         inFormat,
                                         lPropertyList,
                                         &lError);
    CPPUNIT_ASSERT(lStatus == true);
    CPPUNIT_ASSERT(lError == NULL);

    // Attempt to open the file with the appropriate flags to ensure
    // the writability parameter was respected.

    lOpenStatus = open(mValidPropertyListTemporaryPath, lOpenFlags);
    CPPUNIT_ASSERT(lOpenStatus > 0);

    close(lOpenStatus);
}

void
TestCFUPropertyListWriteToFile :: TestInvalidCFStringFormat(void)
{
    const CFPropertyListFormat kInvalidFormat = static_cast<CFPropertyListFormat>(400);
    CFStringRef                lPath          = NULL;
    CFPropertyListRef          lPropertyList  = mDictionaryRef;
    CFStringRef                lError         = NULL;
    bool                       lStatus;

    lPath = CFStringCreateWithCString(kCFAllocatorDefault,
                                      mValidPropertyListTemporaryPath,
                                      CFStringGetSystemEncoding());
    CPPUNIT_ASSERT(lPath != NULL);

    lStatus = CFUPropertyListWriteToFile(lPath,
                                         kInvalidFormat,
                                         lPropertyList,
                                         &lError);
    CPPUNIT_ASSERT(lStatus != true);
    CPPUNIT_ASSERT(lError == NULL);

    if (lError != NULL) {
        CFRelease(lError);
    }

    if (lPath != NULL) {
        CFRelease(lPath);
    }
}

void
TestCFUPropertyListWriteToFile :: TestNonNullCFStringXML(void)
{
    const CFPropertyListFormat kFormat = kCFPropertyListXMLFormat_v1_0;

    TestNonNullCFString(kFormat);
}

void
TestCFUPropertyListWriteToFile :: TestNonNullCFStringBinary(void)
{
    const CFPropertyListFormat kFormat = kCFPropertyListBinaryFormat_v1_0;

    TestNonNullCFString(kFormat);
}

void
TestCFUPropertyListWriteToFile :: TestNonNullCFString(const CFPropertyListFormat &inFormat)
{
    CFStringRef       lPath         = NULL;
    CFPropertyListRef lPropertyList = mDictionaryRef;
    CFStringRef       lError        = NULL;
    bool              lStatus;

    lPath = CFStringCreateWithCString(kCFAllocatorDefault,
                                      mValidPropertyListTemporaryPath,
                                      CFStringGetSystemEncoding());
    CPPUNIT_ASSERT(lPath != NULL);

    lStatus = CFUPropertyListWriteToFile(lPath,
                                         inFormat,
                                         lPropertyList,
                                         &lError);
    CPPUNIT_ASSERT(lStatus == true);
    CPPUNIT_ASSERT(lError == NULL);

    if (lPath != NULL) {
        CFRelease(lPath);
    }
}

void
TestCFUPropertyListWriteToURL :: setUp(void)
{
    TestCFUPropertyListWrite::SetUp();
}

void
TestCFUPropertyListWriteToURL :: tearDown(void)
{
    TestCFUPropertyListWrite::TearDown();
}

void
TestCFUPropertyListWriteToURL :: TestNull(void)
{
    const CFPropertyListFormat kFormat       = kCFPropertyListXMLFormat_v1_0;
    const bool                 kIsDirectory  = true;
    CFURLRef                   lURLRef       = NULL;
    CFPropertyListRef          lPropertyList = NULL;
    bool                       lStatus;

    lURLRef = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                            CFSTR("/tmp/test.plist"),
                                            kCFURLPOSIXPathStyle,
                                            !kIsDirectory);
    CPPUNIT_ASSERT(lURLRef != NULL);

    lStatus = CFUPropertyListWriteToURL(NULL, kFormat, lPropertyList, NULL);
    CPPUNIT_ASSERT(lStatus == false);

    lStatus = CFUPropertyListWriteToURL(lURLRef, kFormat, NULL, NULL);
    CPPUNIT_ASSERT(lStatus == false);

    lStatus = CFUPropertyListWriteToURL(NULL, kFormat, NULL, NULL);
    CPPUNIT_ASSERT(lStatus == false);

    if (lURLRef != NULL) {
        CFRelease(lURLRef);
    }
}

void
TestCFUPropertyListWriteToURL :: TestInvalidFormat(void)
{
    const CFPropertyListFormat kInvalidFormat = static_cast<CFPropertyListFormat>(400);
    bool                       kIsDirectory   = true;
    CFStringRef                lPath          = NULL;
    CFURLRef                   lURLRef        = NULL;
    CFPropertyListRef          lPropertyList  = mDictionaryRef;
    CFStringRef                lError         = NULL;
    bool                       lStatus;

    lPath = CFStringCreateWithCString(kCFAllocatorDefault,
                                      mValidPropertyListTemporaryPath,
                                      CFStringGetSystemEncoding());
    CPPUNIT_ASSERT(lPath != NULL);

    lURLRef = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                            lPath,
                                            kCFURLPOSIXPathStyle,
                                            !kIsDirectory);
    CPPUNIT_ASSERT(lURLRef != NULL);

    lStatus = CFUPropertyListWriteToURL(lURLRef,
                                        kInvalidFormat,
                                        lPropertyList,
                                        &lError);
    CPPUNIT_ASSERT(lStatus != true);
    CPPUNIT_ASSERT(lError == NULL);

    if (lError != NULL) {
        CFRelease(lError);
    }

    if (lPath != NULL) {
        CFRelease(lPath);
    }

    if (lURLRef != NULL) {
        CFRelease(lURLRef);
    }
}

void
TestCFUPropertyListWriteToURL :: TestNonNullXML(void)
{
    const CFPropertyListFormat kFormat = kCFPropertyListXMLFormat_v1_0;

    TestNonNull(kFormat);
}

void
TestCFUPropertyListWriteToURL :: TestNonNullBinary(void)
{
    const CFPropertyListFormat kFormat = kCFPropertyListBinaryFormat_v1_0;

    TestNonNull(kFormat);
}

void
TestCFUPropertyListWriteToURL :: TestNonNull(const CFPropertyListFormat &inFormat)
{
    bool              kIsDirectory  = true;
    CFStringRef       lPath         = NULL;
    CFURLRef          lURLRef       = NULL;
    CFPropertyListRef lPropertyList = mDictionaryRef;
    CFStringRef       lError        = NULL;
    bool              lStatus;

    lPath = CFStringCreateWithCString(kCFAllocatorDefault,
                                      mValidPropertyListTemporaryPath,
                                      CFStringGetSystemEncoding());
    CPPUNIT_ASSERT(lPath != NULL);

    lURLRef = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                            lPath,
                                            kCFURLPOSIXPathStyle,
                                            !kIsDirectory);
    CPPUNIT_ASSERT(lURLRef != NULL);

    lStatus = CFUPropertyListWriteToURL(lURLRef,
                                        inFormat,
                                        lPropertyList,
                                        &lError);
    CPPUNIT_ASSERT(lStatus == true);
    CPPUNIT_ASSERT(lError == NULL);

    if (lPath != NULL) {
        CFRelease(lPath);
    }

    if (lURLRef != NULL) {
        CFRelease(lURLRef);
    }
}
