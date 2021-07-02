#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <arm_neon.h>

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
