#
# Copyright 2021 Alysson Ribeiro da Silva - Federal University of Minas Gerais
#
# Permission is hereby granted, free of charge, to any person obtaining a copy 
# of this software and associated documentation files (the "Software"), to deal 
# in the Software without restriction, including without limitation the rights 
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
# of the Software, and to permit persons to whom the Software is furnished to do 
# so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all 
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
# PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

# cc and flags
CC = g++
CXXFLAGS = -std=c++11 -g -Wall
#CXXFLAGS = -std=c++11 -O3 -Wall

# folders
INCLUDE_FOLDER = ./include/
BIN_FOLDER = ./bin/
OBJ_FOLDER = ./obj/
SRC_FOLDER = ./src/
ASSETS_FOLDER = ./assets/
OUT_FOLDER = ./out/

# all sources, objs, and header files
MAIN = Main
TARGET = run.out
SRC = $(wildcard $(SRC_FOLDER)*.cc)
OBJ = $(patsubst $(SRC_FOLDER)%.cc, $(OBJ_FOLDER)%.o, $(SRC))
EXE = $(BIN_FOLDER)/run.out
ANALISAMEM = ./analisamem/bin/analisamem

all: build mem

build: $(OBJ)
	$(CC) $(CXXFLAGS) -o $(BIN_FOLDER)$(TARGET) $(OBJ)

mem: $(EXE)
	$(EXE) -s -p /tmp/summatrixlog.out -1 $(ASSETS_FOLDER)/InputMatrix1.txt -2 $(ASSETS_FOLDER)/InputMatrix2.txt -o $(OUT_FOLDER)/outputsum.txt -l
	rm -rf /tmp/sumdin
	mkdir /tmp/sumdin
	$(ANALISAMEM) -i /tmp/summatrixlog.out -p /tmp/sumdin/sumdin
	gnuplot /tmp/sumdin/*.gp

	$(EXE) -m -p /tmp/multmatrixlog.out -1 $(ASSETS_FOLDER)/InputMatrix1.txt -2 $(ASSETS_FOLDER)/InputMatrix2.txt -o $(OUT_FOLDER)/outputmult.txt -l
	rm -rf /tmp/multdin
	mkdir /tmp/multdin
	$(ANALISAMEM) -i /tmp/multmatrixlog.out -p /tmp/multdin/multdin
	gnuplot /tmp/multdin/*.gp

	$(EXE) -t -p /tmp/transpmatrixlog.out -1 $(ASSETS_FOLDER)/InputMatrix1.txt -2 $(ASSETS_FOLDER)/InputMatrix2.txt -o $(OUT_FOLDER)/outputtransp.txt -l
	rm -rf /tmp/transpdin
	mkdir /tmp/transpdin
	$(ANALISAMEM) -i /tmp/transpmatrixlog.out -p /tmp/transpdin/transpdin
	gnuplot /tmp/transpdin/*.gp

$(OBJ_FOLDER)%.o: $(SRC_FOLDER)%.cc
	$(CC) $(CXXFLAGS) -c $< -o $@ -I$(INCLUDE_FOLDER)

clean:
	@rm -rf $(OBJ_FOLDER)* $(BIN_FOLDER)*
