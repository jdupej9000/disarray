#pragma once

#include <cassert>

#if (__cplusplus >= 201703L || (defined(_MSVC_LANG) && (_MSVC_LANG >= 201703L)))
#define DSRY_CPP17
#endif

#if (__cplusplus >= 202002L || (defined(_MSVC_LANG) && (_MSVC_LANG >= 202002L)))
#define DSRY_CPP20
#endif

// If defined, the use of x86 BMI and BMI2 instructions is allowed.
#define DSRY_BMI

#define dsry_assert(x) assert(x)
#define dsry_assert_notreached assert(false)

#if defined(DSRY_CPP20)
#define dsry_likely(x) (x) [[likely]]
#define dsry_unlikely(x) (x) [[unlikely]]
#else
#define dsry_likely(x) (x)
#define dsry_unlikely(x) (x)
#endif
