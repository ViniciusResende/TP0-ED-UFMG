#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <string.h>
#include "matrix.h"
#include "memlog.h" 
#include "msgassert.h" 

#define SUMOPERATION 1
#define MULTIPLYOPERATION 2
#define TRANSPOSEOPETARION 3

static int choosedOption;
char logname[100];
int optRows, optColumns, regmem;
char firstMatrixFile[100], secondMatrixFile[100], outputMatrixFile[100];

void menu() {
  fprintf(stderr,"Matrix Class\n");
  fprintf(stderr,"-s \t\t(sum matrices) \n");
  fprintf(stderr,"-m \t\t(multiply matrices) \n");
  fprintf(stderr,"-t \t\t(transpose matrix)\n");
  fprintf(stderr,"-p <file>\t(access register file)\n");
  fprintf(stderr,"-1 <file>\t(first matrix file)\n");
  fprintf(stderr,"-2 <file>\t(second matrix file)\n");
  fprintf(stderr,"-o <file>\t(output matrix file)\n");
  fprintf(stderr,"-l \t\t(register memory access)\n");
}


void parse_args(int argc,char ** argv) {
  extern char * optarg;
  // extern int optind;

  int c;

  choosedOption = -1;
  optRows = -1;
  optColumns = -1;
  regmem = 0;
  logname[0] = 0;

  // getopt - letra indica a opcao, : junto a letra indica parametro
  // no caso de escolher mais de uma operacao, vale a ultima
  while ((c = getopt(argc, argv, "smtp:1:2:o:x:y:lh")) != EOF)
    switch(c) {
      case 'm':
        warnAssert(choosedOption==-1,"More than one option choosed");
        choosedOption = MULTIPLYOPERATION;
        break;
      case 's':
        warnAssert(choosedOption==-1,"More than one option choosed");
        choosedOption = SUMOPERATION;
        break;
      case 't':
        warnAssert(choosedOption==-1,"More than one option choosed");
        choosedOption = TRANSPOSEOPETARION;
        break;
      case 'p': 
        strcpy(logname, optarg);
        break;
      case '1': 
        strcpy(firstMatrixFile, optarg);
        break;
      case '2': 
        strcpy(secondMatrixFile, optarg);
        break;
      case 'o': 
        strcpy(outputMatrixFile, optarg);
        break;
      case 'x': 
        optRows = atoi(optarg);
        break;
      case 'y': 
        optColumns = atoi(optarg);
        break;
      case 'l': 
        regmem = 1;
        break;
      default:
        menu();
        exit(1);
        break;
    }

  // verificacao da consistencia das opcoes
  errorAssert(choosedOption > 0,"Matrix Class - you must choose an operation");
  errorAssert(strlen(logname) > 0,
    "Matrix Class - access register file name must be previously defined");
}

Matrix setupMatrix(int matrixId, char inputFileName[]) {
  FILE *file;
  file = fopen(inputFileName, "r");
  errorAssert(file != NULL,"\nFailed to open Matrix input file");
  int rows, columns;
  double aux;
  fscanf(file, "%d ", &rows);
  fscanf(file, "%d ", &columns);

  Matrix temporary(rows, columns, matrixId);

  while (feof(file) == 0) {
    for (int i = 0; i < rows; i++) {
      for(int j = 0; j < columns; j++) {
        fscanf(file, "%lf ", &aux);
        temporary.setElement(i, j, aux);
      }
    }
  }
  
  fclose(file);
  return temporary;
}

void executeOperation() {
  setFaseMemLog(0);

  Matrix A = setupMatrix(0, firstMatrixFile);

  Matrix B = setupMatrix(1, secondMatrixFile);

  Matrix C(1, 1, 2);
  C.initializeAsNullMatrix();

  setFaseMemLog(1);

  A.warmUpMatrix();
  B.warmUpMatrix();
  C.warmUpMatrix();

  if(choosedOption == SUMOPERATION)
    A.sumMatrices(&B, &C);
  else if(choosedOption == MULTIPLYOPERATION)
    A.mutiplyMatrices(&B, &C);

  setFaseMemLog(2);
  C.warmUpMatrix();

  if (regmem) { 
    C.printMatrix();
    C.writeMatrix(outputMatrixFile);
  }
}

void executeTranpose() {
  setFaseMemLog(0);

  Matrix A = setupMatrix(0, firstMatrixFile);

	setFaseMemLog(1);

  A.warmUpMatrix();
  
  A.transposeMatrix();

  setFaseMemLog(2);

  A.warmUpMatrix();

	if (regmem) { 
    A.printMatrix();
    A.writeMatrix(outputMatrixFile);
  }
}

int main(int argc, char ** argv) {
  // avaliar linha de comando
  parse_args(argc,argv);

  // iniciar registro de acesso
  startMemLog(logname);

  // ativar ou nao o registro de acesso
  if (regmem) 
    activateMemLog();
  else 
    deactivateMemLog();

  // execucao dependente da operacao escolhida
  switch (choosedOption) {
    case SUMOPERATION:
      executeOperation();
	    break;
    case MULTIPLYOPERATION:
      executeOperation();
	    break;
    case TRANSPOSEOPETARION:
      executeTranpose();
	    break;
    default:
      menu();
	    exit(1);
  }

  return endUpMemLog();
}

