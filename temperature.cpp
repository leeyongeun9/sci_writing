#include "temperature.h"

using namespace std;

int main(int argc, char *argv[]){
  double V, S;
  mt19937 eng(time(NULL));

  vector< Person*  > people;

  for (int i = 0 ; i < NUMBER_OF_PERSON ; i ++){
    double posiLambda, negaLambda;

    normal_distribution<double> normal_posi(MEAN_LAMBDA_SUCCESSIVE_POSI_INVERSE, SIGMA_LAMBDA_POSI);
    normal_distribution<double> normal_nega(MEAN_LAMBDA_SUCCESSIVE_NEGA_INVERSE, SIGMA_LAMBDA_NEGA);

    posiLambda = 1 / normal_posi(eng);
    negaLambda = 1 / normal_nega(eng);

    //posiLambda = fmin(0, posiLambda);
    //negaLambda = fmin(0, posiLambda);

    Person *newPerson = new Person(posiLambda, negaLambda, 1/LAMBDA_PLUS_INVERSE, 1/LAMBDA_ZERO_INVERSE, 1/LAMBDA_MINUS_INVERSE);
    newPerson -> generate_opinion(TIME_LIMIT_IN_MINUTES);
    people.push_back(newPerson);

    cout << posiLambda << endl;
  }

  double curTime;
  for (curTime = 0; curTime < TIME_LIMIT_IN_MINUTES; ){
    S = 0;
    V = 0;
    for (int i = 0 ; i < people.size() ; i ++ ){
      double value = people[i]->getOpinionValue(curTime);
      S += value;
      V += abs(value);
    }
    cout << "Time : " << curTime << ", (S,V) = (" << S << ", " << V << ")" << endl;
    if (abs(S) > THRESHOLD_S && V > THRESHOLD_V) {
      for (int i = 0 ; i < people.size() ; i ++){
        people[i]->makeChange(curTime, S / abs(S));
      }
    }
    curTime += 10.0 / 60;
  }
  cout << "The Last V Value is : " << V << endl;
}
