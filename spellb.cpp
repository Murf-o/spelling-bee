//  Sebastian Barroso 
//  UIN: 661337995
//  CS 351 Spring 2023

#include "SBTrie.h"
#include "Trie.h"

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>

using std::cout;
using std::cin;
using std::string;

void getNewDictionary(SBTrie *sbt, string filename){
  if(sbt->getFileRead()){sbt->clear();} //if already read a file, clear it
  
  if(!sbt->getFromFile(filename)){cout << "file could not be open." << endl;}
}

void updateDictionary(SBTrie *sbt, string filename){
  // enter needed code here for command 2
  sbt->getFromFile(filename);
}

void setupLetters(SBTrie *sbt, string letters){
  // enter needed code here for command 3
  int length = letters.length();
  //if not enough letters
  if(length < 7) {cout << "Insufficient amount of letters." << endl; return;}

  //Otherwise

  //parse letters into central letter and other letters
  int start;
  char centralLetter;
  for(start = 0; start < length; start++){ //find central letter
  char c = letters[start];
    if(isalpha(c))  {  centralLetter = letters[start]; break;}
  }

 int index = 0;
  char lettersArr[6] = {0};
  for(int i = start+1;i < length; i++){  //get other letters
    char c = letters[i];
      if(!isalpha(c)){  //skip to next letter
          continue;
        }
      
      if(index > 5){cout << "Invalid letter set" << endl; return;}
      lettersArr[index] = letters[i];
      index++;
    }

    if(lettersArr[5] == 0) {cout << "Invalid letter set" << endl; return;}

  if(!sbt->setUpLet(centralLetter, lettersArr)){cout << "Invalid letter set" << endl; return;}
  sbt->clearFound();
  
}

void showLetters(SBTrie *sbt){
  // enter needed code here for command 4
  char arr[7];
  
  //if no letters
  if(!sbt->getLetters(arr)) {cout << "set up the letters allowed first!" << endl;    return;}

  //if there are letters
  cout << "Central Letter: " << arr[0] << endl;
  cout << "6 Other Letters: ";
  for(int i = 1; i < 6; i++){cout << arr[i] << ',';}
  cout << arr[6] << endl;
}

void attemptWord(SBTrie *sbt, string letters){
  // enter needed code here for command 5
  int length = letters.length();
  if(length < 4) {cout << "word is too short" << endl; return;}

  int handler = sbt->attemptWord(letters);

  if(handler == 1){
    int size = letters.length();
  
    if(size >= 7 && sbt->getPangram()) { cout << "found " << letters << " " << length << " points, total " << sbt->getScore() << " points, Pangram found " << endl;}
    
    else if(size > 4){ cout << "found " << letters << " " << length << " points, total " << sbt->getScore() << " points";}

    else{
      cout << "found " << letters << " 1 point, total " << sbt->getScore() << " ";
      if( sbt->getScore() == 1){cout << "point";}
      else{cout << "points";}
    }
    if(sbt->checkPangram(letters) && !sbt->getBingo() && sbt->checkBingo()) {cout << ", Pangram found, Bingo scored" << endl;}  //if a new bingo and pangram has occured at the same time
    else if(!sbt->getBingo() && sbt->checkBingo())  {cout << ", Bingo scored" << endl;} //just bingo
    else{cout << endl;}

  }
  else if(handler == -2)  {cout << "word is missing central letter" << endl;}
  else if(handler == -3)  {cout << "word contains invalid letter" << endl;}
  else if(handler == -4)  {cout << "word is not in the dictionary" << endl;}
  else  {cout << "word has already been found" << endl;}
  return;
}

void showFoundWords(SBTrie *sbt){
  // enter needed code here for command 6

    vector<string>* v = sbt->guessedWords();
    for(string str: *v){
      cout << str << "\n";
    }
    cout << sbt->getWordsFound() << " word(s) found, total " << sbt->getScore() << " ";
    if(sbt->getScore() == 1) {cout << "point";}
    else  {cout << "points";}
    if(sbt->getBingo() && sbt->getPangram()) {cout << ", Pangram found, Bingo scored" << endl;}
    else if(sbt->getBingo()) {cout << ", Bingo scored" << endl;}
    else if(sbt->getPangram()) {cout << ", Pangram found" << endl;}
    cout << endl;
}

void showAllWords(SBTrie *sbt){
  // enter needed code here for command 7
  
  vector<string>* v;

  if(sbt->getCentralLetter() == 0)  //letters aren't initialized. return
    return;

  v = sbt->sbWords(sbt->getCentralLetter(), sbt->getOtherLetters());
  int col;

  for(string str: *v){
    col = str.length();
    cout << str;
    //print up to col 20
    if(col < 17){while(col < 20) {cout << " "; col++;}}
    else{ cout << " ";}
    
    cout << str.length();
    if(sbt->checkPangram(str))  {cout << " Pangram\n";}
    else                         {cout << "\n";}
  }
  fflush(stdout);
}

void displayCommands(){
  cout << "\nCommands are given by digits 1 through 9\n\n";
  cout << "  1 <filename> - read in a new dictionary from a file\n";
  cout << "  2 <filename> - update the existing dictionary with words from a file\n";
  cout << "  3 <7letters> - enter a new central letter and 6 other letters\n";
  cout << "  4            - display current central letter and other letters\n";
  cout << "  5 <word>     - enter a potential word\n";
  cout << "  6            - display found words and other stats\n";
  cout << "  7            - list all possible Spelling Bee words from the dictionary\n";
  cout << "  8            - display this list of commands\n";
  cout << "  9            - quit the program\n\n";

}

int main(int argc, char** argv){
  SBTrie *sbt = new SBTrie;

  cout << "Welcome to Spelling Bee Game\n";

  displayCommands();
  
  bool done = false;
  string line;
  string input;

  do {
    cout << "cmd> ";

    // read a complete line
    std::getline(std::cin, line);

    // now create a "stringstream" on the line just read
    std::stringstream ss(line);

    // clear input from any previous value
    input = "";
    // get command character
    char command;
    ss >> command;
    //cout << "Debug command:" << command << "***\n";
    
    if(command == '1'){
        ss >> input;
        //cout << std::setw(17) << input << " " << std::right << std::setw(2) << input.length() << std::left << endl;
        //cout << "Debug 1:" << input << "***\n";
        getNewDictionary(sbt, input);
    }

    if(command == '2'){
        ss >> input;
        //cout << "Debug 2:" << input << "***\n";
        updateDictionary(sbt, input);
    }
        
    if(command == '3'){
        ss >> input;
        //cout << "Debug 3:" << input << "***\n";
        setupLetters(sbt, input);
    }

    if(command == '4'){
        showLetters(sbt);
    }

    if(command == '5'){
        ss >> input;
        //cout << "Debug 5:" << input << "***\n";
        attemptWord(sbt, input);
    }

    if(command == '6'){
        showFoundWords(sbt);
    }

    if(command == '7'){
        showAllWords(sbt);
    }

    if(command == '8' || command == '?'){
        displayCommands();
    }

    if(command == '9' || command == 'q'){
        done=true;
    }
  } while(!done && !cin.eof());
  delete sbt; //delete sbt
  return 0;
}
