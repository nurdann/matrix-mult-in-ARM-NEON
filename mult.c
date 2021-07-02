#include <stdint.h>
#include <stdio.h>
#include <arm_neon.h>
#include "helper.h"
#include "mult.h"

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
		//printf("([%d][%d] = %d x [%d][%d] = %d), ", i, k, i * cols1 + k, k, j, k * cols2 + j);

		// print actual numbers
		//printf("%d x %d, ", matrix1[i * cols1 + k], matrix2[k * cols2 + j]);

		// [i][k] => [i * cols1 + k]
		// [k][j] => [k * cols2 + j]
		sumProducts += matrix1[i * cols1 + k] * matrix2[k * cols2 + j];
	  }
	  // print computed sum
	  //printf(" => %d\n", sumProducts);
	  output_matrix[i * cols2 + j] = sumProducts;
	}
  }
}

void mult1x8(int16_t *matrix1, int32_t rows1, int32_t cols1,
		  int16_t *matrix2, int32_t rows2, int32_t cols2,
		  int16_t *output_matrix) {
  if(cols1 != rows2) {
	fprintf(stderr, "Matrix dimensions are not aligned");
	return;
  }

  for(int32_t i = 0; i < rows1; i++) {
	for(int32_t j = 0; j < cols2; j++) {
	  // Reset row accumulator values to 0
	  int16x8_t ACC = vmovq_n_s16(0);

	  int32_t k = 0;
	  for(; k < cols1 - 7; k += 8) {
		// Load matrix1[i][k] to matrix1[i][k + 7]
		int16x8_t A = vld1q_s16(matrix1 + i * cols1 + k);

		// Get column values from matrix2
		int16_t alignB[8];
		for(int l = 0; l < 8; l++) {
		  alignB[l] = matrix2[k * cols2 + l * cols2 + j];
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

// Used as guide https://developer.arm.com/documentation/102467/0100/Matrix-multiplication-example
void mult4x8(int16_t *matrix1, int32_t rows1, int32_t cols1,
		  int16_t *matrix2, int32_t rows2, int32_t cols2,
		  int16_t *output_matrix) {
  if(cols1 != rows2) {
	fprintf(stderr, "Matrix dimensions are not aligned");
	return;
  }
  
  int32_t i = 0;
  for(; i + 3 < rows1; i += 4) {
	int32_t j = 0;
	for(; j + 7 < cols2; j += 8) {
	  int16x8_t R00 = vmovq_n_s16(0);
	  int16x8_t R01 = vmovq_n_s16(0);
	  int16x8_t R02 = vmovq_n_s16(0);
	  int16x8_t R03 = vmovq_n_s16(0);
	  int16x8_t R04 = vmovq_n_s16(0);
	  int16x8_t R05 = vmovq_n_s16(0);
	  int16x8_t R06 = vmovq_n_s16(0);
	  int16x8_t R07 = vmovq_n_s16(0);

	  int16x8_t R10 = vmovq_n_s16(0);
	  int16x8_t R11 = vmovq_n_s16(0);
	  int16x8_t R12 = vmovq_n_s16(0);
	  int16x8_t R13 = vmovq_n_s16(0);
	  int16x8_t R14 = vmovq_n_s16(0);
	  int16x8_t R15 = vmovq_n_s16(0);
	  int16x8_t R16 = vmovq_n_s16(0);
	  int16x8_t R17 = vmovq_n_s16(0);

	  int16x8_t R20 = vmovq_n_s16(0);
	  int16x8_t R21 = vmovq_n_s16(0);
	  int16x8_t R22 = vmovq_n_s16(0);
	  int16x8_t R23 = vmovq_n_s16(0);
	  int16x8_t R24 = vmovq_n_s16(0);
	  int16x8_t R25 = vmovq_n_s16(0);
	  int16x8_t R26 = vmovq_n_s16(0);
	  int16x8_t R27 = vmovq_n_s16(0);

	  int16x8_t R30 = vmovq_n_s16(0);
	  int16x8_t R31 = vmovq_n_s16(0);
	  int16x8_t R32 = vmovq_n_s16(0);
	  int16x8_t R33 = vmovq_n_s16(0);
	  int16x8_t R34 = vmovq_n_s16(0);
	  int16x8_t R35 = vmovq_n_s16(0);
	  int16x8_t R36 = vmovq_n_s16(0);
	  int16x8_t R37 = vmovq_n_s16(0);

	  int32_t k = 0;
	  for(; k + 7 < cols1; k += 8) {
		// Load 8x8 submatrix from matrix1
		int16x8_t A0 = vld1q_s16(matrix1 + i * cols1 + k);
		int16x8_t A1 = vld1q_s16(matrix1 + i * cols1 + cols1 + k);
		int16x8_t A2 = vld1q_s16(matrix1 + i * cols1 + cols1 * 2 + k);
		int16x8_t A3 = vld1q_s16(matrix1 + i * cols1 + cols1 * 3 + k);
		int16x8_t A4 = vld1q_s16(matrix1 + i * cols1 + cols1 * 4 + k);
		int16x8_t A5 = vld1q_s16(matrix1 + i * cols1 + cols1 * 5 + k);
		int16x8_t A6 = vld1q_s16(matrix1 + i * cols1 + cols1 * 6 + k);
		int16x8_t A7 = vld1q_s16(matrix1 + i * cols1 + cols1 * 7 + k);
		
		// Align column values continuosly,
		// then load 8x8 submatrix from matrix2
		int16_t alignColumn[8];
		columnToContinuous(matrix2, k, j, cols2, alignColumn);
		int16x8_t B0 = vld1q_s16(alignColumn);
		columnToContinuous(matrix2, k, j + 1, cols2, alignColumn);
		int16x8_t B1 = vld1q_s16(alignColumn);
		columnToContinuous(matrix2, k, j + 2, cols2, alignColumn);
		int16x8_t B2 = vld1q_s16(alignColumn);
		columnToContinuous(matrix2, k, j + 3, cols2, alignColumn);
		int16x8_t B3 = vld1q_s16(alignColumn);
		columnToContinuous(matrix2, k, j + 4, cols2, alignColumn);
		int16x8_t B4 = vld1q_s16(alignColumn);
		columnToContinuous(matrix2, k, j + 5, cols2, alignColumn);
		int16x8_t B5 = vld1q_s16(alignColumn);
		columnToContinuous(matrix2, k, j + 6, cols2, alignColumn);
		int16x8_t B6 = vld1q_s16(alignColumn);
		columnToContinuous(matrix2, k, j + 7, cols2, alignColumn);
		int16x8_t B7 = vld1q_s16(alignColumn);

		// Multiply and add
		R00 = vaddq_s16(R00, vmulq_s16(A0, B0));
		R01 = vaddq_s16(R01, vmulq_s16(A0, B1));
		R02 = vaddq_s16(R02, vmulq_s16(A0, B2));
		R03 = vaddq_s16(R03, vmulq_s16(A0, B3));
		R04 = vaddq_s16(R04, vmulq_s16(A0, B4));
		R05 = vaddq_s16(R05, vmulq_s16(A0, B5));
		R06 = vaddq_s16(R06, vmulq_s16(A0, B6));
		R07 = vaddq_s16(R07, vmulq_s16(A0, B7));

		R10 = vaddq_s16(R10, vmulq_s16(A1, B0));
		R11 = vaddq_s16(R11, vmulq_s16(A1, B1));
		R12 = vaddq_s16(R12, vmulq_s16(A1, B2));
		R13 = vaddq_s16(R13, vmulq_s16(A1, B3));
		R14 = vaddq_s16(R14, vmulq_s16(A1, B4));
		R15 = vaddq_s16(R15, vmulq_s16(A1, B5));
		R16 = vaddq_s16(R16, vmulq_s16(A1, B6));
		R17 = vaddq_s16(R17, vmulq_s16(A1, B7));

		R20 = vaddq_s16(R20, vmulq_s16(A2, B0));
		R21 = vaddq_s16(R21, vmulq_s16(A2, B1));
		R22 = vaddq_s16(R22, vmulq_s16(A2, B2));
		R23 = vaddq_s16(R23, vmulq_s16(A2, B3));
		R24 = vaddq_s16(R24, vmulq_s16(A2, B4));
		R25 = vaddq_s16(R25, vmulq_s16(A2, B5));
		R26 = vaddq_s16(R26, vmulq_s16(A2, B6));
		R27 = vaddq_s16(R27, vmulq_s16(A2, B7));

		R30 = vaddq_s16(R30, vmulq_s16(A3, B0));
		R31 = vaddq_s16(R31, vmulq_s16(A3, B1));
		R32 = vaddq_s16(R32, vmulq_s16(A3, B2));
		R33 = vaddq_s16(R33, vmulq_s16(A3, B3));
		R34 = vaddq_s16(R34, vmulq_s16(A3, B4));
		R35 = vaddq_s16(R35, vmulq_s16(A3, B5));
		R36 = vaddq_s16(R36, vmulq_s16(A3, B6));
		R37 = vaddq_s16(R37, vmulq_s16(A3, B7));
	  }

	  // Sum accumulated sum products
	  int16_t L00 = sum16x8(R00);
	  int16_t L01 = sum16x8(R01);
	  int16_t L02 = sum16x8(R02);
	  int16_t L03 = sum16x8(R03);
	  int16_t L04 = sum16x8(R04);
	  int16_t L05 = sum16x8(R05);
	  int16_t L06 = sum16x8(R06);
	  int16_t L07 = sum16x8(R07);

	  int16_t L10 = sum16x8(R10);
	  int16_t L11 = sum16x8(R11);
	  int16_t L12 = sum16x8(R12);
	  int16_t L13 = sum16x8(R13);
	  int16_t L14 = sum16x8(R14);
	  int16_t L15 = sum16x8(R15);
	  int16_t L16 = sum16x8(R16);
	  int16_t L17 = sum16x8(R17);

	  int16_t L20 = sum16x8(R20);
	  int16_t L21 = sum16x8(R21);
	  int16_t L22 = sum16x8(R22);
	  int16_t L23 = sum16x8(R23);
	  int16_t L24 = sum16x8(R24);
	  int16_t L25 = sum16x8(R25);
	  int16_t L26 = sum16x8(R26);
	  int16_t L27 = sum16x8(R27);

	  int16_t L30 = sum16x8(R30);
	  int16_t L31 = sum16x8(R31);
	  int16_t L32 = sum16x8(R32);
	  int16_t L33 = sum16x8(R33);
	  int16_t L34 = sum16x8(R34);
	  int16_t L35 = sum16x8(R35);
	  int16_t L36 = sum16x8(R36);
	  int16_t L37 = sum16x8(R37);

	  // Compute left over
	  for(; k < cols1; k++) {
		L00 += matrix1[i * cols1 + k] * matrix2[k * cols2 + j];
		L01 += matrix1[i * cols1 + k] * matrix2[k * cols2 + j + 1];
		L02 += matrix1[i * cols1 + k] * matrix2[k * cols2 + j + 2];
		L03 += matrix1[i * cols1 + k] * matrix2[k * cols2 + j + 3];
		L04 += matrix1[i * cols1 + k] * matrix2[k * cols2 + j + 4];
		L05 += matrix1[i * cols1 + k] * matrix2[k * cols2 + j + 5];
		L06 += matrix1[i * cols1 + k] * matrix2[k * cols2 + j + 6];
		L07 += matrix1[i * cols1 + k] * matrix2[k * cols2 + j + 7];

		L10 += matrix1[i * cols1 + cols1 + k] * matrix2[k * cols2 + j];
		L11 += matrix1[i * cols1 + cols1 + k] * matrix2[k * cols2 + j + 1];
		L12 += matrix1[i * cols1 + cols1 + k] * matrix2[k * cols2 + j + 2];
		L13 += matrix1[i * cols1 + cols1 + k] * matrix2[k * cols2 + j + 3];
		L14 += matrix1[i * cols1 + cols1 + k] * matrix2[k * cols2 + j + 4];
		L15 += matrix1[i * cols1 + cols1 + k] * matrix2[k * cols2 + j + 5];
		L16 += matrix1[i * cols1 + cols1 + k] * matrix2[k * cols2 + j + 6];
		L17 += matrix1[i * cols1 + cols1 + k] * matrix2[k * cols2 + j + 7];
		
		L20 += matrix1[i * cols1 + cols1 * 2 + k] * matrix2[k * cols2 + j];
		L21 += matrix1[i * cols1 + cols1 * 2 + k] * matrix2[k * cols2 + j + 1];
		L22 += matrix1[i * cols1 + cols1 * 2 + k] * matrix2[k * cols2 + j + 2];
		L23 += matrix1[i * cols1 + cols1 * 2 + k] * matrix2[k * cols2 + j + 3];
		L24 += matrix1[i * cols1 + cols1 * 2 + k] * matrix2[k * cols2 + j + 4];
		L25 += matrix1[i * cols1 + cols1 * 2 + k] * matrix2[k * cols2 + j + 5];
		L26 += matrix1[i * cols1 + cols1 * 2 + k] * matrix2[k * cols2 + j + 6];
		L27 += matrix1[i * cols1 + cols1 * 2 + k] * matrix2[k * cols2 + j + 7];
		
		L30 += matrix1[i * cols1 + cols1 * 3 + k] * matrix2[k * cols2 + j];
		L31 += matrix1[i * cols1 + cols1 * 3 + k] * matrix2[k * cols2 + j + 1];
		L32 += matrix1[i * cols1 + cols1 * 3 + k] * matrix2[k * cols2 + j + 2];
		L33 += matrix1[i * cols1 + cols1 * 3 + k] * matrix2[k * cols2 + j + 3];
		L34 += matrix1[i * cols1 + cols1 * 3 + k] * matrix2[k * cols2 + j + 4];
		L35 += matrix1[i * cols1 + cols1 * 3 + k] * matrix2[k * cols2 + j + 5];
		L36 += matrix1[i * cols1 + cols1 * 3 + k] * matrix2[k * cols2 + j + 6];
		L37 += matrix1[i * cols1 + cols1 * 3 + k] * matrix2[k * cols2 + j + 7];
	  }

 	  output_matrix[i * cols2 + j] = L00;
	  output_matrix[i * cols2 + j + 1] = L01;
	  output_matrix[i * cols2 + j + 2] = L02;
	  output_matrix[i * cols2 + j + 3] = L03;
	  output_matrix[i * cols2 + j + 4] = L04;
	  output_matrix[i * cols2 + j + 5] = L05;
	  output_matrix[i * cols2 + j + 6] = L06;
	  output_matrix[i * cols2 + j + 7] = L07;
	  
 	  output_matrix[i * cols2 + cols2 + j] = L10;
	  output_matrix[i * cols2 + cols2 + j + 1] = L11;
	  output_matrix[i * cols2 + cols2 + j + 2] = L12;
	  output_matrix[i * cols2 + cols2 + j + 3] = L13;
	  output_matrix[i * cols2 + cols2 + j + 4] = L14;
	  output_matrix[i * cols2 + cols2 + j + 5] = L15;
	  output_matrix[i * cols2 + cols2 + j + 6] = L16;
	  output_matrix[i * cols2 + cols2 + j + 7] = L17;
	  
 	  output_matrix[i * cols2 + cols2 * 2 + j] = L20;
	  output_matrix[i * cols2 + cols2 * 2 + j + 1] = L21;
	  output_matrix[i * cols2 + cols2 * 2 + j + 2] = L22;
	  output_matrix[i * cols2 + cols2 * 2 + j + 3] = L23;
	  output_matrix[i * cols2 + cols2 * 2 + j + 4] = L24;
	  output_matrix[i * cols2 + cols2 * 2 + j + 5] = L25;
	  output_matrix[i * cols2 + cols2 * 2 + j + 6] = L26;
	  output_matrix[i * cols2 + cols2 * 2 + j + 7] = L27;
	  
 	  output_matrix[i * cols2 + cols2 * 3 + j] = L30;
	  output_matrix[i * cols2 + cols2 * 3 + j + 1] = L31;
	  output_matrix[i * cols2 + cols2 * 3 + j + 2] = L32;
	  output_matrix[i * cols2 + cols2 * 3 + j + 3] = L33;
	  output_matrix[i * cols2 + cols2 * 3 + j + 4] = L34;
	  output_matrix[i * cols2 + cols2 * 3 + j + 5] = L35;
	  output_matrix[i * cols2 + cols2 * 3 + j + 6] = L36;
	  output_matrix[i * cols2 + cols2 * 3 + j + 7] = L37;
	}

	// Compute left over columns that are not multiple of 8
	// Assume 4-rows exists
	for(; j < cols2; j++) {
	  int16_t R0 = 0;
	  int16_t R1 = 0;
	  int16_t R2 = 0;
	  int16_t R3 = 0;
	  for(int32_t k = 0; k < cols1; k++) {
		R0 += matrix1[i * cols1 + k] * matrix2[k * cols2 + j];
		R1 += matrix1[i * cols1 + cols1 + k] * matrix2[k * cols2 + j];
		R2 += matrix1[i * cols1 + cols1 * 2 + k] * matrix2[k * cols2 + j];
		R3 += matrix1[i * cols1 + cols1 * 3 + k] * matrix2[k * cols2 + j];
	  }

	  output_matrix[i * cols2 + j] = R0;
	  output_matrix[i * cols2 + cols2 + j] = R1;
	  output_matrix[i * cols2 + cols2 * 2 + j] = R2;
	  output_matrix[i * cols2 + cols2 * 3 + j] = R3;
	}
  }

  // Compute left over rows that are not multiple of 4
  for(; i < rows1; i++) {
	for(int32_t j = 0; j < cols2; j++) {
	  int16x8_t ACC = vmovq_n_s16(0);
	  int32_t k = 0;
	  for(; k + 7 < cols1; k += 8) {
		int16x8_t A = vld1q_s16(matrix1 + i * cols1 + k);

		int16_t alignCol[8];
		columnToContinuous(matrix2, k, j, cols2, alignCol);
		int16x8_t B = vld1q_s16(alignCol);
		
		ACC = vaddq_s16(ACC, vmulq_s16(A, B));
	  }

	  int16_t sumProd = sum16x8(ACC);
	  for(; k + 7 < cols1; k += 8) {
		sumProd +=  matrix1[i * cols1 + k] * matrix2[k * cols2 + j];
	  }
	  
	  output_matrix[i * cols2 + j] = sumProd;

	}
  }

}

void columnToContinuous(int16_t* matrix, int32_t k, int32_t j, int32_t cols, int16_t *output) {
  for(int l = 0; l < 8; l++) {
	output[l] = matrix[k * cols + l*cols + j];
  }
}
