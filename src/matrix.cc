#include "matrix.h"
#include "msgassert.h"

// Superior limit of random initialization
#define INITRANDOMRANGE 10

Matrix::Matrix() : Matrix(1, 1, -1) {
}

Matrix::Matrix(char inputFileName[], int id) {
  FILE *file;
  file = fopen(inputFileName, "r");
  errorAssert(file != NULL,"\nFailed to open Matrix input file");
  int rows, columns;
  double aux;
  fscanf(file, "%d ", &rows);
  fscanf(file, "%d ", &columns);

  errorAssert(rows>0,"Null dimension");
  errorAssert(columns>0,"Null dimension");
  warnAssert(rows<=MAXRECOMMENDEDSIZE,"X dimension above the recommended value");
  warnAssert(columns<=MAXRECOMMENDEDSIZE,"Y dimension above the recommended value");

  this->rows = rows;
  this->columns = columns;
  this->id = id;

  this->mat = (double**) malloc(this->rows * sizeof(double*));
  for(int i = 0; i < this->rows; i++) {
    this->mat[i] = (double*) malloc(this->columns * sizeof(double));
    errorAssert(this->mat[i]!=NULL,"Failed to allocate memory");
  }

  errorAssert(this->mat!=NULL,"Failed to allocate memory");

  while (feof(file) == 0) {
    for (int i = 0; i < rows; i++) {
      for(int j = 0; j < columns; j++) {
        fscanf(file, "%lf ", &aux);
        this->mat[i][j] = aux;
        WRITEMEMLOG((long int) (&(this->mat[i][j])), sizeof(double), this->id);
      }
    }
  }
  
  fclose(file);
}


Matrix::Matrix(int rows, int columns, int id) {
  errorAssert(rows>0,"Null dimension");
  errorAssert(columns>0,"Null dimension");
  warnAssert(rows<=MAXRECOMMENDEDSIZE,"X dimension above the recommended value");
  warnAssert(columns<=MAXRECOMMENDEDSIZE,"Y dimension above the recommended value");

  this->rows = rows;
  this->columns = columns;
  this->id = id;

  this->mat = (double**) malloc(this->rows * sizeof(double*));
  for(int i = 0; i < this->rows; i++) {
    this->mat[i] = (double*) malloc(this->columns * sizeof(double));
    errorAssert(this->mat[i]!=NULL,"Failed to allocate memory");
  }

  errorAssert(this->mat!=NULL,"Failed to allocate memory");
}

void Matrix::initializeAsNullMatrix() {
  errorAssert(this->mat!=NULL,"Class was not correctly instaciated");
  errorAssert((this->rows > 0) && (this->columns > 0),"Matrix has already been destroyed");

  for (int i = 0; i < this->rows; i++) {
    for(int j = 0; j < this->columns; j++) {
      this->mat[i][j] = 0;
      WRITEMEMLOG((long int) (&(this->mat[i][j])), sizeof(double), this->id);
    }
  }
}

void Matrix::initializeAsRandomMatrix() {
  errorAssert(this->mat!=NULL,"Class was not correctly instaciated");
  errorAssert((this->rows > 0) && (this->columns > 0),"Matrix has already been destroyed");

  for (int i = 0; i < this->rows; i++) {
    for (int j = 0; j < this->columns; j++) {
      this->mat[i][j] = drand48() * INITRANDOMRANGE;
      WRITEMEMLOG((long int) (&(this->mat[i][j])), sizeof(double), this->id);
    }
  }
}

