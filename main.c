#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // int16_t
#include "helper.h"
#include "mult.h"

#define multF 2 // 0 1 or 2
#define doPrint 1
#define m1 7
#define n2 8
#define m2 8

int main() {
  static int16_t matrix1[m1 * n2], matrix2[n2 * m2], product[m1 * m2];

  generateMatrix(matrix1, m1, n2);
  generateMatrix(matrix2, n2, m2);
  //printMatrix(matrix1, m1, n2);
  //printMatrix(matrix2, n2, m2);
  //reset(product, m1, m2);
  if(multF == 0) {
	mult(matrix1, m1, n2, matrix2, n2, m2, product);
  } else if(multF == 1) {
	mult1x8(matrix1, m1, n2, matrix2, n2, m2, product);
  } else {
	mult4x8(matrix1, m1, n2, matrix2, n2, m2, product);
  }
  
  if(doPrint) {
	printMatrix(product, m1, m2);
  }

  if(doPrint) {
	static int16_t product2[m1 * m2];
	mult(matrix1, m1, n2, matrix2, n2, m2, product2);
	if(isEqual(product, m1, m2, product2, m1, m2)) {
	  printf("Matrices are equal.\n");
	} else {
	  printf("\n");
	  printMatrix(product2, m1, m2);
	  printf("Matrcies are not equal.\n");
	}
  }
  return 0;
}
