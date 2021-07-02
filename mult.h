#ifndef MULT_H
#define MULT_H

#include <stdint.h>

void mult(int16_t *matrix1, int32_t rows1, int32_t cols1,
		  int16_t *matrix2, int32_t rows2, int32_t cols2,
		  int16_t *output_matrix);

void mult1x8(int16_t *matrix1, int32_t rows1, int32_t cols1,
		  int16_t *matrix2, int32_t rows2, int32_t cols2,
		  int16_t *output_matrix);

void mult4x8(int16_t *matrix1, int32_t rows1, int32_t cols1,
		  int16_t *matrix2, int32_t rows2, int32_t cols2,
		  int16_t *output_matrix);

void columnToContinuous(int16_t* matrix, int32_t k, int32_t j, int32_t cols, int16_t *output);

#endif
