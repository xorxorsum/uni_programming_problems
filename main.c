#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Matrix {
  int m;
  int n;
  int** value;
} Matrix;

Matrix CreateDynamicMatrix(const int m, const int n) {
  Matrix matrix;
  matrix.m = m;
  matrix.n = n;
  matrix.value = (int **)malloc(m * sizeof(int*));
  for (int i = 0; i < m; ++i)
    matrix.value[i] = (int*)malloc(n * sizeof(int));
  return matrix;
}

Matrix GenerateRandomMatrix(const int m, const int n, const int maximum) {
  time_t t;
  Matrix matrix = CreateDynamicMatrix(m, n);

  srand(time(NULL));
  for (int i = 0; i < m; ++i)
    for (int j = 0; j < n; ++j) {
      int r = rand() % maximum;
      matrix.value[i][j] = r;
    }
  
  return matrix;
}

Matrix MatrixMultiply(const Matrix a, const Matrix b) {
  Matrix res = CreateDynamicMatrix(b.m, b.n);
  for (int i = 0; i < a.m; ++i) {
    for (int j = 0; j < b.n; ++j) {
      res.value[i][j] = 0;
      for (int k = 0; k < b.m; k++) 
        res.value[i][j] += a.value[i][k] * b.value[k][j];
    }
  }
  return res;
}

Matrix MatrixSum(const Matrix a, const Matrix b) {
  Matrix res = CreateDynamicMatrix(a.m, a.n);
  for (int i = 0; i < a.m; ++i) {
    for (int j = 0; j < b.n; ++j) {
      res.value[i][j] = a.value[i][j] + b.value[i][j];
    }
  }
  return res;
}

Matrix MatrixTransponate(const Matrix a) {
  Matrix res = CreateDynamicMatrix(a.n, a.m);
  for (int i = 0; i < a.m; ++i) {
    for (int j = 0; j < a.n; ++j) {
        res.value[j][i] = a.value[i][j];
    }
  }
  return res;
}

void FillMatrix(Matrix matrix) {
  for (int i = 0; i < matrix.m; ++i)
    for (int j = 0; j < matrix.n; ++j)
      scanf("%d", &matrix.value[i][j]);
}

void FillMatrixFromFile(Matrix matrix, const char* filename) {
  FILE* fp = fopen(filename, "r");
  int num;
  for (int i = 0; i < matrix.m; ++i) {
    for (int j = 0; j < matrix.n; ++j) {
      fscanf(fp, "%d", &num);
      matrix.value[i][j] = num;
    }
  }
}

void PrintMatrix(Matrix matrix) {
  for (int i = 0; i < matrix.m; ++i) {
    for (int j = 0; j < matrix.n; ++j) {
      printf("%d ", matrix.value[i][j]);
    }
    printf("\n");
  }
}

int main(void) {
  int m = 3, maximum = 125, n = 4;
  Matrix mtx = GenerateRandomMatrix(m, n, maximum);
  Matrix mtx2 = MatrixTransponate(mtx);
  // Matrix mtx3 = MatrixMultiply(mtx, mtx2);
  PrintMatrix(mtx);
  PrintMatrix(mtx2);
  // PrintMatrix(mtx3);
  return 0;
}