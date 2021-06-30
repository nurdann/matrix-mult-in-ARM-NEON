#ifndef HELPER_H
#define HELPER_H

void generateMatrix(int16_t *matrix, int32_t rows, int32_t cols);
void printMatrix(int16_t *matrix, int32_t rows, int32_t cols);
void mult(int16_t *matrix1, int32_t rows1, int32_t cols1,
		  int16_t *matrix2, int32_t rows2, int32_t cols2,
		  int16_t *output_matrix);

#endif
