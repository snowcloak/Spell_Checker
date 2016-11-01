#include "DoubleProbing.h"
#include "QuadraticProbing.cpp"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <stdio.h>
#include <ctype.h>
using namespace std;

template <typename HashTableType>
void
SpellCheck(HashTableType &hashy, const string &dictionaryfile, const string &documentfile) {
  hashy.makeEmpty();  // Do not have to do it if you pass empty hash table.

    //create a file reading object
    //create dictionary
    ifstream WordFile(dictionaryfile);
    string line;
    bool t;
    //while loop until reaches end of file
    while (WordFile>>line){
    hashy.insert(line); //inserting words into hash table
    }
    WordFile.close();

    ifstream SearchFile(documentfile);
    string linet;
    //while loop until reaches end of file
    while(SearchFile>>linet){
    //consults dictionary
    t = hashy.contains(linet);
    //spell checker
    if(!t) {
            cout<<linet<<endl;
            cout<<"incorrect spelling"<<endl;
            string word_mod = linet;
            for(size_t j=0; j<word_mod.length(); j++)
                {  for(size_t k=97; k<=122; k++) //Switches letter
                   {  char c = static_cast<char>(k);
                   /* a) Character change in each possible position */
                      word_mod[j] = c;
                      if (hashy.contains(word_mod))
                      cout << " (case a) ~> " << word_mod << "\n";
                      word_mod = linet;
                   /* a2) Adds one character in each possible position */
                      word_mod.insert(j,1,c);
                      if (hashy.contains(word_mod))
                      cout << " (case a) ~> " << word_mod << "\n";
                      word_mod = linet;
                   }//end for(k<=122)
                   /* b) Removes one character in each possible position */
                   word_mod.erase(word_mod.begin()+j);
                   if (hashy.contains(word_mod))
                       cout << " (case b) ~> " << word_mod << "\n";
                   word_mod = linet;
                   /* c) Swaps adjacent characters in the word */
                   char temp = word_mod[j];
                   word_mod[j] = word_mod[j+1];
                   word_mod[j+1] = temp;
                   if (hashy.contains(word_mod))
                       cout << " (case c) ~> " << word_mod << "\n";
                   word_mod = linet;
                }//end of for loop
	}//end of spell checker
    }
    SearchFile.close();
}

// Sample main for program SpellCheck
int main(int argc, char **argv) {
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " <documentfile>" << endl;
    return 0;
  }
  string document_filename(argv[1]);
  string dictionary_filename("WordsEn.txt");
  cout << "I will run " << argv[0] << endl;
  cout << "Spell Checking: " << document_filename << endl;
  DoubleHashTable<string> double_hashy;
  SpellCheck(double_hashy, dictionary_filename, document_filename);

  return 0;
}
