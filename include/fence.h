#pragma once

#define INLINE __attribute__((always_inline)) inline

INLINE void compilerFence() { asm volatile("":: : "memory"); }