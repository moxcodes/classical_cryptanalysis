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

int main(int argc, char* argv[])
{
  if(argc!=2)
    return 1;
  std::vector<int> totalnGraphs;
  int totalWords;
  Dicttree* ngraphFreqTree = new Dicttree();
  std::ifstream corpus(std::string(argv[1]).c_str());
  std::string line;
  int i = 0;
  std::string word;
  while(getline(corpus,line))
    {
      while(i<line.size())
	{
	  while(i<line.size() && isUncleanAlpha(line.at(i)))
	    {
	      word+=line.at(i);
	      i++;
	    }
	  word=CleanString(word);
	  if(word!="")
	    {
	      ngraphFreqTree->addAndIncrement(word,true);
	      totalWords++;
	      while(totalnGraphs.size()<word.size())
		totalnGraphs.push_back(0);
	      for(int j=word.size();j>0;j--)
		  totalnGraphs.at(i)+=(word.size()-i+1);
	    }
	  while(i<line.size() && !isUncleanAlpha(line.at(i)))
	    i++;
	}
    }
  //tree is populated, but not normalized
  ngraphFreqTree->normalize(totalnGraphs,totalWords,0);
  ngraphFreqTree->PrintWordListFreq("");
}
