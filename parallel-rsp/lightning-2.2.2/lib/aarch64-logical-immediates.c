// AArch64 Logical Immediate Encoding and Decoding
//
// I hereby place this code in the public domain, as per the terms of the
// CC0 license: https://creativecommons.org/publicdomain/zero/1.0/

#include <stdint.h>
#include <stdbool.h>

static inline int nonzeroCountTrailingZeros64(uint64_t n) {
    return __builtin_ctzll(n);
}

static inline int countTrailingZeros64(uint64_t n) {
    return n ? nonzeroCountTrailingZeros64(n) : 64;
}

static inline int nonzeroCountLeadingZeros64(uint64_t n) {
    return __builtin_clzll(n);
}

static inline int nonzeroCountLeadingZeros32(uint32_t n) {
    return __builtin_clz(n);
}

static inline uint64_t rotateRight64(uint64_t v, int n) {
    // return __builtin_rotateright64(v, n);
    return (v >> (n & 63)) | (v << (-n & 63));
}

static inline uint64_t clearTrailingOnes64(uint64_t n) {
    return n & (n + 1);
}

#define ENCODE_FAILED (-1)

int encodeLogicalImmediate64(uint64_t val) {
    // Consider an ARM64 logical immediate as a pattern of "o" ones preceded
    // by "z" more-significant zeroes, repeated to fill a 64-bit integer.
    // o > 0, z > 0, and the size (o + z) is a power of two in [2,64]. This
    // part of the pattern is encoded in the fields "imms" and "N".
    //
    // "immr" encodes a further right rotate of the repeated pattern, allowing
    // a wide range of useful bitwise constants to be represented.
    //
    // (The spec describes the "immr" rotate as rotating the "o + z" bit
    // pattern before repeating it to fill 64-bits, but, as it's a repeating
    // pattern, rotating afterwards is equivalent.)

    // This encoding is not allowed to represent all-zero or all-one values.
    if (val == 0 || ~val == 0)
        return ENCODE_FAILED;

    // To detect an immediate that may be encoded in this scheme, we first
    // remove the right-rotate, by rotating such that the least significant
    // bit is a one and the most significant bit is a zero.
    //
    // We do this by clearing any trailing one bits, then counting the
    // trailing zeroes. This finds an "edge", where zero goes to one.
    // We then rotate the original value right by that amount, moving
    // the first one to the least significant bit.

    int rotation = countTrailingZeros64(clearTrailingOnes64(val));
    uint64_t normalized = rotateRight64(val, rotation & 63);

    // Now we have normalized the value, and determined the rotation, we can
    // determine "z" by counting the leading zeroes, and "o" by counting the
    // trailing ones. (These will both be positive, as we already rejected 0
    // and ~0, and rotated the value to start with a zero and end with a one.)

    int zeroes = nonzeroCountLeadingZeros64(normalized);
    int ones = nonzeroCountTrailingZeros64(~normalized);
    int size = zeroes + ones;

    // Detect the repeating pattern (by comparing every repetition to the
    // one next to it, using rotate).

    if (rotateRight64(val, size & 63) != val)
        return ENCODE_FAILED;

    // We do not need to further validate size to ensure it is a power of two
    // between 2 and 64. The only "minimal" patterns that can repeat to fill a
    // 64-bit value must have a length that is a factor of 64 (i.e. it is a
    // power of two in the range [1,64]). And our pattern cannot be of length
    // one (as we already rejected 0 and ~0).
    //
    // By "minimal" patterns I refer to patterns which do not themselves
    // contain repetitions. For example, '010101' is a non-minimal pattern of
    // a non-power-of-two length that can pass the above rotational test. It
    // consists of the minimal pattern '01'. All our patterns are minimal, as
    // they contain only one contiguous run of ones separated by at least one
    // zero.

    // Finally, we encode the values. "rotation" is the amount we rotated
    // right by to "undo" the right-rotate encoded in immr, so must be
    // negated.

    // size 2:  N=0 immr=00000r imms=11110s
    // size 4:  N=0 immr=0000rr imms=1110ss
    // size 8:  N=0 immr=000rrr imms=110sss
    // size 16: N=0 immr=00rrrr imms=10ssss
    // size 32: N=0 immr=0rrrrr imms=0sssss
    // size 64: N=1 immr=rrrrrr imms=ssssss
    int immr = -rotation & (size - 1);
    int imms = -(size << 1) | (ones - 1);
    int N = (size >> 6);

    return (N << 12) | (immr << 6) | (imms & 0x3f);
}

int encodeLogicalImmediate32(uint32_t val) {
    return encodeLogicalImmediate64(((uint64_t)val << 32) | val);
}

// Decoding!

bool isValidLogicalImmediate64(unsigned val) {
    unsigned N = (val >> 12) & 1;
    unsigned imms = val & 0x3f;
    unsigned pattern = (N << 6) | (~imms & 0x3f);
    return (pattern & (pattern - 1)) != 0;
}

bool isValidLogicalImmediate32(unsigned val) {
    unsigned N = (val >> 12) & 1;
    return N == 0 && isValidLogicalImmediate64(val);
}

#define DECODE_FAILED 0

// returns DECODE_FAILED (zero) if the encoding is invalid
uint64_t decodeLogicalImmediate64(unsigned val) {
    // Fun way to generate the immediates with mask ^ (mask << S)
    static const uint64_t mask_lookup[] = {
        0xffffffffffffffff, // size = 64
        0x00000000ffffffff, // size = 32
        0x0000ffff0000ffff, // size = 16
        0x00ff00ff00ff00ff, // size = 8
        0x0f0f0f0f0f0f0f0f, // size = 4
        0x3333333333333333, // size = 2
    };

    unsigned N = (val >> 12) & 1;
    int immr = (val >> 6) & 0x3f;
    unsigned imms = val & 0x3f;

    unsigned pattern = (N << 6) | (~imms & 0x3f);

    if (!(pattern & (pattern - 1))) return DECODE_FAILED;

    int leading_zeroes = nonzeroCountLeadingZeros32(pattern);
    unsigned imms_mask = 0x7fffffff >> leading_zeroes;
    uint64_t mask = mask_lookup[leading_zeroes - 25];
    unsigned S = (imms + 1) & imms_mask;
    return rotateRight64(mask ^ (mask << S), immr);
}

uint32_t decodeLogicalImmediate32(unsigned val) {
    unsigned N = (val >> 12) & 1;
    if (N) return DECODE_FAILED;
    return (uint32_t)decodeLogicalImmediate64(val);
}
