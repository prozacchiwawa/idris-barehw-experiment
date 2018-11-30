#ifndef _BOOTTYPES_H_
#define _BOOTTYPES_H_

#define NULL ((void*)0)

typedef unsigned int size_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;

typedef uint16_t Elf64_Half;
typedef uint32_t Elf64_Word;
typedef uint64_t Elf64_Addr;
typedef int64_t Elf64_Off;

#endif//_BOOTTYPES_H_
