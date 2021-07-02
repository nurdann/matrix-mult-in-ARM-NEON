# Matrix multiplication using ARM NEON intrinsics

## Base implemention (mult)

The product of `AxB` has first dimesion of `A` and second dimension of `B`, so in our case the indices `i` and `j` work as entries for a new matrix product. Then the shared index `k` iterates over a row in `A` and column in `B`.

```c
  for(int32_t i = 0; i < rows1; i++) {
	for(int32_t j = 0; j < cols2; j++) {
	  int16_t sumProducts = 0;
	  for(int32_t k = 0; k < rows2; k++) {
		sumProducts += matrix1[i * cols1 + k] * matrix2[k * cols2 + j];
	  }
	  output_matrix[i * cols2 + j] = sumProducts;
	}
  }
```

```
$ gcc -o x main.c helper.c mult.c
$ ./x
([0][0] = 0 x [0][0] = 0), ([0][1] = 1 x [1][0] = 3), ([0][2] = 2 x [2][0] = 6), ([0][3] = 3 x [3][0] = 9),  => 42
([0][0] = 0 x [0][1] = 1), ([0][1] = 1 x [1][1] = 4), ([0][2] = 2 x [2][1] = 7), ([0][3] = 3 x [3][1] = 10),  => 48
([0][0] = 0 x [0][2] = 2), ([0][1] = 1 x [1][2] = 5), ([0][2] = 2 x [2][2] = 8), ([0][3] = 3 x [3][2] = 11),  => 54
([1][0] = 4 x [0][0] = 0), ([1][1] = 5 x [1][0] = 3), ([1][2] = 6 x [2][0] = 6), ([1][3] = 7 x [3][0] = 9),  => 114
([1][0] = 4 x [0][1] = 1), ([1][1] = 5 x [1][1] = 4), ([1][2] = 6 x [2][1] = 7), ([1][3] = 7 x [3][1] = 10),  => 136
([1][0] = 4 x [0][2] = 2), ([1][1] = 5 x [1][2] = 5), ([1][2] = 6 x [2][2] = 8), ([1][3] = 7 x [3][2] = 11),  => 158
([2][0] = 8 x [0][0] = 0), ([2][1] = 9 x [1][0] = 3), ([2][2] = 10 x [2][0] = 6), ([2][3] = 11 x [3][0] = 9),  => 186
([2][0] = 8 x [0][1] = 1), ([2][1] = 9 x [1][1] = 4), ([2][2] = 10 x [2][1] = 7), ([2][3] = 11 x [3][1] = 10),  => 224
([2][0] = 8 x [0][2] = 2), ([2][1] = 9 x [1][2] = 5), ([2][2] = 10 x [2][2] = 8), ([2][3] = 11 x [3][2] = 11),  => 262
42, 48, 54
114, 136, 158
186, 224, 262
```

## 8x1 multiplications (mult8x1)

Install ARM compiler,

```
sudo apt install gcc-arm-linux-gnueabi
```

Install `qemu` to run ARM executable on x86 architechture,

```
sudo apt install binfmt-support qemu qemu-user-static
```
source: https://askubuntu.com/a/842271

Compile and run the executable

```
$ /usr/bin/arm-linux-gnueabi-gcc -mfpu=neon -mcpu=cortex-a9 -mfloat-abi=softfp main.c -static -o x
```


We can multiply 8 elements at a time using `16x8_t` type that holds 8 16-bit integers. For that we need align 8 elements continuosly `int16_t v[8]` and load as a `16x8_t` vector.
