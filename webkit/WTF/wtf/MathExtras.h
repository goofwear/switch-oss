/*
 * Copyright (C) 2006, 2007, 2008, 2009, 2010, 2013 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef WTF_MathExtras_h
#define WTF_MathExtras_h

#include <algorithm>
#include <cmath>
#include <float.h>
#include <limits>
#include <stdint.h>
#include <stdlib.h>
#include <wtf/StdLibExtras.h>

#if OS(SOLARIS)
#include <ieeefp.h>
#endif

#if OS(OPENBSD)
#include <sys/types.h>
#include <machine/ieee.h>
#endif

#ifndef M_PI
const double piDouble = 3.14159265358979323846;
const float piFloat = 3.14159265358979323846f;
#else
const double piDouble = M_PI;
const float piFloat = static_cast<float>(M_PI);
#endif

#ifndef M_PI_2
const double piOverTwoDouble = 1.57079632679489661923;
const float piOverTwoFloat = 1.57079632679489661923f;
#else
const double piOverTwoDouble = M_PI_2;
const float piOverTwoFloat = static_cast<float>(M_PI_2);
#endif

#ifndef M_PI_4
const double piOverFourDouble = 0.785398163397448309616;
const float piOverFourFloat = 0.785398163397448309616f;
#else
const double piOverFourDouble = M_PI_4;
const float piOverFourFloat = static_cast<float>(M_PI_4);
#endif

#ifndef M_SQRT2
const double sqrtOfTwoDouble = 1.41421356237309504880;
const float sqrtOfTwoFloat = 1.41421356237309504880f;
#else
const double sqrtOfTwoDouble = M_SQRT2;
const float sqrtOfTwoFloat = static_cast<float>(M_SQRT2);
#endif

#if OS(SOLARIS)

namespace std {

#ifndef isfinite
inline bool isfinite(double x) { return finite(x) && !isnand(x); }
#endif
#ifndef signbit
inline bool signbit(double x) { return copysign(1.0, x) < 0; }
#endif
#ifndef isinf
inline bool isinf(double x) { return !finite(x) && !isnand(x); }
#endif

} // namespace std

#endif

#if COMPILER(MSVC)

#if _MSC_VER < 1800
extern "C" inline double log2(double num)
{
    return log(num) / 0.693147180559945309417232121458176568;
}
extern "C" inline float log2f(float num)
{
    return log(num) / 0.693147180559945309417232121458176568;
}
#endif

// Work around a bug in Win, where atan2(+-infinity, +-infinity) yields NaN instead of specific values.
extern "C" inline double wtf_atan2(double x, double y)
{
    double posInf = std::numeric_limits<double>::infinity();
    double negInf = -std::numeric_limits<double>::infinity();
    double nan = std::numeric_limits<double>::quiet_NaN();

    double result = nan;

    if (x == posInf && y == posInf)
        result = piOverFourDouble;
    else if (x == posInf && y == negInf)
        result = 3 * piOverFourDouble;
    else if (x == negInf && y == posInf)
        result = -piOverFourDouble;
    else if (x == negInf && y == negInf)
        result = -3 * piOverFourDouble;
    else
        result = ::atan2(x, y);

    return result;
}

#if _MSC_VER < 1800
// We must not do 'num + 0.5' or 'num - 0.5' because they can cause precision loss.
static double round(double num)
{
    double integer = ceil(num);
    if (num > 0)
        return integer - num > 0.5 ? integer - 1.0 : integer;
    return integer - num >= 0.5 ? integer - 1.0 : integer;
}
static float roundf(float num)
{
    float integer = ceilf(num);
    if (num > 0)
        return integer - num > 0.5f ? integer - 1.0f : integer;
    return integer - num >= 0.5f ? integer - 1.0f : integer;
}
namespace std {
inline double round(double num) { return ::round(num); }
}
#endif
inline long long llround(double num) { return static_cast<long long>(round(num)); }
inline long long llroundf(float num) { return static_cast<long long>(roundf(num)); }
inline long lround(double num) { return static_cast<long>(round(num)); }
inline long lroundf(float num) { return static_cast<long>(roundf(num)); }
inline double trunc(double num) { return num > 0 ? floor(num) : ceil(num); }
inline float truncf(float num) { return num > 0 ? floor(num) : ceil(num); }

inline double copysign(double num1, double num2) { return _copysign(num1, num2); }

#if _MSC_VER < 1800
extern "C" double nextafter(double from, double to);
extern "C" float nextafterf(float from, float to);
#endif

namespace std {

inline bool isinf(double num) { return !_finite(num) && !_isnan(num); }
inline bool isnan(double num) { return !!_isnan(num); }
inline bool isfinite(double x) { return !!_finite(x); }
#if _MSC_VER < 1800
inline bool signbit(double num) { return _copysign(1.0, num) < 0; }
#endif

} // namespace std



// Work around a bug in the Microsoft CRT, where fmod(x, +-infinity) yields NaN instead of x.
extern "C" inline double wtf_fmod(double x, double y) { return (!std::isinf(x) && std::isinf(y)) ? x : fmod(x, y); }

// Work around a bug in the Microsoft CRT, where pow(NaN, 0) yields NaN instead of 1.
extern "C" inline double wtf_pow(double x, double y) { return y == 0 ? 1 : pow(x, y); }

#define atan2(x, y) wtf_atan2(x, y)
#define fmod(x, y) wtf_fmod(x, y)
#define pow(x, y) wtf_pow(x, y)

#endif // COMPILER(MSVC)

inline double deg2rad(double d)  { return d * piDouble / 180.0; }
inline double rad2deg(double r)  { return r * 180.0 / piDouble; }
inline double deg2grad(double d) { return d * 400.0 / 360.0; }
inline double grad2deg(double g) { return g * 360.0 / 400.0; }
inline double turn2deg(double t) { return t * 360.0; }
inline double deg2turn(double d) { return d / 360.0; }
inline double rad2grad(double r) { return r * 200.0 / piDouble; }
inline double grad2rad(double g) { return g * piDouble / 200.0; }

inline float deg2rad(float d)  { return d * piFloat / 180.0f; }
inline float rad2deg(float r)  { return r * 180.0f / piFloat; }
inline float deg2grad(float d) { return d * 400.0f / 360.0f; }
inline float grad2deg(float g) { return g * 360.0f / 400.0f; }
inline float turn2deg(float t) { return t * 360.0f; }
inline float deg2turn(float d) { return d / 360.0f; }
inline float rad2grad(float r) { return r * 200.0f / piFloat; }
inline float grad2rad(float g) { return g * piFloat / 200.0f; }

// std::numeric_limits<T>::min() returns the smallest positive value for floating point types
template<typename T> inline T defaultMinimumForClamp() { return std::numeric_limits<T>::min(); }
template<> inline float defaultMinimumForClamp() { return -std::numeric_limits<float>::max(); }
template<> inline double defaultMinimumForClamp() { return -std::numeric_limits<double>::max(); }
template<typename T> inline T defaultMaximumForClamp() { return std::numeric_limits<T>::max(); }

template<typename T> inline T clampTo(double value, T min = defaultMinimumForClamp<T>(), T max = defaultMaximumForClamp<T>())
{
    if (value >= static_cast<double>(max))
        return max;
    if (value <= static_cast<double>(min))
        return min;
    return static_cast<T>(value);
}
template<> inline long long int clampTo(double, long long int, long long int); // clampTo does not support long long ints.

inline int clampToInteger(double value)
{
    return clampTo<int>(value);
}

inline unsigned clampToUnsigned(double value)
{
    return clampTo<unsigned>(value);
}

inline float clampToFloat(double value)
{
    return clampTo<float>(value);
}

inline int clampToPositiveInteger(double value)
{
    return clampTo<int>(value, 0);
}

inline int clampToInteger(float value)
{
    return clampTo<int>(value);
}

inline int clampToInteger(unsigned x)
{
    const unsigned intMax = static_cast<unsigned>(std::numeric_limits<int>::max());

    if (x >= intMax)
        return std::numeric_limits<int>::max();
    return static_cast<int>(x);
}

inline bool isWithinIntRange(float x)
{
    return x > static_cast<float>(std::numeric_limits<int>::min()) && x < static_cast<float>(std::numeric_limits<int>::max());
}

template<typename T> inline bool hasOneBitSet(T value)
{
    return !((value - 1) & value) && value;
}

template<typename T> inline bool hasZeroOrOneBitsSet(T value)
{
    return !((value - 1) & value);
}

template<typename T> inline bool hasTwoOrMoreBitsSet(T value)
{
    return !hasZeroOrOneBitsSet(value);
}

template <typename T> inline unsigned getLSBSet(T value)
{
    unsigned result = 0;

    while (value >>= 1)
        ++result;

    return result;
}

template<typename T> inline T divideRoundedUp(T a, T b)
{
    return (a + b - 1) / b;
}

template<typename T> inline T timesThreePlusOneDividedByTwo(T value)
{
    // Mathematically equivalent to:
    //   (value * 3 + 1) / 2;
    // or:
    //   (unsigned)ceil(value * 1.5));
    // This form is not prone to internal overflow.
    return value + (value >> 1) + (value & 1);
}

template<typename T> inline bool isNotZeroAndOrdered(T value)
{
    return value > 0.0 || value < 0.0;
}

template<typename T> inline bool isZeroOrUnordered(T value)
{
    return !isNotZeroAndOrdered(value);
}

template<typename T> inline bool isGreaterThanNonZeroPowerOfTwo(T value, unsigned power)
{
    // The crazy way of testing of index >= 2 ** power
    // (where I use ** to denote pow()).
    return !!((value >> 1) >> (power - 1));
}

#ifndef UINT64_C
#if COMPILER(MSVC)
#define UINT64_C(c) c ## ui64
#else
#define UINT64_C(c) c ## ull
#endif
#endif

#if COMPILER(MINGW64) && (!defined(__MINGW64_VERSION_RC) || __MINGW64_VERSION_RC < 1)
inline double wtf_pow(double x, double y)
{
    // MinGW-w64 has a custom implementation for pow.
    // This handles certain special cases that are different.
    if ((x == 0.0 || std::isinf(x)) && std::isfinite(y)) {
        double f;
        if (modf(y, &f) != 0.0)
            return ((x == 0.0) ^ (y > 0.0)) ? std::numeric_limits<double>::infinity() : 0.0;
    }

    if (x == 2.0) {
        int yInt = static_cast<int>(y);
        if (y == yInt)
            return ldexp(1.0, yInt);
    }

    return pow(x, y);
}
#define pow(x, y) wtf_pow(x, y)
#endif // COMPILER(MINGW64) && (!defined(__MINGW64_VERSION_RC) || __MINGW64_VERSION_RC < 1)


// decompose 'number' to its sign, exponent, and mantissa components.
// The result is interpreted as:
//     (sign ? -1 : 1) * pow(2, exponent) * (mantissa / (1 << 52))
inline void decomposeDouble(double number, bool& sign, int32_t& exponent, uint64_t& mantissa)
{
    ASSERT(std::isfinite(number));

    sign = std::signbit(number);

    uint64_t bits = WTF::bitwise_cast<uint64_t>(number);
    exponent = (static_cast<int32_t>(bits >> 52) & 0x7ff) - 0x3ff;
    mantissa = bits & 0xFFFFFFFFFFFFFull;

    // Check for zero/denormal values; if so, adjust the exponent,
    // if not insert the implicit, omitted leading 1 bit.
    if (exponent == -0x3ff)
        exponent = mantissa ? -0x3fe : 0;
    else
        mantissa |= 0x10000000000000ull;
}

// Calculate d % 2^{64}.
inline void doubleToInteger(double d, unsigned long long& value)
{
    if (std::isnan(d) || std::isinf(d))
        value = 0;
    else {
        // -2^{64} < fmodValue < 2^{64}.
        double fmodValue = fmod(trunc(d), std::numeric_limits<unsigned long long>::max() + 1.0);
        if (fmodValue >= 0) {
            // 0 <= fmodValue < 2^{64}.
            // 0 <= value < 2^{64}. This cast causes no loss.
            value = static_cast<unsigned long long>(fmodValue);
        } else {
            // -2^{64} < fmodValue < 0.
            // 0 < fmodValueInUnsignedLongLong < 2^{64}. This cast causes no loss.
            unsigned long long fmodValueInUnsignedLongLong = static_cast<unsigned long long>(-fmodValue);
            // -1 < (std::numeric_limits<unsigned long long>::max() - fmodValueInUnsignedLongLong) < 2^{64} - 1.
            // 0 < value < 2^{64}.
            value = std::numeric_limits<unsigned long long>::max() - fmodValueInUnsignedLongLong + 1;
        }
    }
}

namespace WTF {

// From http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
inline uint32_t roundUpToPowerOfTwo(uint32_t v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

inline unsigned fastLog2(unsigned i)
{
    unsigned log2 = 0;
    if (i & (i - 1))
        log2 += 1;
    if (i >> 16)
        log2 += 16, i >>= 16;
    if (i >> 8)
        log2 += 8, i >>= 8;
    if (i >> 4)
        log2 += 4, i >>= 4;
    if (i >> 2)
        log2 += 2, i >>= 2;
    if (i >> 1)
        log2 += 1;
    return log2;
}

template <typename T>
inline typename std::enable_if<std::is_floating_point<T>::value, T>::type safeFPDivision(T u, T v)
{
    // Protect against overflow / underflow.
    if (v < 1 && u > v * std::numeric_limits<T>::max())
        return std::numeric_limits<T>::max();
    if (v > 1 && u < v * std::numeric_limits<T>::min())
        return 0;
    return u / v;
}

// Floating point numbers comparison:
// u is "essentially equal" [1][2] to v if: | u - v | / |u| <= e and | u - v | / |v| <= e
//
// [1] Knuth, D. E. "Accuracy of Floating Point Arithmetic." The Art of Computer Programming. 3rd ed. Vol. 2.
//     Boston: Addison-Wesley, 1998. 229-45.
// [2] http://www.boost.org/doc/libs/1_34_0/libs/test/doc/components/test_tools/floating_point_comparison.html
template <typename T>
inline typename std::enable_if<std::is_floating_point<T>::value, bool>::type areEssentiallyEqual(T u, T v, T epsilon = std::numeric_limits<T>::epsilon())
{
    if (u == v)
        return true;

    const T delta = std::abs(u - v);
    return safeFPDivision(delta, std::abs(u)) <= epsilon && safeFPDivision(delta, std::abs(v)) <= epsilon;
}

inline bool isIntegral(float value)
{
    return static_cast<int>(value) == value;
}

} // namespace WTF

#endif // #ifndef WTF_MathExtras_h
