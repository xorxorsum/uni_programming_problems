#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/foo.h"

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
    for (int j = 0; j < m.n; ++j) {
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

Matrix MatrixDiff(const Matrix a, const Matrix b) {
  Matrix res = CreateDynamicMatrix(a.m, a.n);
  for (int i = 0; i < a.m; ++i) {
    for (int j = 0; j < b.n; ++j) {
      res.value[i][j] = a.value[i][j] - b.value[i][j];
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

Matrix InvertMatrix(Matrix m) {
    double pivot, tmp;
    Matrix inverted = {0};

    for (int i = 0; i < m.n; i++) {
        inverted.value[i][i] = 1.0;
    }

    for (int i = 0; i < m.n; i++) {
        pivot = m.value[i][i];

        for (int j = 0; j < m.n; j++) {
            m.value[i][j] /= pivot;
            inverted.value[i][j] /= pivot;
        }

        for (int j = 0; j < m.n; j++) {
            if (j != i) {
                tmp = m.value[j][i];

                for (int k = 0; k < m.n; k++) {
                    m.value[j][k] -= tmp * m.value[i][k];
                    inverted.value[j][k] -= tmp * inverted.value[i][k];
                }
            }
        }
    }

    return inverted;
}

void StrassenSplit(Matrix a, Matrix a11, Matrix a12, Matrix a21, Matrix a22) {
  int size = a.m >> 1;

  for (int j = 0; j < size; ++j) {
    for (int i = 0; i < size; ++i) {
      a11.value[j][i] = a.value[j][i];
      a21.value[j][i] = a.value[j + size][i];
      a12.value[j][i] = a.value[j][i + size];
      a22.value[j][i] = a.value[j + size][i  + size];
    }
  }
}

Matrix StrassenCollect(Matrix a11, Matrix a12, Matrix a21, Matrix a22) {
  int size = a11.m;
  Matrix res = CreateDynamicMatrix(size << 1, size << 1);

  for (int j = 0; j < size; ++j) {
    for (int i = 0; i < size; ++i) {
      res.value[j][i] = a11.value[j][i];
      res.value[j + size][i] = a21.value[j][i];
      res.value[j][i + size] = a12.value[j][i];
      res.value[j + size][i  + size] = a22.value[j][i];
    }
  }
  
  return res;
}

Matrix Strassen(Matrix a, Matrix b) {
  int size = a.n >> 1;

  Matrix a11 = CreateDynamicMatrix(size, size);
  Matrix a21 = CreateDynamicMatrix(size, size);
  Matrix a12 = CreateDynamicMatrix(size, size);
  Matrix a22 = CreateDynamicMatrix(size, size);

  Matrix b11 = CreateDynamicMatrix(size, size);
  Matrix b21 = CreateDynamicMatrix(size, size);
  Matrix b12 = CreateDynamicMatrix(size, size);
  Matrix b22 = CreateDynamicMatrix(size, size);

  StrassenSplit(a, a11, a12, a21, a22);
  StrassenSplit(b, b11, b12, b21, b22);

  Matrix p1 = Strassen(MatrixSum(a11, a22), MatrixSum(b11, b22));
  Matrix p2 = Strassen(MatrixSum(a11, a22), b11);
  Matrix p3 = Strassen(a11, MatrixDiff(b11, b22));
  Matrix p4 = Strassen(a22, MatrixDiff(b21, b11));
  Matrix p5 = Strassen(MatrixSum(a11, a12), b22);
  Matrix p6 = Strassen(MatrixDiff(a21, a11), MatrixSum(b11, b12));
  Matrix p7 = Strassen(MatrixDiff(a12, a22), MatrixSum(b21, b22));

  Matrix c11 = MatrixSum(MatrixSum(p1, p4), MatrixDiff(p7, p5));
  Matrix c12 = MatrixSum(p3, p5);
  Matrix c21 = MatrixSum(p2, p4);
  Matrix c22 = MatrixSum(MatrixDiff(p1, p2), MatrixSum(p3, p6));

  return StrassenCollect(c11, c12, c21, c22);
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
