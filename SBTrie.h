
#ifndef _MY_SB_TRIE_H
#define _MY_SB_TRIE_H

#include "Trie.h"


class SBTrie: public Trie {

  // the main reason for inheriting the Trie class is that the words() method in Tries
  // is quite inefficient for the use of the Spelling Bee game.
  //
  // This allows us to customize a better method to get the desired words by "adding an
  // addtional method" on the the Trie class at the cost of using inheritance.

  protected:
    // private/protected stuff goes here!
    //  - protected allows this class to be inherited
    //    and adapted to the problem at hand (if needed)
    //   typedefs
    //   data members
    //   definition of trie node structure
    //   private helper functions
    //   etc.
    char centralLet;
    char allowedLetters[6];
    int score;
    bool bingo;
    int bingoTrack[7]; //keeps track of the words that have started with different letters -- bingo tracker
    NODE* rootFound;  //keeps track of words found
    bool pangram;
    int wordsFound;



    //checks if a word is valid using allowed letters and central letter, returns true if it is. DOESN'T check if centralLETTER has to be in word
    bool wordValid(const string& word) const{
      bool b;
      if(centralLet == 0){return false;}  //if letters haven't been set up, return false
      for(char c: word){
        b = false;

        //if char in word is equal to at least one allowed letter, then that letter is valid; do for all letters in word -- if one is false, return false
        for(int i = 0; i< 6; i++){
          if(c == allowedLetters[i]) 
            b = true;
        }
        if(!b){
          if(c == centralLet){continue;}    //if letter is the central letter, then letter is allowed; move on to next letter
        
          return false;
        }
      }
      return true;
    }

    bool letterAllowed(char c) const{
        if(c == centralLet)
          return true;

        for(int i = 0; i < 6; i++){
          if(c == allowedLetters[i])
            return true;
        }

        return false;
    }

    bool containCentralLetter(const string& word) const{
      for(char c: word){
        if(c == centralLet)
          return true;
      }
      return false;
    }

     //traverses through the trie and adds words into vector 'v' if VALID word, called from the sbWords function.
    void traversalValidHelper(NODE* t, string& word, vector<string>& v) const{
        if(t == nullptr){return;}  //if nullptr or invalid word, invalid characters, return 


        if(t->isEndOfWord && word.length()+1 >= 4 && wordValid(word+t->let)  && containCentralLetter(word + t->let)){
          //check if centralLetter
          v.push_back(word + t->let);
        }
        
        for(char c: orderedLets){ //traverse through in alphabetical order     
         word = word + t->let;       
        if(!letterAllowed(t->let)) {word.pop_back();  continue;}  //if letter is invalid, go to the next char
          traversalValidHelper(t->letters[c%26], word, v);
          word.pop_back();
        }
    }

    

    void updateBingo(char c){
      bingoTrack[c%7]++;
    }
    

  public:
    /**
    * constructor and destructor
    */
    SBTrie()   {
      score = 0;
      wordsFound = 0;
      centralLet = 0;
      allowedLetters[0] = 0;
      bingo = false;
      for(int i = 0; i < 7; i++) {bingoTrack[i] = 0;}

      rootFound = new NODE;
      for(int i = 0; i < 26; i++){rootFound->letters[i] = nullptr;}
      rootFound->isEndOfWord = false;
      rootFound->let = 0;
      pangram = false;

    }
    ~SBTrie(){
      // your destructor code here.
      deleteChildren(rootFound);
    }

    int getScore(){
      return score;
    }

    bool checkBingo(){
      for(int i: bingoTrack){
        if(i < 1){  //if no word has started with this character once, no bingo
          return false;
        }
      }
      bingo = true; //update bingo to true
      return true;
    }

    //FUNC: returns letters allowed, including central
    bool getLetters(char* arr){

      //check if letters are not null (are initialized)
      if(centralLet == 0 || allowedLetters[0] == 0)  //return false -- no letters set up
        return false;                                         

      //display central letter and then the rest
      arr[0] = centralLet;
      
      for(int i = 1; i < 6; i++)  {arr[i] = allowedLetters[i];}
      return arr;
    }

