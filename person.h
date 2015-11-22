#ifndef PERSON
#define PERSON

#include <iostream>
#include <cmath>
#include <random>
#include <string>
#include <stdio.h>
#include <time.h>
#include <vector>

#define BIG_VALUE 100000
#define PRINT 0

using namespace std;

class Person{
    
public:

    typedef struct opinion_info{
        double time;
        int sign;
        double value;
    } opinion;

    double firstarrival_lambda,interarrival_positive_lambda, interarrival_negative_lambda;
    double exponential_lambda_1, exponential_lambda_2, exponential_lambda_3;
    vector<opinion> opinion_list;
    vector<opinion> tempChange_list;

    //default_random_engine eng;
    static const double successiveRatio;

    void generate_opinion_recursive(double timelimit, double offset, mt19937_64 &eng);
    opinion findLastOpinion(vector<opinion> list, double start_time);
    opinion findFirstOpinion(vector<opinion> list, double start_time);
    double getCalculatedValue(double initial, double ellapsedTime, double lambda);

public:
    Person(double lambda1, double lambda2, double lambda3);
    Person(double first_lambda,double positive_lambda, double negative_lambda, double lambd1, double lambda2, double lambda3);

    double generate_opinion(double timelimit, mt19937_64 &eng);
    void makeChange(double time, int direction);
    double getOpinionValue(double curtime);
    
    double getOpinionValueWithUnitChanges(double initial, double timeEllipse, int changes);
    
};
#endif
