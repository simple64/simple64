#pragma once

// endianness
#define LSB_FIRST 1 
#ifdef LSB_FIRST
    #define BYTE_ADDR_XOR       3
    #define WORD_ADDR_XOR       1
    #define BYTE4_XOR_BE(a)     ((a) ^ BYTE_ADDR_XOR)
#else
    #define BYTE_ADDR_XOR       0
    #define WORD_ADDR_XOR       0
    #define BYTE4_XOR_BE(a)     (a)
#endif

#ifdef LSB_FIRST
    #define BYTE_XOR_DWORD_SWAP 7
    #define WORD_XOR_DWORD_SWAP 3
#else
    #define BYTE_XOR_DWORD_SWAP 4
    #define WORD_XOR_DWORD_SWAP 2
#endif

#define DWORD_XOR_DWORD_SWAP 1

// inlining
#define INLINE inline

#ifdef _MSC_VER
#define STRICTINLINE __forceinline
#elif defined(__GNUC__)
#define STRICTINLINE __attribute__((always_inline)) inline
#else
#define STRICTINLINE inline
#endif

// misc
#define UNUSED(x) (void)(x)
