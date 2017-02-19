#pragma once
#include <assert.h>
#include <iostream>
using namespace std;

#include <Imagine/LinAlg.h>
#include<map>

// probability of failure"
static const float BETA = 0.01f;

// global constant : define the size of the descriptors used
const int v_size = 128;

// define a type for matrices
typedef Imagine::FMatrix<double, v_size, v_size> Matrix;

// define a type for vectors
typedef Imagine::FVector<double, v_size> Vector;

typedef std::map<int,string> ref;

// initialization of the random package
static void init_random(){srand(time(NULL));}

// returns an integer between a and b
static int random_int(int a, int b){return rand() % (b - a) + a;}
