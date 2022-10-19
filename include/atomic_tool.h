#pragma once

#include "common.h"

#define INLINE __attribute__((always_inline)) inline

INLINE uint64_t atomicLoadGE();

INLINE void atomicAddGE() {
    uint64_t expected, desired;
    expected = atomicLoadGE();
    for (;;) {
        desired = expected + 1;
        if (__atomic_compare_exchange_n(&(GlobalEpoch.obj_), &expected, desired, false, __ATOMIC_ACQ_REL, __ATOMIC_ACQ_REL)) break;
    }
}

INLINE uint64_t atomicLoadGE() {
    uint64_t_64byte result = __atomic_load_n(&(GlobalEpoch.obj_), __ATOMIC_ACQUIRE);
    return result.obj_;
}