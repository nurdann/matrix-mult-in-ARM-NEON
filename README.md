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
42, 48, 54
114, 136, 158
186, 224, 262
```

## 1x8 multiplications (mult1x8)

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

Since we are using flat indexing, `M[i][j]` turns to `M[i * number_of_columns + j]` so that we skip all previous row values.
```c
int16x8_t A = vld1q_s16(matrix1 + i * cols1 + k);
```

Additionally, we need to extract columns values before forming ARM NEON vector,

```
int16_t alignB[8];
for(int l = 0; l < 8; l++) {
	alignB[l] = matrix2[k * cols2 + l * cols2 + j];
}
int16x8_t B = vld1q_s16(alignB);
```

## 4x8 multiplications (mult4x8)

To multiply 8 items 4 row values at a time, essentially performing 4x8 submatrix calculations, we need to create 32 (from `4*8`) `16x8_t` variables that hold row/column calculations for the resulting matrix.

## Benchmarking

|Function\nxn| 500 | 1000 | 5000 |
|:---        |:---    |:--     |:--- |
|mult        | 0.996s | 8.562s | 35m57.211s |
|mult1x8     | 1.401s | 11.233s | 37m9.711s |
|mult4x8     | 0.748s | 5.948s | 15m9.300s  |

Where `Function` indicates function name used, and `nxn` is the matrix dimension.

There is no substantial difference between `mult` and `mult1x8` likely because of `mult1x8` the 8 multiplications at time does not compensate for the overhead aligning data. However for `mult4x8`, there are substantial gains by performing `4x8` submatrix multiplications, which could be even faster on ARM NEON devices as opposed to virtual environment.
