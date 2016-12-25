/*
 * Steve Vaught
 * This program uses the Unlicense
 */

// == Include statements ==

#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <iterator>
#include <vector>
#include <random>
using namespace std;

// == Function prototypes ==

string buildRandomString(int length);

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
  std::uniform_int_distribution<> dis(0,std::distance(start, end) -1);
  std::advance(start,dis(g));
  return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  return select_randomly(start,end,gen);
}

// == Main ==
int main(){
  
  string target("Methinks it is like a weasel");

  cout << buildRandomString( target.length() ) << endl;

  return 0;
}

string buildRandomString(int length){

  vector<char> alphanumerics
  {
    'A','B','C','D','E','F',
    'G','H','I','J','K',
    'L','M','N','O','P',
    'Q','R','S','T','U',
    'V','W','X','Y','Z',
    'a','b','c','d','e','f',
    'g','h','i','j','k',
    'l','m','n','o','p',
    'q','r','s','t','u',
    'v','w','x','y','z',
    ' '
  }; //include space in the alphabet


  string randstring = "";
  for (auto i = 0; i < length; i++){
    randstring.push_back( *select_randomly(alphanumerics.begin(), alphanumerics.end()) );
  }

  return randstring;

} //end buildRandomString


