/*
 *    Copyright (c) 2008-2023 Nuovation System Designs, LLC
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
 *      This file defines C++ utility interfaces for interacting with
 *      Apple's CoreFoundation framework.
 */

#ifndef CFUTILITIES_CFUTILITIES_HPP
#define CFUTILITIES_CFUTILITIES_HPP

#ifdef __cplusplus

#include <limits>

#include <boost/type_traits.hpp>

#include <AssertMacros.h>

#include <CoreFoundation/CoreFoundation.h>

#include "CFUtilities.h"


/**
 *  @brief
 *    Release, retain, and assign a CoreFoundation type reference.
 *
 *  This routine sets a CoreFoundation reference by releasing (if
 *  non-NULL) the old reference, retaining (if non-NULL) the new
 *  reference and then settings the old reference (lvalue) to the new
 *  reference (rvalue).
 *
 *  @tparam         T       The CoreFoundation type to retain and set a
 *                          reference to.
 *
 *  @param[in,out]  inTo    A reference to a CoreFoundation reference
 *                          type to be set to (lvalue). On success, if
 *                          non-NULL, a released reference to @a inTo
 *                          and a retained reference to @a inFrom.
 *
 *  @param[in,out]  inFrom  A base CoreFoundation reference type to be
 *                          set from (rvalue). On success, if
 *                          non-NULL, a retained reference to @a
 *                          inFrom.
 *
 *  @ingroup base
 *
 */
template <typename T>
void
CFUReferenceSet(T & inTo, T inFrom)
{
    if (inTo != NULL) {
        CFRelease(inTo);
    }

    if (inFrom != NULL) {
        CFRetain(inFrom);
    }

    inTo = inFrom;
}

/**
 *  @brief
 *    Retain a reference to a CoreFoundation object.
 *
 *  This routine is a wrapper around CoreFoundation's @a CFRetain
 *  function that handles some of the encumbering type-casting
 *  necessary when using the function in a C++ environment.
 *
 *  @tparam         T            The CoreFoundation type of the
 *                               reference to be retained.
 *
 *  @param[in,out]  inReference  A reference to the object to be
 *                               retained. On success, the @a
 *                               inReference reference count is
 *                               increased by one.
 *
 *  @returns
 *    A reference to the retained object.
 *
 *  @ingroup base
 *
 */
template <typename T>
T
CFURetain(T inReference)
{
    return static_cast<T>(const_cast<void *>(CFRetain(inReference)));
}

/**
 *  This function template returns the appropriate CoreFoundation
 *  number type for the specified number.
 *
 *  @tparam     T         The C/C++ plain old data type to attempt to
 *                        convert the CoreFoundation number to.
 *
 *  @param[in]  inNumber  A reference to the number to get a number type for.
 *
 *  @returns
 *    The corresponding CoreFoundation number type if a successful
 *    mapping was found; otherwise, false.
 *
 *  @ingroup number
 *
 */
template <typename T>
CFNumberType
CFUGetNumberType(const T & inNumber)
{
    // Steer pointers towards interpretation as an integral type;
    // otherwise, CoreFoundation will attempt to perform a floating
    // point conversion on the value and get it wrong
    // entirely. Similarly, steer enumerations towards interpretation
    // as an integral type.

    (void)inNumber;

    return (CFUGetNumberType((boost::is_integral<T>::value ||
                              boost::is_pointer<T>::value ||
                              boost::is_enum<T>::value),
                             sizeof (T)));
}

/**
 *  @brief
 *    Creates a CoreFoundation number object using a specified value.
 *
 *  This function template creates a CoreFoundation number with the
 *  given value.
 *
 *  @tparam     T            The type to attempt to create a
 *                           CoreFoundation number for.
 *
 *  @param[in]  inAllocator  A reference to the allocator to use to
 *                           allocate memory for the number.
 *  @param[in]  inNumber     A reference to the number to create a
 *                           CoreFoundation number for.
 *
 *  @returns
 *    A CoreFoundation number reference if successful; otherwise, NULL.
 *
 *  @ingroup number
 *
 */
template <typename T>
CFNumberRef
CFUNumberCreate(CFAllocatorRef inAllocator, const T & inNumber)
{
    const CFNumberType theType = CFUGetNumberType<T>(inNumber);

    return (CFNumberCreate(inAllocator, theType, &inNumber));
}

/**
 *  This function template attempts to convert and copy the
 *  specified CoreFoundation number reference value into the
 *  provided storage.
 *
 *  Behavior otherwise follows that of CFNumberGetValue.
 *
 *
 *  @tparam         T         The type to attempt to convert the
 *                            CoreFoundation number to.
 *
 *  @param[in]      inNumber  A reference to the number to get a
 *                            value for.
 *  @param[in,out]  outValue  A reference to storage for the converted
 *                            value.
 *
 *  @returns
 *    The corresponding CoreFoundation number type if a successful
 *    conversion was performed; otherwise, false.
 *
 *  @ingroup number
 *
 */
