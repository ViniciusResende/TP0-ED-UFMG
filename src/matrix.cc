#include "matrix.h"
#include "msgassert.h"

// Superior limit of random initialization
#define INITRANDOMRANGE 10
// Macro to swap elements
#define ELEMSWAP(x,y) (x+=y,y=x-y,x-=y)

Matrix::Matrix() : Matrix(0, 0, -1) {
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

  for (int i = 0; i < this->rows; i++) {
    for(int j = 0; j < this->columns; j++) {
      this->mat[i][j] = 0;
      WRITEMEMLOG((long int) (&(this->mat[i][j])), sizeof(double), this->id);
    }
  }
}

void Matrix::initializeAsRandomMatrix() {
  errorAssert(this->mat!=NULL,"Class was not correctly instaciated");
  this->initializeAsNullMatrix(); //TODO: verify if is really necessary

  for (int i = 0; i < this->rows; i++) {
    for (int j = 0; j < this->columns; j++) {
      this->mat[i][j] = drand48() * INITRANDOMRANGE;
      WRITEMEMLOG((long int) (&(this->mat[i][j])), sizeof(double), this->id);
    }
  }
}

double Matrix::warmUpMatrix() {
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
  // print columns identifiers
  printf("%9s"," ");
  for(int i = 0; i < this->columns; i++) printf("%8d ",i);
  printf("\n");

  for (int i = 0; i < this->rows; i++) {
    printf("%8d ",i);
    for(int j = 0; j < this->columns; j++) {
      printf("%8.2f ", this->mat[i][j]);
      READMEMLOG((long int)(&(this->mat[i][j])) ,sizeof(double), this->id);
    }
    printf("\n");
  }

  // print lines
  // int aux = 0, currentLine = 0;
  // for (int i = 0; i < this->rows * this->columns; i++) {
  //   if(aux < this->columns) {
  //     if(aux == 0) printf("%8d ", currentLine);
  //     printf("%8.2f ", *(this->mat[i]));
  //     READMEMLOG((long int) (&(*(this->mat[i]))), sizeof(double), this->id);
  //     aux++;
  //   } else {
  //     printf("%8.2f ", *(this->mat[i]));
  //     READMEMLOG((long int) (&(*(this->mat[i]))), sizeof(double), this->id);
  //     aux = 0;
  //     currentLine++;
  //     printf("\n");
  //   }
  // }
}

void Matrix::setElement(int x, int y, double value) {
  errorAssert((x > 0) && (x <= this->rows), "Invalid row index");
  errorAssert((y > 0) && (y <= this->columns), "Invalid column index");

  // int offset = x * this->columns + y;
  this->mat[x][y] = value;
  WRITEMEMLOG((long int) (&(this->mat[x][y])), sizeof(double), this->id);
}

double Matrix::getElement(int x, int y) {
  errorAssert((x > 0) && (x <= this->rows), "Invalid row index");
  errorAssert((y > 0) && (y <= this->columns), "Invalid column index");

  // int offset = x * this->columns + y;
  READMEMLOG((long int) (&(this->mat[x][y])), sizeof(double), this->id);
  return this->mat[x][y];
}

void Matrix::copyMatrix(Matrix *dst, int dst_id) {
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
  // delete dst_aux;
}

void Matrix::sumMatrices(Matrix *parcelMatrix, Matrix *resultMatrix) {
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
  // delete resultMatrix_aux;
}

void Matrix::mutiplyMatrices(Matrix *parcelMatrix, Matrix *resultMatrix) {
  errorAssert(this->columns == parcelMatrix->rows, "Dimensions are not compatible");

  Matrix *resultMatrix_aux = new Matrix(this->rows, parcelMatrix->columns, resultMatrix->id);
  resultMatrix_aux->initializeAsNullMatrix();

  for (int i = 0; i < resultMatrix_aux->rows; i++) {
    for (int j = 0; j < resultMatrix_aux->columns; j++) {
      for (int k = 0; k < this->columns; k++) {
        resultMatrix_aux->mat[i][j] += this->mat[i][k] * parcelMatrix->mat[k][j];
        READMEMLOG((long int) (&(this->mat[i][k])), sizeof(double), this->id);
        READMEMLOG((long int) (&(parcelMatrix->mat[k][j])), sizeof(double), parcelMatrix->id);
        WRITEMEMLOG((long int) (&(resultMatrix->mat[i][j])), sizeof(double), resultMatrix->id);
      }
    }
  }

  (*resultMatrix) = (*resultMatrix_aux);
}

void Matrix::transposeMatrix() {
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
  printf("Apagando matriz %d\n", this->id);

  for(int i = 0; i < this->rows; i++) {
    free(this->mat[i]);
  }
  free(this->mat);

  this->columns = -1;
  this->rows = -1;
}