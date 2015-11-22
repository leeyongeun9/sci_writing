#ifndef TEMPERATURE
#define TEMPERATURE

#include <iostream>
#include <cmath>
#include <random>
#include <string>
#include <stdio.h>
#include <time.h>
#include <vector>
#include "Person.h"

#define NUMBER_OF_PERSON 100
#define TIME_LIMIT_IN_MINUTES 60

#define LAMBDA_PLUS_INVERSE 15
#define LAMBDA_ZERO_INVERSE 30
#define LAMBDA_MINUS_INVERSE 50

#define MEAN_LAMBDA_SUCCESSIVE_POSI_INVERSE 30
#define SIGMA_LAMBDA_POSI 10

#define MEAN_LAMBDA_SUCCESSIVE_NEGA_INVERSE 80
#define SIGMA_LAMBDA_NEGA 25

#define THRESHOLD_V 10
#define THRESHOLD_S 3

int main(int argc, char *argv[]);

#endif
