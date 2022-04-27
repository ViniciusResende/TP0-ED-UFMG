#ifndef MAXRECOMMENDEDSIZE
#define MAXRECOMMENDEDSIZE 500
#endif

#ifndef MATRIXH
#define MATRIXH

#include "memlog.h"

class Matrix {
  public:  
    Matrix();
    Matrix(int rows, int columns, int id);
    Matrix(char inputFileName[], int id);

    void initializeAsNullMatrix();
    void initializeAsRandomMatrix();
    double warmUpMatrix();
    void printMatrix();
    void writeMatrix(char outputFileName[]);
    void setElement(int x, int y, double value);
    double getElement(int x, int y);
    void copyMatrix(Matrix *dst, int dst_id);
    void sumMatrices(Matrix *parcelMatrix, Matrix *resultMatrix);
    void mutiplyMatrices(Matrix *parcelMatrix, Matrix *resultMatrix);
    void transposeMatrix();

    ~Matrix();

  private:
    double ** mat;
    int rows;
    int columns;
    int id;
};

#endif
