#ifndef HELPER_H
#define HELPER_H

#include <stdint.h>
#include <arm_neon.h>

void generateMatrix(int16_t *matrix, int32_t rows, int32_t cols);
void reset(int16_t *matrix, int32_t rows, int32_t cols);
int isEqual(int16_t *matrix1, int32_t rows1, int32_t cols1, int16_t *matrix2, int32_t rows2, int32_t cols2);
void printMatrix(int16_t *matrix, int32_t rows, int32_t cols);
void print16x8(int16x8_t vector);
int16_t sum16x8(int16x8_t vector);

#endif
