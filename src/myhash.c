#include "myhash.h"

uint64_t chunk_hash(int cx, int cy, int cz) {
    //changed to handle potential collisions when dealing with negatives;
    //now using twos complement to mitigate negatives error
    //beware: coordinates must be between +-1,048,576 to avoid collision
    uint64_t x = ((uint64_t)(cx + 0x100000)) & 0x1FFFFF;
    uint64_t y = ((uint64_t)(cy + 0x100000)) & 0x1FFFFF;
    uint64_t z = ((uint64_t)(cz + 0x100000)) & 0x1FFFFF;
    return (x << 42) | (y << 21) | z;
}
