#include <stdint.h>
#include <stdio.h>
#include <arm_neon.h>
#include "helper.h"

void mult(int16_t *matrix1, int32_t rows1, int32_t cols1,
		  int16_t *matrix2, int32_t rows2, int32_t cols2,
		  int16_t *output_matrix) {
  if(cols1 != rows2) {
	fprintf(stderr, "Matrix dimensions are not aligned");
	return;
  }

  /*
	indices i and j fix rows for matrix1 and columns for matrix2
	since column size of matrix1 is equal to row size of matrix2
	the index k iterates over unfixed portion of matrix1 and matrix2 indices
   */

  for(int32_t i = 0; i < rows1; i++) {
	for(int32_t j = 0; j < cols2; j++) {
	  int16_t sumProducts = 0;
	  for(int32_t k = 0; k < rows2; k++) {
		// print matrix point as two indices and as an nth element of matrix
		printf("([%d][%d] = %d x [%d][%d] = %d), ", i, k, i * cols1 + k, k, j, k * cols2 + j);

		// print actual numbers
		//printf("%d x %d, ", matrix1[i * cols1 + k], matrix2[k * cols2 + j]);

		// [i][k] => [i * cols1 + k]
		// [k][j] => [k * cols2 + j]
		sumProducts += matrix1[i * cols1 + k] * matrix2[k * cols2 + j];
	  }
	  // print computed sum
	  printf(" => %d\n", sumProducts);
	  output_matrix[i * cols2 + j] = sumProducts;
	}
  }
}

void mult8x1(int16_t *matrix1, int32_t rows1, int32_t cols1,
		  int16_t *matrix2, int32_t rows2, int32_t cols2,
		  int16_t *output_matrix) {
  if(cols1 != rows2) {
	fprintf(stderr, "Matrix dimensions are not aligned");
	return;
  }

  for(int i = 0; i < rows1; i++) {
	for(int j = 0; j < cols2; j++) {
	  // Reset row accumulator values to 0
	  int16x8_t ACC = vmovq_n_s16(0);

	  int k = 0;
	  for(; k < cols1 - 7; k += 8) {
		// Load matrix1[i][k] to matrix1[i][k + 7]
		int16x8_t A = vld1q_s16(matrix1 + i * cols1 + k);

		// Get column values from matrix2
		int16_t alignB[8];
		for(int l = k; l < 8 + k; l++) {
		  alignB[l] = matrix2[l * cols2 + j];
		}
		int16x8_t B = vld1q_s16(alignB);

		// Load matrix2[k][j] to matrix2[k + 7][j]
		ACC = vaddq_s16(ACC, vmulq_s16(A, B));
	  }

	  // Sum left over products, if matrix dimenion is not multiple of 8
	  int16_t leftover_sum = 0;
	  for(; k < cols1; k++) {
		leftover_sum += matrix1[i * cols1 + k] * matrix2[k * cols2 + j];
	  }

	  // Write to output matrix cell
	  output_matrix[i * cols2 + j] = sum16x8(ACC) + leftover_sum;
	}
  }
}
