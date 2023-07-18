spellb : spellb.cpp SBTrie.h Trie.h
	g++ -std=c++11 -o spellb spellb.cpp 


clean:
	rm -f spellb *.o

