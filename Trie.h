
#ifndef _MY_TRIE_H
#define _MY_TRIE_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>  //used to read files
#include <cctype> //used for toLower and isalpha

using namespace std;


class Trie {

  protected:
    // private/protected stuff goes here!
    //  - protected allows this class to be inherited
    //    and adapted to the problem at hand (if needed)
    //   typedefs
    //   data members
    //   definition of trie node structure
    //   private helper functions
    //   etc.
    struct NODE{
      NODE* letters[26];
      bool isEndOfWord;
      char let;
    };

    int numWords; 
    char orderedLets[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    bool fileRead;
    NODE* root;

    //returns false if 'let' is an invalid character
    bool letterValid(char let) const{ 
      //cout << "isalpha returns: " << isalpha(let) << endl;

      if(isalpha(let) == 0)
        return false;

      return true;
    }

    //if any subtree in trie contains a word, put true inside b
    void containWords(NODE* t, bool& b){
      if(t == nullptr){return;}
      if(t->isEndOfWord){
        b = true;
        return;
      }
      if(b) //if b already decided, return
          return;
        
      for(char c: orderedLets){
        containWords(t->letters[c%26], b);
      } 
    }



    //traverses through the trie and adds words into vector 'v', called from the words function.
    void traversalHelper(NODE* t, string& word, vector<string>& v) const{
        if(t == nullptr){return;}
          
        if(t->isEndOfWord){
          v.push_back(word + t->let);
        }
        
        for(char c: orderedLets){ //traverse through in alphabetical order
          word = word + t->let;
          traversalHelper(t->letters[c%26], word, v);
          word.pop_back();
        }
    }

    //deletes all children starting from Trie t, called from deconstructor and clear
    void deleteChildren(NODE* t){

        if(t == nullptr)  {return;}

        for(char c: orderedLets){
          deleteChildren(t->letters[c%26]);
        }
        //cout  << "let: " << t->let << endl;
        delete t;
        return;
    }

  public:
    /**
    * constructor and destructor
    */
    Trie()   {    
      numWords = 0;
      root = new NODE;

      for(int i = 0; i < 26; i++) {root->letters[i] = nullptr;}

      root->isEndOfWord = false;
      root->let = 0;
      fileRead = false;
      
    }


    //deletes everything, including root
    ~Trie(){
      deleteChildren(root);
    }

    /*
     * function: getFromFile
     * description:  extract all of the words from the file
     *   specified by the filename given in the parameter.
     *   Words are separated by whitespace characters and 
     *   must only contain letters.
     *
     * return:  indicates success/failure (file not readable...)
     */
    bool getFromFile(string filename){
      ifstream file(filename);
      string word;
      if(!file.is_open()){
        return false;
      }
      
      while(file >> word){
        //word = word.substr(0, word.find_first_of(" "));
        insert(word);
      }
      fileRead = true;
      return true;
    }

    /*
     * function: insert 
     * description:  inserts the word given by the parameter
     *   into the trie data structure.
     *   
     *   fails if word already exists or contains non-letters
     *
     * return:  indicates success/failure
     */
    bool insert(string word){

      int size = word.length();
      NODE* tmp;
      NODE* parent = root;
      char let;

      //traverse through word and trie
      for(int i = 0; i < size; i++){
        let = word[i];
        let = tolower(let);

        if(!letterValid(let))  //if the string contains an invalid character, return false
         { return false;}
         
        tmp = parent->letters[let%26]; 

        if(tmp == nullptr){ //new trie node if one doesn't exist
          tmp = new NODE();    
          tmp->let = let;
          parent->letters[let%26] = tmp;
          tmp->isEndOfWord = false;
          for(int i = 0; i < 26; i++){tmp->letters[i] = nullptr;}
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
     * function: search 
     * description:  determines if the word given by the parameter
     *   is stored in the trie data structure.
     *   
     *   fails if word already does not exist
     *
     * return:  indicates success/failure
     */
    bool search(string word) const{
      int size = word.length();
      NODE* tmp;
      NODE* parent = root;
      char let;

      //traverse through word and trie
      for(int i = 0; i < size; i++){
        let = word[i];
        let = tolower(let);

        if(!letterValid(let))  //if the string contains an invalid character, return false
          return false;
        
        tmp = parent->letters[let%26]; 
        if(tmp == nullptr)
          return false;

        parent = tmp;
      }
      if(tmp->isEndOfWord)  //if word ie endOfWord, return true
        return true;

      return false; //otherwise, false
    }

    /*
     * function: remove 
     * description:  removes the word given by the parameter
     *   from the trie data structure.  Only deallocate nodes
     *   in subtrees that do not contain any words.
     *   
     *   succeed if word exists and is properly removed
     *
     * return:  indicates success/failure
     */
    bool remove(string word){

      int size = word.length();
      NODE* tmp;
      NODE* parent = root;
      NODE* reseter; 
      int whereTmp;
      char let;

      for(int i = 0; i < size; i++){
        
        let = word[i];
        let = tolower(let);
        if(!letterValid(let))  //if the string contains an invalid character, return false
            return false;
      
        tmp = parent->letters[let%26];
        if(tmp == nullptr)  //word doesn't exist
          return false;
        reseter = parent;
        whereTmp = let%26;
        parent = tmp;
      }

      if(!tmp->isEndOfWord)//if not end of word, return false
        return false;

      //Now, check to see if seach subtree contains words
      //if subtree does contain words, don't delete, otherwise, delete that subtree
      bool b = false;
      for(char c: orderedLets){
        containWords(tmp->letters[c%26], b);
        if(b){
          return false;
        }
      }
      deleteChildren(tmp);
      reseter->letters[whereTmp] = nullptr; //prevent accessing deleted tmp
      numWords--;
      return true;
    }

    /*
     * function: clear
     * description:  remove all words from the trie and deallocate
     *   all nodes.
     *
     * return:  indicates success/failure
     */
    bool clear(){
      for(int i = 0 ; i < 26; i++){ //delete everything except root node
        deleteChildren(root->letters[i]);
      }
      fileRead = false;
      for(int i = 0; i < 26; i++){root->letters[i] = nullptr;}  //reset 'letters' array values to nullptr
      return true;
    }



    /*
     * function: wordCount
     * description:  return the number of words currently stored in the trie
     *
     * return:  an integer containing the number of words stored
     *
     * comment/note:  this should have an O(1) runtime.
     */
    int wordCount( ) const{
      return numWords;
    }    


    /*
     * function: words
     * description:  build a vector of all words in the dictionary and return that
     *   vector.  The words are to be in sorted ascending order.
     *
     * return:  a pointer to a vector of strings
     *
     */
    std::vector<string>* words( ) const{
      vector<string> *v = new vector<string>();
      string word = "";
      traversalHelper(root, word, *v);
      return v;
    }

    bool getFileRead(){return fileRead;}
    


};

#endif
