/*
 *    Copyright (c) 2008-2021 Nuovation System Designs, LLC
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
 *      This file defines C utility interfaces for interacting with
 *      Apple's CoreFoundation framework.
 */

#ifndef CFUTILITIES_CFUTILITIES_H
#define CFUTILITIES_CFUTILITIES_H

#include <stdbool.h>

#include <CoreFoundation/CoreFoundation.h>

#ifdef __cplusplus
extern "C" {
#endif

// CFBase Operations

extern bool            CFUIsTypeID(CFTypeRef inReference, CFTypeID inID);

extern void            CFURelease(CFTypeRef inReference);

// CFBoolean Operations

extern CFBooleanRef    CFUBooleanCreate(bool inBoolean);

// CFDate and CFTime Operations

extern time_t          CFUAbsoluteTimeGetPOSIXTime(CFAbsoluteTime inAt);
extern CFAbsoluteTime  CFUPOSIXTimeGetAbsoluteTime(time_t inTime);
extern time_t          CFUDateGetPOSIXTime(CFDateRef inDate);
extern CFDateRef       CFUDateCreate(CFAllocatorRef inAllocator, time_t inTime);

// CFDictionary Operations

extern CFArrayRef      CFUDictionaryCopyKeys(CFDictionaryRef inDictionary);
extern void            CFUDictionaryMerge(CFMutableDictionaryRef inDestination,
                                          CFDictionaryRef        inSource,
                                          bool                   inReplace);
extern Boolean         CFUDictionarySetCString(CFMutableDictionaryRef inDestination,
                                               const void *           inKey,
                                               const char *           inString);

// CFNumber Operations

extern CFNumberType    CFUGetNumberType(bool isIntegral, size_t inBytes);

// CFPropertyList Operations

extern Boolean         CFUPropertyListReadFromFile(const char *        inPath,
                                                   CFOptionFlags       inMutability,
                                                   CFPropertyListRef * outPlist,
                                                   CFStringRef *       outError);
extern Boolean         CFUPropertyListWriteToFile(const char *         inPath,
                                                  bool                 inWritable,
                                                  CFPropertyListFormat inFormat,
                                                  CFPropertyListRef    inPlist,
                                                  CFStringRef *        outError);

extern Boolean         CFUPropertyListReadFromURL(CFURLRef            inURL,
                                                  CFOptionFlags       inMutability,
                                                  CFPropertyListRef * outPlist,
                                                  CFStringRef *       outError);
extern Boolean         CFUPropertyListWriteToURL(CFURLRef             inURL,
                                                 CFPropertyListFormat inFormat,
                                                 CFPropertyListRef    inPlist,
                                                 CFStringRef *        outError);

// CFSet Operations

extern bool            CFUSetIsEmptySet(CFSetRef theSet);
extern void            CFUSetIntersectionSet(CFMutableSetRef theSet,
                                             CFSetRef        theOtherSet);
extern void            CFUSetUnionSet(CFMutableSetRef theSet,
                                      CFSetRef        theOtherSet);

// CFTree Operations

extern void            CFUTreeContextInit(CFTreeContext * inContext);
extern CFTreeRef       CFUTreeCreate(CFTypeRef inType);

// CFString Operations

extern bool            CFUStringsMatch(CFStringRef aFirst, CFStringRef aSecond);

#ifdef __cplusplus
}
#endif

#endif /* CFUTILITIES_CFUTILITIES_H */
