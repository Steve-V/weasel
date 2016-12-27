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
string breedString(const string mom, const string dad);
string cull(vector<string> children);
void breed(vector<string> parents);

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
  /* Set target, create initial random string, run evolution and keep count*/
  
  string target("Methinks it is like a weasel");

  string scrambled = buildRandomString( target.length() );

  int attempts = 0;
  while( target.compare(scrambled) != 0 ) {
    cout << attempts++ << ": " << scrambled << " : " << score(scrambled) << endl;
    evolve(scrambled, target);
  }

  cout << scrambled << " : Solved in " << attempts << " attempts!" << endl;

  return 0;
}

vector<char> getAlphanumerics(){
  // Return the candidate alphabet from which random letters (and space) are pulled
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
  }; 
}

string buildRandomString(int length){

  vector<char> alphanumerics = getAlphanumerics();

  string randstring = "";
  for (auto i = 0; i < length; i++){
    // push random letters onto the string until we reach the target string length
    randstring.push_back( *select_randomly(alphanumerics.begin(), alphanumerics.end()) );
  }

  return randstring;

} 

void evolve(string& parent, const string target){
  vector<string> children = spawn(parent); // populate a vector of children based on parent
  children.push_back(parent); // parent might be a better fit than any of its children
  breed(children); // allow children to reproduce
  parent = cull(children); // replace parent with the best fit of it's children (savage!)
  return;
}

vector<string> spawn(const string scrambled){
  // given a string, populate and return a vector with mutated children based on it
  const int litterSize = 99;
  vector<string> children = {};

  for (int i = 0; i < litterSize; i++){
    children.push_back( mutateString(scrambled) );
  }

  return children;
}

string mutateString(const string parent){
  static std::default_random_engine gen; //initialize a rand generator as static (one per program) 
  static std::uniform_int_distribution<int> dist(0,100); // force random numbers to fit 0-100 distribution
  const int mutateChance = 5; // percent chance of mutation
  vector<char> alphanumerics = getAlphanumerics();
  string mutated = parent; // child string begins as an unaltered copy of parent string

  int dice  = 0;
  for (auto& eachletter : mutated){
    dice = dist(gen); // roll the dice
    if (dice < mutateChance) {
      eachletter =  *select_randomly(alphanumerics.begin(), alphanumerics.end()) ;
    }
  }
  return mutated;
}

string breedString(const string mom, const string dad){
  static std::default_random_engine gen; //initialize a rand generator as static (one per program) 
  static std::bernoulli_distribution coin(0.5); // force random numbers to fit true/false distribution with equal likelihood
  string baby = ""; // child string begins as blank

  string::const_iterator momIt = mom.begin();
  string::const_iterator dadIt = dad.begin();

  for (; momIt < mom.end(); momIt++, dadIt++){ // fault if dad is shorter than mom
    coin(gen) ? baby.push_back(*momIt) : baby.push_back(*dadIt);
    }
  return baby;
}


string cull(vector<string> children){
  // from a vector of many children, select the one which has the lowest score
  // it's nice when the language has exactly an algorithm to do the task you need
  // we only need to pass a function to tell it how to do the comparison
  return *std::min_element(children.begin(),children.end(),compareScores);
}


const int score(const string candidate){

  const string target("Methinks it is like a weasel"); //need to find a way not to duplicate this

  string::const_iterator it = candidate.begin();
  string::const_iterator targetit = target.begin();
  int score = 0;

  for (; it < candidate.end(); it++, targetit++){
    if (*it != *targetit){
      //cout << "Score: " << getDistance(*it,*targetit) << endl;
      //score += getDistance(*it,*targetit);
      score += 1;
    }
  }
  return score;
}

const int getDistance(const char firstchar, const char secondchar){
  // Currently unused but might be used in the future
  // Currently score works by checking if each letter is right or wrong in a yes/no fashion
  // if you wanted to get fancier, you could determine how close a letter is to correct
  // this function attempts to do that.
  // Does not properly handle Z->A comparisons
  vector<char> alphabet = getAlphanumerics();
  vector<char>::iterator firstit, secondit;
  firstit = find(alphabet.begin(), alphabet.end(), firstchar);
  secondit = find(alphabet.begin(), alphabet.end(), secondchar);
  return abs( distance(firstit,secondit) );
}

void breed(vector<string> parents){
  auto cutoff = std::next(parents.begin(), parents.size()/3); // iterator to 1/3rd of the vector
  std::partial_sort(parents.begin(), cutoff, parents.end(), compareScores); //sort the first 3rd

  std::vector<string>::iterator ita = parents.begin();
  std::vector<string>::iterator itb = parents.begin()+1;

  for (; ita < cutoff; ita++, itb++){
    parents.push_back( breedString( *ita, *itb ) );
  }
  return;
}

