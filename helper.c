#include <stdint.h>
#include <stdio.h>

void generateMatrix(int16_t *matrix, int32_t rows, int32_t cols) {
  for(int32_t i = 0; i < rows; i++) {
	for(int32_t j = 0; j < cols; j++) {
	  matrix[i * cols + j] = i * cols + j;
	}
  }
}

void printMatrix(int16_t *matrix, int32_t rows, int32_t cols) {
  for(int32_t i = 0; i < rows; i++) {
	for(int32_t j = 0; j < cols; j++) {
	  if(j > 0) printf(", ");
	  printf("%2d", matrix[i * cols + j]);
	}
	printf("\n");
  }
}

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

