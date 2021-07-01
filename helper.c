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
