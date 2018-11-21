#ifndef _IDROS_LIMITS_H_
#define _IDROS_LIMITS_H_

#define LONG_MIN ((0x7fffffff * -1) - 1)
#define LONG_MAX 0x7fffffff

#define ULONG_MAX ((0x7fffffffu * 2) + 1)

#define LONG_LONG_MIN (((0x7fffffffffffffffll) * -1ll) - 1ll)
#define LONG_LONG_MAX 0x7fffffffffffffffll

#define INT_MAX LONG_MAX
#define INT_MIN LONG_MIN

#define ULONG_LONG_MAX ((0x7fffffffffffffffll * 2ll) + 1ll)

#define CHAR_MIN -128
#define CHAR_MAX 127
#define UCHAR_MAX 255

#define UINT_MAX ULONG_MAX

#define CHAR_BIT 8

#endif//_IDROS_LIMITS_H_
