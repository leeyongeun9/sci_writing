#include "person.h"
using namespace std;

const double Person::successiveRatio = 1.1;


Person::Person(double lambda1, double lambda2, double lambda3){
    exponential_lambda_1 = lambda1;
    exponential_lambda_2 = lambda2;
    exponential_lambda_3 = lambda3;
}

Person::Person(double first_lambda, double positive_lambda, double negative_lambda, double lambda1, double lambda2, double lambda3){
    firstarrival_lambda = first_lambda;
    interarrival_positive_lambda = positive_lambda;
    interarrival_negative_lambda = negative_lambda;

    exponential_lambda_1 = lambda1;
    exponential_lambda_2 = lambda2;
    exponential_lambda_3 = lambda3;
}


double Person::generate_opinion(double timelimit, mt19937_64 &eng){
    opinion_list.clear();
    double firstOpinion;
    exponential_distribution<double> exponential(firstarrival_lambda);
    firstOpinion = exponential(eng);
    double returnValue = firstOpinion;
    while (firstOpinion >= timelimit){
        firstOpinion -= timelimit;
    }

    opinion newOpinion;
    newOpinion.time = firstOpinion;
    newOpinion.value = 1;
    uniform_real_distribution<double> uniform_real(0.0,1.0);

    if ( uniform_real(eng) >= 0.5 ){
      newOpinion.sign = 1;
    } else {
      newOpinion.sign = -1;
    }

    opinion_list.push_back(newOpinion);
    generate_opinion_recursive(timelimit, firstOpinion, eng);
    return returnValue;
}

void Person::generate_opinion_recursive(double timelimit, double offset, mt19937_64 &eng){
    double next = 0, positive = 0, negative = 0;
    exponential_distribution<double> exponential_positive(interarrival_positive_lambda);
    exponential_distribution<double> exponential_negative(interarrival_negative_lambda);
    while (positive == 0){positive = exponential_positive(eng);}
    while (negative == 0){negative = exponential_negative(eng);}
    if(PRINT){
        cout << "generate_recursive / positive, negative : " << positive << ", " << negative << endl;
        cout << "timelimit : " << timelimit << endl;
        cout << "offset : " << offset << endl;
        cout << endl;
    }
    if (fmin(positive,negative) + offset > timelimit) return;

    opinion newOpinion;
    if (positive < negative){
        next = positive;
        newOpinion.sign = opinion_list.back().sign;
        newOpinion.value = opinion_list.back().value * successiveRatio;
    } else {
        next = negative;
        newOpinion.sign = (-1) * opinion_list.back().sign;
        newOpinion.value = 1;
    }
    next += offset;
    newOpinion.time = next;
    opinion_list.push_back(newOpinion);
    generate_opinion_recursive(timelimit, next, eng);
}

void Person::makeChange(double time, int direction){
    opinion newChange;
    newChange.time = time;
    newChange.sign = direction;
    newChange.value = 0;

    tempChange_list.push_back(newChange);
}

Person::opinion Person::findLastOpinion(vector<opinion> list, double start_time) {
    for ( vector< opinion >::reverse_iterator iterPos = list.rbegin(); iterPos != list.rend() ; ++iterPos){
        if ( iterPos->time < start_time ) {
            return *iterPos;
        }
    }
    opinion returnOpinion;
    returnOpinion.time = 0;
    returnOpinion.sign = 0;
    returnOpinion.value = 0;
    return returnOpinion;
}

Person::opinion Person::findFirstOpinion(vector<opinion> list, double start_time) {
    for ( vector< opinion >::iterator iterPos = list.begin(); iterPos != list.end() ; ++iterPos){
        if ( iterPos->time > start_time ) {
            return *iterPos;
        }
    }
    opinion returnOpinion;
    returnOpinion.time = 0;
    returnOpinion.sign = 0;
    returnOpinion.value = 0;
    return returnOpinion;
}

double Person::getOpinionValue(double curtime){
    double calculatedValue = 1;
    double endLimit = curtime;
    int changes = 0;
    
    opinion lastOpinion = findLastOpinion(opinion_list, curtime);
    if (lastOpinion.sign == 0) return 0;
    
    double nextstart = lastOpinion.time;
    calculatedValue = lastOpinion.value * lastOpinion.sign;
    
    
    while(true){
        opinion nextChange = findFirstOpinion(tempChange_list, nextstart);
        if(nextChange.time == 0 || nextChange.time >= endLimit){
            double initial = calculatedValue;
            calculatedValue = getOpinionValueWithUnitChanges(calculatedValue, endLimit - nextstart, changes);
            if (PRINT){
                cout << "At last time, initial : " << initial << ", last : " << calculatedValue << endl;
                cout << "change number : " << changes <<endl;
                cout << "Time : " << nextstart << " ~ " << endLimit << endl;
            }
            break;
        } else {
            double initial = calculatedValue;
            calculatedValue = getOpinionValueWithUnitChanges(calculatedValue, nextChange.time - nextstart, changes);
            
            if (PRINT){
                cout << "next Change, initial : " << initial << ", last : " << calculatedValue << endl;
                cout << "change number : " << changes <<endl;
                cout << "Time : " << nextstart << " ~ " << nextChange.time << endl;
            }

            nextstart = nextChange.time;
            changes += nextChange.sign * lastOpinion.sign;
        }
    }
    return calculatedValue;
}

double Person::getCalculatedValue(double initial, double ellapsedTime, double lambda){
    double returnValue = initial * pow(M_E, (-1 * ellapsedTime) * lambda);
    return returnValue;
}

double Person::getOpinionValueWithUnitChanges(double initial, double timeEllipse, int changes){
    double returnValue;
    double lambda;
    if (changes == 0) lambda = exponential_lambda_2;
    else if (changes == 1) lambda = exponential_lambda_1;
    else if (changes == 2) lambda = exponential_lambda_2;
    else lambda = exponential_lambda_3;
    
    returnValue = getCalculatedValue(initial, timeEllipse, lambda);
    return  returnValue;
}










