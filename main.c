#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // int16_t
#include "helper.h"

#define m1 3
#define n2 4
#define m2 3

int main() {
  static int16_t matrix1[m1 * n2], matrix2[n2 * m2], product[m1 * m2];

  generateMatrix(matrix1, m1, n2);
  generateMatrix(matrix2, n2, m2);
  //printMatrix(matrix1, m1, n2);
  //printMatrix(matrix2, n2, m2);
  mult(matrix1, m1, n2, matrix2, n2, m2, product);
  printMatrix(product, m1, m2);
  return 0;
}
