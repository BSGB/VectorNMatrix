// VectorMatrixProjectTwo.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <conio.h>
#include <time.h>
#include <omp.h>
#include <iostream>

using namespace std;

#define x 200
#define y 200
#define z 200

void tictac(int miliseconds) {
	clock_t endwait;
	endwait = clock() + miliseconds * CLOCKS_PER_SEC / 1000;
	while (clock() < endwait) {}
}

double workTime(clock_t time) {
	return static_cast<double> (time) / CLOCKS_PER_SEC;
}


int matrixA[x][y];
int matrixB[y][z];
int matrixResult[x][z];

int vector[y];
int vectorResult[x];

int main()
{
	omp_set_nested(1);

	srand(time(NULL));

	int loopCounterI = 0;
	int loopCounterJ = 0;
	int loopCounterK = 0;

	int sum = 0;

#pragma omp parallel for private(loopCounterI) schedule(static)
	for (loopCounterI = 0; loopCounterI < x; loopCounterI++) {
		//tictac(30);
#pragma omp parallel for shared(matrixA) private(loopCounterJ) schedule(static)
		for (loopCounterJ = 0; loopCounterJ < y; loopCounterJ++) {
			matrixA[loopCounterI][loopCounterJ] = 5;
		}
	}

#pragma omp parallel for private(loopCounterI) schedule(static)
	for (loopCounterI = 0; loopCounterI < y; loopCounterI++) {
		//tictac(30);
#pragma omp parallel for shared(matrixB) private(loopCounterJ) schedule(static)
		for (loopCounterJ = 0; loopCounterJ < z; loopCounterJ++) {
			matrixB[loopCounterI][loopCounterJ] = 5;
		}
	}

#pragma omp parallel for shared(vector) private(loopCounterI) schedule(static)
	for (loopCounterI = 0; loopCounterI < y; loopCounterI++) {
		//tictac(10);
		vector[loopCounterI] = rand() % 50 + 7;
	}

	//macierz x macierz

#pragma omp parallel for private(loopCounterI) schedule(static)
	for (loopCounterI = 0; loopCounterI < x; loopCounterI++) {
		//tictac(30);
#pragma omp parallel for shared(matrixResult) private(loopCounterJ, sum) schedule(static)
		for (loopCounterJ = 0; loopCounterJ < z; loopCounterJ++) {
			sum = 0;
#pragma omp parallel for shared(matrixA, matrixB) private(loopCounterK) reduction(+:sum) schedule(static)
			for (loopCounterK = 0; loopCounterK < y; loopCounterK++) {
				//tictac(10);
				sum += matrixA[loopCounterI][loopCounterK] * matrixB[loopCounterK][loopCounterJ];
			}
			matrixResult[loopCounterI][loopCounterJ] = sum;
		}
	}

	//macierz x wektor

#pragma omp parallel for shared(vectorResult) private(loopCounterI, sum) schedule(static)
	for (loopCounterI = 0; loopCounterI < x; loopCounterI++) {
		//tictac(30);
		sum = 0;
#pragma omp parallel for shared(matrixA, vector) private(loopCounterJ) reduction(+:sum) schedule(static)
		for (loopCounterJ = 0; loopCounterJ < y; loopCounterJ++) {
			sum += matrixA[loopCounterI][loopCounterJ] * vector[loopCounterJ];
		}
		vectorResult[loopCounterI] = sum;
	}

	//wynik macierzy
	for (int i = 0; i < x; i++) {
		for(int j = 0; j < z; j++){
			cout << "[" << matrixResult[i][j] << "]";
		}
		cout << endl;
	}

	//wynik wektora
	for (int i = 0; i < x; i++) {
		cout << "{" << vectorResult[i] << "}";
	}

	cout << "Czas pracy: " << workTime(clock()) << endl;

	return 0;
}