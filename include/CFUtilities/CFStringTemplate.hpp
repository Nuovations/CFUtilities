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
 *      This file defines and implements an object wrapper for
 *      CoreFoundation immutable and mutable string types.
 */

#ifndef CFUTILITIES_CFSTRING_TEMPLATE_HPP
#define CFUTILITIES_CFSTRING_TEMPLATE_HPP

#include <algorithm>

#include <CoreFoundation/CoreFoundation.h>

#include "CFUtilities.hpp"

#ifdef __cplusplus

/**
 *  A template to simplify interwork between CoreFoundation strings
 *  and C/C++ standard library string types and code.
 *
 *  @tparam  CFStringType  The CoreFoundation string type, either
 *                         @a CFStringRef or @a CFMutableStringRef.
 *
 *  @ingroup string
 */
template <typename CFStringType>
class CFStringTemplate
{
public:
    /**
     *  This routine is the default object constructor. Is
     *  instantiates an object with a NULL CoreFoundation string
     *  type reference.
     *
     */
    CFStringTemplate(void) :
        mString(NULL)
    {
        return;
    }

    /**
     *  This routine is an object constructor. Is instantiates an
     *  object with the specified CoreFoundation string.
     *
     *  @param[in]  inString    The CoreFoundation string reference with
     *                          which to construct the object.
     *
     */
    CFStringTemplate(CFStringType inString) :
        mString(NULL)
    {
        CFUReferenceSet(mString, inString);
    }

    /**
     *  This routine is a class copy constructor. It instantiates an
     *  object with the specified object.
     *
     *  @param[in]  inString  A reference to the object with which to
     *                        construct the object.
     *
     */
    CFStringTemplate(const CFStringTemplate & inString) :
        mString(NULL)
    {
        CFUReferenceSet(mString, inString.mString);
    }

    /**
     *  This routine is the class destructor. It simply releases the
     *  internal string data member.
     *
     */
    virtual ~CFStringTemplate(void)
    {
        CFURelease(mString);
    }

    /**
     *  This routine is a class assignment operator. It assigns a
     *  CoreFoundation string reference type to the object.
     *
     *  @param[in]  inString  The CoreFoundation string reference to
     *                        assign to the object.
     *
     *  @returns
     *    A reference to the assigned to (lvalue) object.
     *
     */
    CFStringTemplate & operator =(CFStringType inString)
    {
        CFUReferenceSet(mString, inString);

        return (*this);
    }

    /**
     *  This routine is a class assignment operator, specifically,
     *  the copy operator. It assigns/copies another like object to
     *  the current object.
     *
     *  @param[in]  inString  A reference to the object to assign/copy
     *                        to the object.
     *
     *  @returns
     *    A reference to the assigned to (lvalue) object.
     *
     */
    CFStringTemplate & operator =(const CFStringTemplate & inString)
    {
        CFUReferenceSet(mString, inString.mString);

        return (*this);
    }

    /**
     *  This routine returns the length, in 16-bit Unicode characters,
     *  of the string.
     *
     *  @returns
     *    The length, in 16-bit Unicode characters, of the string.
     *
     */
    CFIndex GetLength(void) const
    {
        return ((mString == NULL) ? 0 : CFStringGetLength(mString));
    }

    /**
     *  This routine returns status indicating whether or not the
     *  object is empty. That is, contains zero (0) characters.
     *
     *  @returns
     *    True if the string is empty; otherwise, false.
     *
     */
    bool IsEmpty(void) const
    {
        return (GetLength() == 0);
    }

    /**
     *  This routine attempts to return in O(1) time, the C string
     *  associated with the object using the default system
     *  encoding.
     *
     *  @note
     *    VERY careful attention to scoping must be paid attention
     *    to because the storage backed by the returned pointer is
     *    not guaranteed beyond the lifetime of the object.
     *
     *  @returns
     *    A pointer to the string using the system encoding if the
     *    string is decodable; otherwise, NULL.
     *
     */
    const char * GetCString(void) const
    {
        return (GetCString(CFStringGetSystemEncoding()));
    }

    /**
     *  This routine attempts to return in O(1) time, the C string
     *  associated with the object using the specified encoding.
     *
     *  @note
     *    VERY careful attention to scoping must be paid attention to
     *    because the storage backed by the returned pointer is not
     *    guaranteed beyond the lifetime of the object.
     *
     *  @param[in]  inEncoding  The string encoding with which the
     *                          C string is to be returned.
     *
     *  @returns
     *    A pointer to the string using the system encoding if the
     *    string is decodable; otherwise, NULL.
     *
     */
    const char * GetCString(CFStringEncoding inEncoding) const
    {
        return ((mString == NULL) ?
                "" :
                CFStringGetCStringPtr(mString, inEncoding));
    }

    /**
     *  This routine returns the CoreFoundation string with which
     *  the object was instantiated.
     *
     *  @returns
     *    The CoreFoundation string associated with the object.
     *
     */
    CFStringType GetString(void) const
    {
        return (mString);
    }

    /**
     *  This routine swaps, in O(1) time, the internal data member
     *  associated with this object with that of the specified
     *  object.
     *
     *  @param[in]  inString  The string object with which the data
     *                        member is to be swapped.
     *
     */
    void Swap(CFStringTemplate & inString)
    {
        std::swap(mString, inString.mString);
    }

    /**
     *  This routine compares the specified string object for
     *  equality with this one.
     *
     *  @param[in]  inString  A reference to the string object to compare.
     *
     *  @returns
     *    True if the specified string object is equivalent to this
     *    one; otherwise, false.
     *
     */
    bool operator ==(const CFStringTemplate & inString) const
    {
        return (operator ==(inString.mString));
    }

    /**
     *  This routine compares the specified CoreFoundation string
     *  for equality with this string object.
     *
     *  @param[in]  inString  A reference to the CoreFoundation string
     *                        to compare.
     *
     *  @returns
     *    True if the specified string is equivalent to this string
     *    object; otherwise, false.
     *
     */
    bool operator ==(CFStringType inString) const
    {
        return (CFUStringsMatch(mString, inString));
    }

private:
    CFStringType mString;
};

// Predefined specializations for the CFStringTemplate template

/**
 *  Template specialization for immutable CoreFoundation strings (that
 *  is, @a CFStringRef).
 *
 *  @ingroup string
 */
typedef CFStringTemplate<CFStringRef> CFString;

/**
 *  Template specialization for mutable CoreFoundation strings (that
 *  is, @a CFMutableStringRef).
 *
 *  @ingroup string
 */
typedef CFStringTemplate<CFMutableStringRef> CFMutableString;

#endif // __cplusplus

#endif // CFUTILITIES_CFSTRING_TEMPLATE_HPP
