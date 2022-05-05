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

struct ConfigStruct {
  int choosedOption;
  bool regmem;
  char logname[100], firstMatrixFile[100], secondMatrixFile[100], outputMatrixFile[100];
};

ConfigStruct config;

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

  int c;

  config.choosedOption = -1;
  config.regmem = false;
  config.logname[0] = 0;
  config.firstMatrixFile[0] = 0;
  config.secondMatrixFile[0] = 0;
  config.outputMatrixFile[0] = 0;

  // getopt - letra indica a opcao, : junto a letra indica parametro
  // no caso de escolher mais de uma operacao, vale a ultima
  while ((c = getopt(argc, argv, "smtp:1:2:o:lh")) != EOF)
    switch(c) {
      case 'm':
        warnAssert(config.choosedOption==-1,"More than one option choosed");
        config.choosedOption = MULTIPLYOPERATION;
        break;
      case 's':
        warnAssert(config.choosedOption==-1,"More than one option choosed");
        config.choosedOption = SUMOPERATION;
        break;
      case 't':
        warnAssert(config.choosedOption==-1,"More than one option choosed");
        config.choosedOption = TRANSPOSEOPETARION;
        break;
      case 'p': 
        strcpy(config.logname, optarg);
        break;
      case '1': 
        strcpy(config.firstMatrixFile, optarg);
        break;
      case '2': 
        strcpy(config.secondMatrixFile, optarg);
        break;
      case 'o': 
        strcpy(config.outputMatrixFile, optarg);
        break;
      case 'l': 
        config.regmem = true;
        break;
      default:
        menu();
        exit(1);
        break;
    }

  errorAssert(config.choosedOption > 0,"Matrix Class - you must choose an operation");
  errorAssert(strlen(config.logname) > 0,
    "Matrix Class - access register file name must be previously defined");
  errorAssert(strlen(config.firstMatrixFile) > 0,
    "Matrix Class - first matrix input file name must be previously defined");
}

void executeOperation() {
  setFaseMemLog(0);

  Matrix A(config.firstMatrixFile, 0);

  Matrix B(config.secondMatrixFile, 1);

  Matrix C(1, 1, 2);
  C.initializeAsNullMatrix();

  setFaseMemLog(1);

  A.warmUpMatrix();
  B.warmUpMatrix();
  C.warmUpMatrix();

  if(config.choosedOption == SUMOPERATION)
    A.sumMatrices(&B, &C);
  else if(config.choosedOption == MULTIPLYOPERATION)
    A.mutiplyMatrices(&B, &C);

  setFaseMemLog(2);
  C.warmUpMatrix();

  if (config.regmem) { 
    C.printMatrix();
    C.writeMatrix(config.outputMatrixFile);
  }
}

void executeTranpose() {
  setFaseMemLog(0);

  Matrix A(config.firstMatrixFile, 0);

	setFaseMemLog(1);

  A.warmUpMatrix();
  
  A.transposeMatrix();

  setFaseMemLog(2);

  A.warmUpMatrix();

	if (config.regmem) { 
    A.printMatrix();
    A.writeMatrix(config.outputMatrixFile);
  }
}

int main(int argc, char ** argv) {
  parse_args(argc,argv);

  startMemLog(config.logname);

  if (config.regmem) 
    activateMemLog();
  else 
    deactivateMemLog();

  switch (config.choosedOption) {
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

