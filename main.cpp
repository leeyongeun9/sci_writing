//
//  main.cpp
//  simulator
//
//  Created by 비주얼캠프 on 2015. 11. 18..
//  Copyright (c) 2015년 비주얼캠프. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <random>
#include <string>
#include <time.h>
#include <vector>
#include <fstream>
#include "person.h"

#define NUMBER_OF_PERSON 60
#define TIME_LIMIT_IN_MINUTES 90

#define LAMBDA_PLUS_INVERSE 10
#define LAMBDA_ZERO_INVERSE 30
#define LAMBDA_MINUS_INVERSE 40

#define MEAN_LAMBDA_SUCCESSIVE_FIRST_INVERSE 20
#define MEAN_LAMBDA_SUCCESSIVE_POSI_INVERSE 70
#define SIGMA_LAMBDA_POSI 10

#define MEAN_LAMBDA_SUCCESSIVE_NEGA_INVERSE 210
#define SIGMA_LAMBDA_NEGA 25

#define THRESHOLD_V 20
#define THRESHOLD_S 3

#define CHANGE_TIME_INTERVAL 5
#define CALCULATE_COUNT 50

#define MIN_THRESHOLD_V 10
#define MIN_THRESHOLD_S 2

#define MAX_THRESHOLD_V 20
#define MAX_THRESHOLD_S 10

//#define TEST

using namespace std;

