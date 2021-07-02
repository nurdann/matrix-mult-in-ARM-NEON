#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <arm_neon.h>
#include "helper.h"

void generateMatrix(int16_t *matrix, int32_t rows, int32_t cols) {
  for(int32_t i = 0; i < rows; i++) {
	for(int32_t j = 0; j < cols; j++) {
	  matrix[i * cols + j] = i * cols + j;
	}
  }
}

void reset(int16_t *matrix, int32_t rows, int32_t cols) {
  for(int32_t i = 0; i < rows; i++) {
	for(int32_t j = 0; j < cols; j++) {
	  matrix[i * cols + j] = 0;
	}
  }
}

int isEqual(int16_t *matrix1, int32_t rows1, int32_t cols1, int16_t *matrix2, int32_t rows2, int32_t cols2) {
  if(rows1 != rows2 || cols1 != cols2) return 0;

  for(int32_t i = 0; i < rows1; i++) {
	for(int32_t j = 0; j < cols1; j++) {
	  if(matrix1[i * cols1 + j] != matrix2[i * cols1 + j]) {
		return 0;
	  }
	}
  }

  return 1;
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
void print16x8(int16x8_t vector) {
  int16_t v[8];
  vst1q_s16(v, vector);

  for(int i = 0; i < 8; i++) {
	if(i) printf(", ");
	printf("%d", v[i]);
  }
  printf("\n");
}

int16_t sum16x8(int16x8_t vector) {
  int16x4_t quaterSum = vadd_s16(vget_high_s16(vector), vget_low_s16(vector));
  int32x2_t halfSum = vpaddl_s16(quaterSum);
  return vget_lane_s32(halfSum, 0) + vget_lane_s32(halfSum, 1);
}
