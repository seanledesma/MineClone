#include "myhash.h"

uint64_t chunk_hash(int cx, int cy, int cz) {
    uint64_t x = (uint64_t)(cx) & 0x1FFFFF;
    uint64_t y = (uint64_t)(cy) & 0x1FFFFF;
    uint64_t z = (uint64_t)(cz) & 0x1FFFFF;
    return (x << 42) | (y << 21) | z;
}
