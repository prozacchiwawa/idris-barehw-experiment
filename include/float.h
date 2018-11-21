#ifndef _IDROS_FLOAT_H_
#define _IDROS_FLOAT_H_

/* This file exists soley to keep Metrowerks' compilers happy.  The version
   used by GCC can be found in /usr/lib/gcc, although it's
   not very informative.  */

#ifndef __FLT_RADIX__   
#define __FLT_RADIX__ 2
#endif
#ifndef __FLT_MANT_DIG__
#define __FLT_MANT_DIG__ 24
#endif
#ifndef __FLT_DIG__
#define __FLT_DIG__ 6
#endif
#ifndef __FLT_EPSILON__
#define __FLT_EPSILON__ 1.19209290e-07F
#endif
#ifndef __FLT_MIN__
#define __FLT_MIN__ 1.17549435e-38F
#endif
#ifndef __FLT_MAX__
#define __FLT_MAX__ 3.40282347e+38F
#endif
#ifndef __FLT_MIN_EXP__
#define __FLT_MIN_EXP__ (-125)
#endif
#ifndef __FLT_MIN_10_EXP__
#define __FLT_MIN_10_EXP__ (-37)
#endif
#ifndef __FLT_MAX_EXP__
#define __FLT_MAX_EXP__ 128
#endif
#ifndef __FLT_MAX_10_EXP__
#define __FLT_MAX_10_EXP__ 38
#endif
#ifndef __DBL_MANT_DIG__
#define __DBL_MANT_DIG__ 53
#endif
#ifndef __DBL_DIG__
#define __DBL_DIG__ 15
#endif
#ifndef __DBL_EPSILON__
#define __DBL_EPSILON__ 2.2204460492503131e-16
#endif
#ifndef __DBL_MIN__
#define __DBL_MIN__ 2.2250738585072014e-308
#endif
#ifndef __DBL_MAX__
#define __DBL_MAX__ 1.7976931348623157e+308
#endif
#ifndef __DBL_MIN_EXP__
#define __DBL_MIN_EXP__ (-1021)
#endif
#ifndef __DBL_MIN_10_EXP__
#define __DBL_MIN_10_EXP__ (-307)
#endif
#ifndef __DBL_MAX_EXP__
#define __DBL_MAX_EXP__ 1024
#endif
#ifndef __DBL_MAX_10_EXP__
#define __DBL_MAX_10_EXP__ 308
#endif
#ifndef __LDBL_MANT_DIG__
#define __LDBL_MANT_DIG__ 53
#endif
#ifndef __LDBL_DIG__  
#define __LDBL_DIG__ 15 
#endif
#ifndef __LDBL_EPSILON__
#define __LDBL_EPSILON__ 2.2204460492503131e-16
#endif
#ifndef __LDBL_MIN__
#define __LDBL_MIN__ 2.2250738585072014e-308
#endif
#ifndef __LDBL_MAX__
#define __LDBL_MAX__ 1.7976931348623157e+308
#endif
#ifndef __LDBL_MIN_EXP__
#define __LDBL_MIN_EXP__ (-1021)
#endif
#ifndef __LDBL_MIN_10_EXP__
#define __LDBL_MIN_10_EXP__ (-307)
#endif
#ifndef __LDBL_MAX_EXP__
#define __LDBL_MAX_EXP__ 1024
#endif
#ifndef __LDBL_MAX_10_EXP__
#define __LDBL_MAX_10_EXP__ 308
#endif

/*
 * ISO C Standard:  5.2.4.2.2  Characteristics of floating types <float.h>
 */

#undef FLT_RADIX
#define FLT_RADIX	__FLT_RADIX__

#undef FLT_MANT_DIG
#undef DBL_MANT_DIG
#undef LDBL_MANT_DIG
#define FLT_MANT_DIG	__FLT_MANT_DIG__
#define DBL_MANT_DIG	__DBL_MANT_DIG__
#define LDBL_MANT_DIG	__LDBL_MANT_DIG__

#undef FLT_DIG
#undef DBL_DIG
#undef LDBL_DIG
#define FLT_DIG		__FLT_DIG__
#define DBL_DIG		__DBL_DIG__
#define LDBL_DIG	__LDBL_DIG__

#undef FLT_MIN_EXP
#undef DBL_MIN_EXP
#undef LDBL_MIN_EXP
#define FLT_MIN_EXP	__FLT_MIN_EXP__
#define DBL_MIN_EXP	__DBL_MIN_EXP__
#define LDBL_MIN_EXP	__LDBL_MIN_EXP__

#undef FLT_MIN_10_EXP
#undef DBL_MIN_10_EXP
#undef LDBL_MIN_10_EXP
#define FLT_MIN_10_EXP	__FLT_MIN_10_EXP__
#define DBL_MIN_10_EXP	__DBL_MIN_10_EXP__
#define LDBL_MIN_10_EXP	__LDBL_MIN_10_EXP__

#undef FLT_MAX_EXP
#undef DBL_MAX_EXP
#undef LDBL_MAX_EXP
#define FLT_MAX_EXP	__FLT_MAX_EXP__
#define DBL_MAX_EXP	__DBL_MAX_EXP__
#define LDBL_MAX_EXP	__LDBL_MAX_EXP__

#undef FLT_MAX_10_EXP
#undef DBL_MAX_10_EXP
#undef LDBL_MAX_10_EXP
#define FLT_MAX_10_EXP	__FLT_MAX_10_EXP__
#define DBL_MAX_10_EXP	__DBL_MAX_10_EXP__
#define LDBL_MAX_10_EXP	__LDBL_MAX_10_EXP__

#undef FLT_MAX
#undef DBL_MAX
#undef LDBL_MAX
#define FLT_MAX		__FLT_MAX__
#define DBL_MAX		__DBL_MAX__
#define LDBL_MAX	__LDBL_MAX__

#undef FLT_EPSILON
#undef DBL_EPSILON
#undef LDBL_EPSILON
#define FLT_EPSILON	__FLT_EPSILON__
#define DBL_EPSILON	__DBL_EPSILON__
#define LDBL_EPSILON	__LDBL_EPSILON__

#undef FLT_MIN
#undef DBL_MIN
#undef LDBL_MIN
#define FLT_MIN		__FLT_MIN__
#define DBL_MIN		__DBL_MIN__
#define LDBL_MIN	__LDBL_MIN__

#undef FLT_ROUNDS
#define FLT_ROUNDS 1

#endif//_IDROS_FLOAT_H_
