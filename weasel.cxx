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
#include <algorithm>
#include <random>
using namespace std;

// == Function prototypes ==

string buildRandomString(int length);
void evolve(string& scrambled, const string target);
const int score(const string scrambled);
const int getDistance(const char first, const char second);
void evolve(string& scrambled, const string target);
vector<string> spawn(const string scrambled);
string mutateString(const string scrambled);
string cull(vector<string> children);

// == Tiny functions ==
bool compareScores(const string& first, const string& second){return(score(first)<score(second));}

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
  //string target("weasel");

  string scrambled = buildRandomString( target.length() );

  int attempts = 0;
  while( /*attempts++ < 9 &&*/ target.compare(scrambled) != 0 ) {
    //++attempts;
    //if (attempts > 1000) {
    cout << attempts++ << ": " << scrambled << " : " << score(scrambled) << endl;
    //attempts = 0;
    //}
    evolve(scrambled, target);
  }

  cout << scrambled << endl;

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

void evolve(string& parent, const string target){
  vector<string> children = spawn(parent);
  //cout << "Original: " << parent << " | Children: " << endl;
  //for (string eachchild : children) { cout << eachchild << ":" << score(eachchild) << " | ";}
  
  //cout << endl;
  children.push_back(parent);
  parent = cull(children);
  return;
}

vector<string> spawn(const string scrambled){
  const int litterSize = 99;
  vector<string> children = {};

  for (int i = 0; i < litterSize; i++){
    children.push_back( mutateString(scrambled) );
  }

  return children;
}

string mutateString(const string scrambled){
  static std::default_random_engine gen;
  static std::uniform_int_distribution<int> dist(0,100); //initialize a rand generator betwen 0 and 100
  const int mutateChance = 5; // percent chance of mutation
  vector<char> alphanumerics = getAlphanumerics();
  string mutated = scrambled;

  int chance  = 0;
  for (auto& eachletter : mutated){
    //cout << "Current letter: " << eachletter << endl;
    chance = dist(gen);
    if (chance < mutateChance) {
      eachletter =  *select_randomly(alphanumerics.begin(), alphanumerics.end()) ;
    }
    //cout << "New letter: " << eachletter << endl;
  }
  return mutated;
}


string cull(vector<string> children){
  return *std::min_element(children.begin(),children.end(),compareScores);
}


const int score(const string candidate){

  //const string target = "weasel"; //need to find a way not to duplicate this
  const string target("Methinks it is like a weasel");

  string::const_iterator it = candidate.begin();
  string::const_iterator targetit = target.begin();
  int score = 0;

  for (; it < candidate.end(); it++, targetit++){
    //cout << "Scrambled: " << *it << " | Target: " << *targetit << endl;
    if (*it != *targetit){
      //cout << "Score: " << getDistance(*it,*targetit) << endl;
      //score += getDistance(*it,*targetit);
      score += 1;
    }
  }
  return score;
}

const int getDistance(const char firstchar, const char secondchar){
  vector<char> alphabet = getAlphanumerics();
  vector<char>::iterator firstit, secondit;
  firstit = find(alphabet.begin(), alphabet.end(), firstchar);
  secondit = find(alphabet.begin(), alphabet.end(), secondchar);
  return abs( distance(firstit,secondit) );
}