int main(int argc, const char * argv[]) {
#ifdef TEST
    double posiLambda = 0;
    double total = 0;
    mt19937_64 eng(time(NULL));
    
    normal_distribution<double> normal_posi(MEAN_LAMBDA_SUCCESSIVE_POSI_INVERSE, SIGMA_LAMBDA_POSI);
    for (int i = 0 ; i < 200 ; i ++ ){        
        posiLambda = normal_posi(eng);
        total += posiLambda;
    }
    total /= 200;
    cout << "200 random number Mean ~ N(30, 10) : " << total << endl;
    
    Person *testPerson = new Person(1.0/MEAN_LAMBDA_SUCCESSIVE_FIRST_INVERSE,
                                    1.0/MEAN_LAMBDA_SUCCESSIVE_POSI_INVERSE,
                                   1.0/MEAN_LAMBDA_SUCCESSIVE_NEGA_INVERSE,
                                   1.0/LAMBDA_PLUS_INVERSE,
                                   1.0/LAMBDA_ZERO_INVERSE,
                                   1.0/LAMBDA_MINUS_INVERSE);
    double firstOpinion = 0;
    for (int i = 0 ; i < 1 ; i ++ ){
        firstOpinion += testPerson->generate_opinion(100, eng);
    }
    firstOpinion /= 1;
    cout << "1 first Opinion ~ Exp(30) : " << firstOpinion << endl;
    cout << endl;
    cout << "===================opnionList=================="<< endl;
    int count = 1;
    vector <Person::opinion>::iterator iter = testPerson->opinion_list.begin();
    for ( ; iter != testPerson->opinion_list.end() ; ++iter){
        cout << "==== opinion " << count << " ====" << endl;
        cout << "time, sign, value -> " << iter->time << ", " << iter->sign << ", " << iter->value <<endl;
        printf("\n");
        count++;
    }
    cout << "==============================================="<< endl;
    cout << endl;
    cout << endl;
    cout << "==============getCaluculatedValue=============="<< endl;
    double calculV;
    calculV = testPerson->getCalculatedValue(1.0, 20, 1.0/30);
    if (abs(calculV-0.513417) < 0.001) {
        cout << "calculated correctly , value = " << calculV << endl;
    } else {
        cout << "calculated uncorrectly , value = " << calculV << endl;
    }
    cout << "==============================================="<< endl;
    cout << endl;
    
    
    cout << endl;
    cout << "================getOpinionValue================"<< endl;
    double endTimeValue;
    endTimeValue = testPerson->getOpinionValue(100.0);
    cout << "==============================================="<< endl;
    cout << endl;
    cout << "last Time Value : " << endTimeValue << endl;
    
    
    cout << endl;
    cout << "==================makeChange=================="<< endl;
    testPerson->makeChange(40.0, 1);
    testPerson->makeChange(70.0, -1);
    endTimeValue = testPerson->getOpinionValue(100.0);
    cout << "==================makeChange=================="<< endl;
    cout << endl;
    cout << "last Time Value : " << endTimeValue << endl;
    
#else
    double V = 0, S = 0;
    string fileName = "result.txt";
    string fileName2 = "result_simple.txt";
    ofstream outputfile(fileName);
    ofstream simplefile(fileName2);
    mt19937_64 eng(time(NULL));
    
    vector< Person*  > people;
    vector< double > changeList;
    

    
    double threshold_s, threshold_v = MIN_THRESHOLD_V;
    
    while (threshold_v <= MAX_THRESHOLD_V){
        threshold_s = MIN_THRESHOLD_S;
        while (threshold_s <= MAX_THRESHOLD_S){
            double totalV = 0;
            double numberChanges = 0;
            outputfile << "\n";
            outputfile << "\n";
            outputfile << "Threshold (S, V) : " << "(" << threshold_s << ", " << threshold_v << ")" << endl;
            for (int repeatNum = 0 ; repeatNum < CALCULATE_COUNT ; repeatNum ++) {
                people.clear();
                changeList.clear();
                for (int i = 0 ; i < NUMBER_OF_PERSON ; i ++){
                    double posiLambda, negaLambda;
                    
                    
                    
                    normal_distribution<double> normal_posi(MEAN_LAMBDA_SUCCESSIVE_POSI_INVERSE, SIGMA_LAMBDA_POSI);
                    normal_distribution<double> normal_nega(MEAN_LAMBDA_SUCCESSIVE_NEGA_INVERSE, SIGMA_LAMBDA_NEGA);
                    
                    posiLambda = 1.0 / normal_posi(eng);
                    negaLambda = 1.0 / normal_nega(eng);
                    
                    Person *newPerson = new Person(1.0/MEAN_LAMBDA_SUCCESSIVE_FIRST_INVERSE,
                                                   posiLambda,
                                                   negaLambda,
                                                   1.0/LAMBDA_PLUS_INVERSE,
                                                   1.0/LAMBDA_ZERO_INVERSE,
                                                   1.0/LAMBDA_MINUS_INVERSE);
                    
                    newPerson -> generate_opinion(TIME_LIMIT_IN_MINUTES, eng);
                    people.push_back(newPerson);
                    
                }
                
                double curTime;
                for (curTime = 0; curTime < TIME_LIMIT_IN_MINUTES; ){
                    S = 0;
                    V = 0;
                    /*if (changeList.size() > 0 && curTime < changeList.back() + CHANGE_TIME_INTERVAL) {
                        curTime += 10.0/60;
                        continue;
                    }*/
                    for (int i = 0 ; i < people.size() ; i ++ ){
                        double value = people[i]->getOpinionValue(curTime);
                        S += value;
                        V += abs(value);
                    }
                    if (abs(S) > threshold_s && V > threshold_v) {
                        for (int i = 0 ; i < people.size() ; i ++){
                            people[i]->makeChange(curTime, S / abs(S));
                        }
                        changeList.push_back(curTime);
                    }
                    curTime += 10.0 / 60;
                }
                S = 0;
                V = 0;
                for (int i = 0 ; i < people.size() ;i ++){
                    double value = people[i] -> getOpinionValue(TIME_LIMIT_IN_MINUTES);
                    S += value;
                    V += abs(value);
                }
                outputfile << "The Last V Value is : " << V << endl;
                totalV += V;
                numberChanges += changeList.size();
            }
            totalV /= CALCULATE_COUNT;
            numberChanges /= CALCULATE_COUNT;
            outputfile << "average final V value : " << totalV << endl;
            outputfile << "total change time : " << numberChanges << endl;
            simplefile << "(" << threshold_s << ", " << threshold_v << ") : " << totalV << endl;
            threshold_s += 0.5;
        }
        threshold_v += 0.5;
    }

    outputfile.close();
    simplefile.close();
#endif
    return 0;
}
