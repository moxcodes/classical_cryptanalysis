#include <string>
#include <iostream>
#include <stdlib.h>
#include "dicttree.h"
#include <fstream>

using namespace dicttree;

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


std::vector<std::string>* ImportWordList(std::string filename)
{
  std::ifstream wordFile(filename.c_str());
  std::vector<std::string>* wordList=new std::vector<std::string>();
  std::string word;
  while(getline(wordFile,word))
    {
      wordList->push_back(CleanString(word));
    }
  wordFile.close();
  return wordList;
}

bool isUncleanAlpha(char c)
{
  if(((int)c >=(int)'a'&&(int)c <=(int)'z') || ((int)c >=(int)'A'&(int)c <=(int)'Z')||c=='\'')
    return true;
  return false;
}

int main(int argc,char* argv[])
{
  if(argc!=3)
    return 1;
  Dicttree* doubletree = new Dicttree();
  std::ifstream corpus(std::string(argv[1]).c_str());
  std::vector<std::string>* wordList=ImportWordList(std::string(argv[2]));
  Dicttree* simpletree = new Dicttree(wordList);
  std::string line;
  int i = 0;
  std::string prevWord = "";
  std::string currWord = "";
  //TODO: incorporate frequencies into the dicttree type
  while(getline(corpus,line))
    {
      i=0;
      while(i<line.size())
	{
	  while(i<line.size() && isUncleanAlpha(line.at(i)))
	    {
	      currWord+=line.at(i);
	      i++;
	    }
	  currWord=CleanString(currWord);
	  if(currWord!="" && prevWord!="")
	    {
	      if(simpletree->contains(currWord) && simpletree->contains(prevWord))
		  doubletree->add(prevWord+currWord);
	      prevWord=currWord;
	      currWord="";
	    }
	  if(currWord!=""&&prevWord=="")
	    {
	      prevWord=currWord;
	      currWord="";
	    }
	  while(i<line.size() && !isUncleanAlpha(line.at(i)))
	    i++;
	}
    }
  //The tree is populated, print out the aggregated biwords
  doubletree->PrintWordList("");
  return 0;
}
