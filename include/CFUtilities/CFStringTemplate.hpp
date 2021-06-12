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
#include <map>
#include <memory>

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
        mString(NULL),
        mCache()
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
        mString(NULL),
        mCache()
    {
        CFUReferenceSet(mString, inString);

        mCache.clear();
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
        mString(NULL),
        mCache()
    {
        CFUReferenceSet(mString, inString.mString);

        mCache.clear();
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

        mCache.clear();

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

        mCache.clear();

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
     *  If the string cannot be returned in O(1) time, an encoding
     *  buffer cache is allocated for the desired encoding and a
     *  pointer to that encoding buffer cache is returned.
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
     *  associated with the object using UTF-8 encoding.
     *
     *  If the string cannot be returned in O(1) time, an encoding
     *  buffer cache is allocated for the desired encoding and a
     *  pointer to that encoding buffer cache is returned.
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
    const char * GetUTF8String(void) const
    {
        return (GetCString(kCFStringEncodingUTF8));
    }

    /**
     *  This routine attempts to return in O(1) time, the C string
     *  associated with the object using the specified encoding.
     *  If the string cannot be returned in O(1) time, an encoding
     *  buffer cache is allocated for the desired encoding and a
     *  pointer to that encoding buffer cache is returned.
     *
     *
     *  If the string cannot be returned in O(1) time, an encoding
     *  buffer cache is allocated for the desired encoding and a
     *  pointer to that encoding buffer cache is returned.
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
        static const char * const kEmptyString = "";
        const char *              lRetval      = NULL;


        if (mString == NULL)
        {
            lRetval = kEmptyString;
        }
        else
        {
            const CFIndex lLength = GetLength();

            if (lLength == 0)
            {
                lRetval = kEmptyString;
            }
            else
            {
                // Attempt to get an O(1) representation supported by
                // CoreFoundation itself, if any.

                lRetval = CFStringGetCStringPtr(mString, inEncoding);

                if (lRetval == NULL)
                {
                    // Per the CoreFoundation documentation, there is
                    // no O(1) representation of the string in the
                    // requested encoding available. Consequently, we
                    // ahve to check the encoding buffer cache local
                    // to this implementation and return the
                    // representation, if present, or create one and
                    // return it, if one is not present.

                    EncodingBufferCache::const_iterator lLast   = mCache.end();
                    EncodingBufferCache::const_iterator lResult = mCache.find(inEncoding);

                    if (lResult != lLast)
                    {
                        // The encoding was in the cache, return it.

                        lRetval = &lResult->second[0];
                    }
                    else
                    {
                        // There was no encoding in the cache, create
                        // one, add it to the cache, and return it.

                        const CFIndex  lSize = CFStringGetMaximumSizeForEncoding(lLength, inEncoding) + static_cast<CFIndex>(sizeof('\0'));
                        EncodingBuffer lEncodingBuffer(new char[static_cast<size_t>(lSize)]);

                        if (lEncodingBuffer.get() != NULL)
                        {
                            const bool lStatus =
                                CFStringGetCString(mString,
                                                   &lEncodingBuffer[0],
                                                   lSize,
                                                   inEncoding);

                            if (lStatus)
                            {
                                mCache[inEncoding] = std::move(lEncodingBuffer);

                                lRetval = mCache[inEncoding].get();
                            }
                        }
                    }
                }
            }
        }

        return (lRetval);
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

        mCache.clear();
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
    /*
     *  The CoreFoundation documentation for @a CFStringGetCStringPtr says:
     *
     *    "Whether or not this function returns a valid pointer or
     *     NULL depends on many factors, all of which depend on how the
     *     string was created and its properties. In addition, the
     *     function result might change between different releases and
     *     on different platforms. So do not count on receiving a
     *     non-NULL result from this function under any circumstances."
     *
     *  While this is true and has been observed to behave differently
     *  across, for example, both iOS and macOS, the behavior is
     *  undesirable from the perspective of users of this
     *  library. Consequently, an encoding buffer cache is implemented
     *  to back storage for @a CFStringGetCString such that on the
     *  n+1th call to CFStringGetCStringPtr, an O(1) representation in
     *  a given encoding is always available.
     *
     *  Why an {auto,unique}_ptr and not a vector of characters?  Size
     *  and speed. A vector carries additional implementation overhead
     *  relative to a "smart" pointer and, additionally, always
     *  initializes every element of the vector. Neither the
     *  implementation overhead nor the superfluous initialization are
     *  needed.
     *
     */
#if __cplusplus >= 201103L
    typedef std::unique_ptr<char []>                   EncodingBuffer;
#else
    typedef std::auto_ptr<char []>                     EncodingBuffer;
#endif
    typedef std::map<CFStringEncoding, EncodingBuffer> EncodingBufferCache;

    CFStringType                mString;
    mutable EncodingBufferCache mCache;
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
