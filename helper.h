#ifndef HELPER_H
#define HELPER_H

#include <stdint.h>
#include <arm_neon.h>

void generateMatrix(int16_t *matrix, int32_t rows, int32_t cols);
void printMatrix(int16_t *matrix, int32_t rows, int32_t cols);
void print16x8(int16x8_t vector);
int16_t sum16x8(int16x8_t vector);

#endif
