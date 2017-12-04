#include <string>
#include <iostream>
#include <stdlib.h>
#include "dicttree.h"
#include <fstream>
#include <stdio.h>


std::string CleanString(std::string generalString);
std::vector<std::string>* ImportWordList(std::string filename);

Dicttree* simpleTree;
Dicttree* bigTree;
Dicttree* doubleTree;
//Dicttree* outputTree;

std::string CleanString(std::string generalString)
{
  std::string nuString="";
  for(int i=0;i<generalString.size();i++)
    {
      if((int)generalString.at(i)>=(int)'a' && (int)generalString.at(i)<=(int)'z')
	nuString+=generalString.at(i);
      if((int)generalString.at(i)>=(int)'A' && (int)generalString.at(i)<=(int)'Z')
	nuString+=(char)((int)generalString.at(i)+ 32);
    }
  return nuString;
}


//get the english wordlist
std::vector<std::string>* ImportWordList(std::string filename)
{
  std::ifstream wordFile(filename.c_str());
  std::vector<std::string>* wordList= new std::vector<std::string>();
  std::string word;
  while(getline(wordFile,word))
    {
      wordList->push_back(CleanString(word));
    }
  wordFile.close();
  return wordList;
}


void subSolve(std::string puzzle)
{
}
