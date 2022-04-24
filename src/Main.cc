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

void menu() {
  fprintf(stderr,"Matrix Class\n");
  fprintf(stderr,"\t-s \t(sum matrices) \n");
  fprintf(stderr,"\t-m \t(multiply matrices) \n");
  fprintf(stderr,"\t-t \t(transpose matrix)\n");
  fprintf(stderr,"\t-p <arq>\t(access register file)\n");
  fprintf(stderr,"\t-l \t(register memory access)\n");
  fprintf(stderr,"\t-x <int>\t(rows)\n");
  fprintf(stderr,"\t-y <int>\t(columns)\n");
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
  while ((c = getopt(argc, argv, "smtp:x:y:lh")) != EOF)
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
        strcpy(logname,optarg);
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
  errorAssert(optRows > 0,"Matrix Class - X dimension of your marix must be positive");
  errorAssert(optColumns > 0,"Matrix Class - Y dimension of your marix must be positive");
}

void executeOperation() {
  setFaseMemLog(0);

  Matrix A(optRows, optColumns, 0);
  A.initializeAsRandomMatrix();

  Matrix B(optRows, optColumns, 1);
  B.initializeAsRandomMatrix();

  Matrix C(optRows, optColumns, 2);
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

  if (regmem) C.printMatrix();
}

void executeTranpose() {
  setFaseMemLog(0);

  Matrix A(optRows, optColumns, 0);
  A.initializeAsRandomMatrix();

	setFaseMemLog(1);

  A.warmUpMatrix();
  
  A.transposeMatrix();

  setFaseMemLog(2);

  A.warmUpMatrix();

	if (regmem) A.printMatrix();
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

