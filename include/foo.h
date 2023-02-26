#ifndef FOO_H_
#pragma once
#define FOO_H_

typedef struct Matrix {
  int m;
  int n;
  int** value;
} Matrix;

Matrix CreateDynamicMatrix(const int m, const int n);

Matrix GenerateRandomMatrix(const int m, const int n, const int maximum);

Matrix MatrixMultiply(const Matrix a, const Matrix b);

Matrix MatrixSum(const Matrix a, const Matrix b);

Matrix MatrixTransponate(const Matrix a);

void FillMatrix(Matrix matrix);

void FillMatrixFromFile(Matrix matrix, const char* filename);

void PrintMatrix(Matrix matrix);

#endif