    bool setUpLet(char centralLet, char allowedLetters[6]){
      //reset variables
      bingo = false;
      pangram = false;
      score = 0;
      wordsFound = 0;

      //check to see if letters repeat
      int repLet[26] = {0};
      repLet[centralLet%26]++;
      char c;
   
      for(int i = 0; i < 6; i++){
        c = tolower(allowedLetters[i]);

        repLet[c%26]++;
        if(repLet[c%26] > 1){return false;}
        
      }

      this->centralLet = centralLet;
      for(int i = 0; i < 6; i++){
        this->allowedLetters[i] = allowedLetters[i];
      }
      score = 0;
      bingo = false;
      pangram = false;
      return true;
    }

   
    //FUNC: reads word attempted and returns int telling the main program how to handle the error/success
    int attemptWord(string word){

      if(!containCentralLetter(word)) 
          return -2;  //missing central letter
    
      if(!wordValid(word)){
        return -3;
      }

      //check if word contains valid characters
     int length = word.length();
        if(length < 4)  //word too short
          return -1;

      //check if word has valid letters
      for(int i = 0; i < length; i++){
        if(!letterValid(word[i])) 
          return -3;
      }


      if(search(word)){ //if word found
        //check if word has not already been entered, if not, insert

        if(!insertWord(word)) 
          return -5;

        if(length >= 7) //pangram
         { score += length; if(checkPangram(word)) {pangram = true; score += 7;}}
        else if(length > 4)
          score += length;
        else 
          score++;  //four letter word

        updateBingo(word[0]);
        wordsFound++;
        return 1; //success
      }
      return -4;  //word not found in dictionary
    }

    //checks if word is a pangram, if true, then return true
    bool checkPangram(string word){
      int length = word.length();
      if(length < 7)
        return false;
      
      if(!wordValid(word))
        return false;

      if(word.find(centralLet) == string::npos)
        return false;

      for(int i = 0; i < 6; i++){
        if(word.find(allowedLetters[i]) == string::npos)
          return false;
      }
      return true;      
    }

    //assume words are valid
    bool insertWord(string word){
      int size = word.length();
      NODE* tmp;
      NODE* parent = rootFound;
      char let;

      //traverse through word and trie
      for(int i = 0; i < size; i++){
        let = word[i];
        let = tolower(let);
         
        tmp = parent->letters[let%26]; 

        if(tmp == nullptr){ //new trie node if one doesn't exist
          tmp = new NODE();    
          for(int i = 0; i < 26; i++){tmp->letters[i] = nullptr;}
          tmp->let = let;
          parent->letters[let%26] = tmp;
          tmp->isEndOfWord = false;
        }
        parent = tmp;
      }

      if(tmp->isEndOfWord)  //if word already exists, return false
          return false;

      numWords++;
      tmp->isEndOfWord = true;  //otherwise, set it as end of word and return true
      return true;
    }

    /*
     * function: sbWords
     * description:  build a vector of all words in the dictionary that
     *   are valid words for the spelling bee problem and return that
     *   vector.  The words are to be in sorted ascending order.
     *   The words included must have a length of 4 or greater, 
     *                      must contain the central letter, and 
     *                      may contain the allowed letters 
     *
     * parameter: char centralLetter - the letter that MUST be contained in the words
     *            string letters - the other letters that are allowed to be in words
     *
     * return:  a pointer to a vector of strings
     *
     */
    std::vector<string>* sbWords(char centralLetter, string letters) const{
      vector<string>* v = new vector<string>();
      string word = "";
      for(char c: orderedLets){
        traversalValidHelper(root->letters[c%26], word, *v);
        word = "";
      }
      
      return v;
    }

    //return centralLet
    char getCentralLetter() {return centralLet;}

    //return allowedLetters, not including central
    char* getOtherLetters() {return allowedLetters;}


    //FUNC: returns vector of words found
    std::vector<string>* guessedWords(){
      vector<string> *v = new vector<string>();
      string word = "";
      traversalHelper(rootFound, word, *v);
      return v;
    }

    //clears children of rootFound
    void clearFound(){
       for(int i = 0 ; i < 26; i++){ //delete everything except root node
        deleteChildren(rootFound->letters[i]);
      }
      for(int i = 0; i < 26; i++){rootFound->letters[i] = nullptr;}  //reset 'letters' array values to nullptr
    }


    bool getBingo(){return bingo;}

    bool getPangram(){return pangram;}

    int getWordsFound(){return wordsFound;}

};

#endif