void Matrix::initializeAsInputedFileMatrix(char inputFileName[]) {
  errorAssert(this->mat!=NULL,"Class was not correctly instaciated");
  errorAssert((this->rows > 0) && (this->columns > 0),"Matrix been destroyed");

  FILE *file;
  file = fopen(inputFileName, "r");
  errorAssert(file != NULL,"\nFailed to open Matrix input file");
  int rows, columns;
  double aux;
  fscanf(file, "%d ", &rows);
  fscanf(file, "%d ", &columns);

  Matrix *auxMatrix = new Matrix(rows, columns, this->id);

  while (feof(file) == 0) {
    for (int i = 0; i < rows; i++) {
      for(int j = 0; j < columns; j++) {
        fscanf(file, "%lf ", &aux);
        auxMatrix->mat[i][j] = aux;
        WRITEMEMLOG((long int) (&(auxMatrix->mat[i][j])), sizeof(double), auxMatrix->id);
      }
    }
  }
  
  fclose(file);
  (*this) = (*auxMatrix);
}

double Matrix::warmUpMatrix() {
  errorAssert(this->mat!=NULL,"Class was not correctly instaciated");
  errorAssert((this->rows > 0) && (this->columns > 0),"Matrix been destroyed");

  double aux, s=0.0;

  for (int i = 0; i < this->rows; i++) {
    for (int j = 0; j < this->columns; j++) {
      aux = this->mat[i][j];
      s+=aux;
      READMEMLOG((long int) (&(this->mat[i][j])), sizeof(double), this->id);
    }
  }

  return s;
}

void Matrix::printMatrix() {
  errorAssert(this->mat!=NULL,"Class was not correctly instaciated");
  errorAssert((this->rows > 0) && (this->columns > 0),"Matrix been destroyed");

  // print columns identifiers
  printf("%9s"," ");
  for (int i = 0; i < this->columns; i++) printf("%8d ", i);
  printf("\n");

  for (int i = 0; i < this->rows; i++) {
    printf("%8d ", i);
    for(int j = 0; j < this->columns; j++) {
      printf("%8.2f ", this->mat[i][j]);
      READMEMLOG((long int)(&(this->mat[i][j])) ,sizeof(double), this->id);
    }
    printf("\n");
  }
}

void Matrix::writeMatrix(char outputFileName[]) {
  errorAssert(this->mat!=NULL,"Class was not correctly instaciated");
  errorAssert((this->rows > 0) && (this->columns > 0),"Matrix has already been destroyed");

  FILE *file;
  file = fopen(outputFileName, "w");
  errorAssert(file != NULL,"\nFailed to open Matrix output file");

  // write columns identifiers
  fprintf(file, "%9s"," ");
  for (int i = 0; i < this->columns; i++) fprintf(file, "%8d ", i);
  fprintf(file, "\n");

  for (int i = 0; i < this->rows; i++) {
    fprintf(file, "%8d ", i);
    for(int j = 0; j < this->columns; j++) {
      fprintf(file, "%8.2f ", this->mat[i][j]);
      READMEMLOG((long int)(&(this->mat[i][j])) ,sizeof(double), this->id);
    }
    fprintf(file, "\n");
  }
}

void Matrix::setElement(int x, int y, double value) {
  errorAssert((x >= 0) && (x < this->rows), "Invalid row index");
  errorAssert((y >= 0) && (y < this->columns), "Invalid column index");

  this->mat[x][y] = value;
  WRITEMEMLOG((long int) (&(this->mat[x][y])), sizeof(double), this->id);
}

double Matrix::getElement(int x, int y) {
  errorAssert((x >= 0) && (x < this->rows), "Invalid row index");
  errorAssert((y >= 0) && (y < this->columns), "Invalid column index");

  READMEMLOG((long int) (&(this->mat[x][y])), sizeof(double), this->id);
  return this->mat[x][y];
}

void Matrix::copyMatrix(Matrix *dst, int dst_id) {
  errorAssert(this->mat!=NULL,"Class was not correctly instaciated");
  errorAssert((this->rows > 0) && (this->columns > 0),"Matrix has already been destroyed");

  Matrix *dst_aux = new Matrix(this->rows, this->columns, dst_id);
  dst_aux->initializeAsNullMatrix();

  for (int i = 0; i < this->rows; i++) {
    for(int j = 0; j < this->columns; j++) {
      dst_aux->mat[i][j] = this->mat[i][j];
      READMEMLOG((long int) (&(this->mat[i][j])), sizeof(double), this->id);
      WRITEMEMLOG((long int) (&(dst_aux->mat[i][j])), sizeof(double), dst_aux->id);
    }
  }

  (*dst) = (*dst_aux);
}

