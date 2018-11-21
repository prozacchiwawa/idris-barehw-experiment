#ifndef _IDROS_STDARG_H_
#define _IDROS_STDARG_H_

#define _VA_LIST_DEFINED
#define __VALIST __builtin_va_list
#define __va_list __VALIST
typedef __VALIST va_list;

#define va_arg(ap,ty) (*((ty*)ap))
#define va_start(ap, arg) ((void*)&arg)
#define va_end(ap) do { } while(0)

#endif//_IDROS_STDARG_H_