template <typename T>
Boolean
CFUNumberGetValue(CFNumberRef inNumber, T & outValue)
{
    return (CFNumberGetValue(inNumber, CFUGetNumberType(outValue), &outValue));
}

/**
 *  This function template attempts to retrieve and convert the
 *  CoreFoundation number associated with the specified key from the
 *  provided dictionary and convert it to the specified value.
 *
 *  @tparam         T             The type to attempt to convert the
 *                                CoreFoundation number to.
 *
 *  @param[in]      inDictionary  A reference to the CoreFoundation
 *                                dictionary to get the number from.
 *  @param[in]      inKey         A pointer to the key for which to
 *                                find a number in the dictionary.
 *  @param[in,out]  outValue      A reference to storage for the
 *                                converted value. On success, a
 *                                reference to the converted value.
 *
 *  @returns
 *    True if the value was found and successfully converted;
 *    otherwise, false.
 *
 *  @ingroup dictionary
 *
 */
template <typename T>
Boolean
CFUDictionaryGetNumber(CFDictionaryRef inDictionary,
                       const void *    inKey,
                       T &             outValue)
{
    bool        status     = false;
    CFNumberRef tempNumber = NULL;

    __Require(inDictionary != NULL, done);
    __Require(inKey != NULL, done);

    // Attempt to get the value from the dictionary.

    tempNumber = static_cast<CFNumberRef>(
        CFDictionaryGetValue(inDictionary, inKey));

    // The key being absent is a quiet failure; whereas, the value not
    // being of type number is.

    __Require_Quiet(tempNumber != NULL, done);
    __Require(CFUIsTypeID(tempNumber, CFNumberGetTypeID()), done);

    status = CFUNumberGetValue<T>(tempNumber, outValue);
    __Check(status != false);

done:
    return (status);
}

/**
 *  This function template attempts to set the specified numeric
 *  value in the provided dictionary associated with the specified
 *  key.
 *
 *  @tparam         T             The type of number to set.
 *
 *  @param[in,out]  inDictionary  A reference to the CoreFoundation
 *                                dictionary to set the numeric value
 *                                in. On success, the dictionary
 *                                contains a reference to the set
 *                                number.
 *  @param[in]      inKey         A pointer to the key with which to
 *                                associate the numeric value in the
 *                                dictionary.
 *  @param[in]      inValue       A reference to the numeric value to
 *                                set.
 *
 *  @returns
 *    True if the value was successfully set; otherwise, false.
 *
 *  @ingroup dictionary
 *
 */
template <typename T>
Boolean
CFUDictionarySetNumber(CFMutableDictionaryRef inDictionary,
                       const void *           inKey,
                       const T &              inValue)
{
    bool        status     = false;
    CFNumberRef tempNumber = NULL;

    __Require(inDictionary != NULL, done);
    __Require(inKey != NULL, done);

    tempNumber = CFUNumberCreate(kCFAllocatorDefault, inValue);
    __Require(tempNumber != NULL, done);

    CFDictionarySetValue(inDictionary, inKey, tempNumber);

    CFRelease(tempNumber);

    status = true;

done:
    return (status);
}

extern Boolean CFUDictionaryGetBoolean(CFDictionaryRef inDictionary,
                                       const void *    inKey,
                                       Boolean &       outValue);
extern Boolean CFUDictionaryGetBoolean(CFDictionaryRef inDictionary,
                                       const void *    inKey,
                                       bool &          outValue);
extern Boolean CFUDictionarySetBoolean(CFMutableDictionaryRef inDictionary,
                                       const void *           inKey,
                                       Boolean                inValue);
extern Boolean CFUDictionaryMerge(CFMutableDictionaryRef inOutBase,
                                  CFDictionaryRef        inAdded,
                                  CFDictionaryRef        inCommon,
                                  CFDictionaryRef        inRemoved);
extern Boolean CFUDictionaryMerge(CFMutableDictionaryRef inOutBase,
                                  CFDictionaryRef        inAdded,
                                  CFDictionaryRef        inCommon,
                                  CFArrayRef             inRemovedKeys);
extern Boolean CFUDictionaryDifference(CFDictionaryRef          inProposed,
                                       CFMutableDictionaryRef & inOutBase,
                                       CFMutableDictionaryRef   outAdded,
                                       CFMutableDictionaryRef   outCommon,
                                       CFMutableDictionaryRef   outRemoved);

extern Boolean CFUPropertyListReadFromFile(CFStringRef         inPath,
                                           CFOptionFlags       inMutability,
                                           CFPropertyListRef * outPlist,
                                           CFStringRef *       outError);
extern Boolean CFUPropertyListWriteToFile(CFStringRef          inPath,
                                          CFPropertyListFormat inFormat,
                                          CFPropertyListRef    inPlist,
                                          CFStringRef *        outError);

#endif // __cplusplus

#endif // CFUTILITIES_CFUTILITIES_HPP