void Matrix::sumMatrices(Matrix *parcelMatrix, Matrix *resultMatrix) {
  errorAssert(this->mat!=NULL,"Class was not correctly instaciated");
  errorAssert(this->rows == parcelMatrix->rows, "Matrices rows do not match");
  errorAssert(this->columns == parcelMatrix->columns, "Matrices columns do not match");

  Matrix *resultMatrix_aux = new Matrix(this->rows, this->columns, resultMatrix->id);
  resultMatrix_aux->initializeAsNullMatrix();

  for (int i = 0; i < this->rows; i++) {
    for(int j = 0; j < this->columns; j++) {
      resultMatrix_aux->mat[i][j] = this->mat[i][j] + parcelMatrix->mat[i][j];
      READMEMLOG((long int) (&(this->mat[i][j])), sizeof(double), this->id);
      READMEMLOG((long int) (&(parcelMatrix->mat[i][j])), sizeof(double), parcelMatrix->id);
      WRITEMEMLOG((long int) (&(resultMatrix_aux->mat[i][j])), sizeof(double), resultMatrix->id);
    }
  }

  (*resultMatrix) = (*resultMatrix_aux);
}

void Matrix::mutiplyMatrices(Matrix *parcelMatrix, Matrix *resultMatrix) {
  errorAssert(this->mat!=NULL,"Class was not correctly instaciated");
  errorAssert(this->columns == parcelMatrix->rows, "Dimensions are not compatible");

  Matrix *resultMatrix_aux = new Matrix(this->rows, parcelMatrix->columns, resultMatrix->id);
  resultMatrix_aux->initializeAsNullMatrix();

  for (int i = 0; i < resultMatrix_aux->rows; i++) {
    for (int j = 0; j < resultMatrix_aux->columns; j++) {
      for (int k = 0; k < this->columns; k++) {
        resultMatrix_aux->mat[i][j] += this->mat[i][k] * parcelMatrix->mat[k][j];
        READMEMLOG((long int) (&(this->mat[i][k])), sizeof(double), this->id);
        READMEMLOG((long int) (&(parcelMatrix->mat[k][j])), sizeof(double), parcelMatrix->id);
        WRITEMEMLOG((long int) (&(resultMatrix_aux->mat[i][j])), sizeof(double), resultMatrix_aux->id);
      }
    }
  }

  (*resultMatrix) = (*resultMatrix_aux);
}

void Matrix::transposeMatrix() {
  errorAssert(this->mat!=NULL,"Class was not correctly instaciated");
  errorAssert((this->rows > 0) && (this->columns > 0),"Matrix has already been destroyed");

  Matrix *auxMatrix = new Matrix(this->columns, this->rows, this->id);

  for (int i = 0; i < this->rows; i++) {
    for(int j = 0; j < this->columns; j++) {
      auxMatrix->mat[j][i] = this->mat[i][j];
      READMEMLOG((long int) (&(this->mat[i][j])), sizeof(double), this->id);
      WRITEMEMLOG((long int) (&(auxMatrix->mat[j][i])), sizeof(double), auxMatrix->id);
    }
  }

  (*this) = (*auxMatrix);
}

Matrix::~Matrix() {
  warnAssert((this->rows > 0) && (this->columns > 0),"Matrix has already been destroyed");
  printf("Deleting matrix: %d\n", this->id);

  for(int i = 0; i < this->rows; i++) {
    free(this->mat[i]);
  }
  free(this->mat);

  this->columns = -1;
  this->rows = -1;
}