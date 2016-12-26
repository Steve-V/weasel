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
void mutate(string& scrambled, const string target);
const int score(const string scrambled, const string target);

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

  string scrambled = buildRandomString( target.length() );

  int attempts = 0;
  while( attempts < 3 && target.compare(scrambled) != 0 ) {
    cout << ++attempts << ": " << scrambled << " does not match! Score: " << score(scrambled,target) << endl;
    mutate(scrambled, target);
  }

  cout << scrambled << " matches!" << endl;

  return 0;
}

vector<char> getAlphanumerics(){
  return vector<char> {
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
}

string buildRandomString(int length){

  vector<char> alphanumerics = getAlphanumerics();

  string randstring = "";
  for (auto i = 0; i < length; i++){
    randstring.push_back( *select_randomly(alphanumerics.begin(), alphanumerics.end()) );
  }

  return randstring;

} //end buildRandomString

void mutate(string& scrambled, const string target){
  scrambled = buildRandomString( target.length() );
  return;
}

const int score(const string scrambled, const string target){

  string::const_iterator it = scrambled.begin();
  string::const_iterator targetit = target.begin();
  //int score = 0;

  for (; it < scrambled.end(); it++, targetit++){
    cout << "Scrambled: " << *it << " | Target: " << *targetit << endl;

  }
  cout << endl;
  return 0;
}

