#include <string>
#include <stdlib.h>
#include <iostream>
#include "dicttree.h"
#include <fstream>


using namespace dicttree;

Dicttree::Dicttree(){
  for(int i=0;i<26;i++){
    children[i]=NULL;
  }
  isLeaf=false;
  hasChildren=false;
}

//In this function, we will assume that the strings are all well-formatted (no caps, no punctuation)
Dicttree::Dicttree(std::string wordfile, int nGraphDepth){
  isLeaf=false;
  hasChildren=false;
  for(int i=0;i<26;i++){
    //Assemble a sub-wordlist
    children[i]=NULL;
  }

  std::vector<int> totalnGraphs;
  int totalBiWords = 0;
  std::ifstream corpus(wordfile.c_str());
  std::string pullLine;
  std::string line1 = "";
  std::string line2 = "";
  std::string cleanLine1;
  std::string cleanLine2;
  int i = 0;
  int wordi = 0;
  std::string word1;
  std::string word2;
  bool EOF;
  int linecount=0;
  //get a string of at least 200 characters when all appended together
  while(line1.size()<200 && !EOF){
    EOF=getline(corpus,pullLine);
    line1+=pullLine;
  }
  
  cleanLine1=CleanString(line1);
  //... and get a second, for when we overflow. 
  while(line2.size()<200 && !EOF){
    EOF=getline(corpus,pullLine);
    line2+=pullLine;
  }
  cleanLine2=CleanString(line2);
  // as soon as start character goes beyond the end of first, transfer second to first, pull a new 200 char min second. Due to being a pain, we will sacrifice the last few lower ngraphs
  while(!(EOF && cleanLine1=="" && cleanLine2 == ""))
    {
      wordi=0;
      int l=0;
      while(wordi<line1.size() && l<line1.size())
	{
	  word1="";
	  word2="";
	  int l=wordi;
	  while(l<line1.size() && isUncleanAlpha(line1.at(l)))
	    {
	      word1+=line1.at(l);
	      l++;
	    }
	  wordi=l;
	  while(l<line1.size() && isUncleanAlpha(line1.at(l)))
	    {
	      word2+=line1.at(l);
	      l++;
	    }
	  if(word1!="" && word2!="")
	    {
	      totalBiWords++;
	      addBiWord(word1,word2);
	    }
	}
      i=0;
      while(i < cleanLine1.size() && i < cleanLine1.size()+cleanLine2.size() - nGraphDepth)
	{
	  std::string ngraph="";
	  for(int len=1;len<=nGraphDepth;len++)
	    {
	      for(int j = i;j<cleanLine1.size()&&j<i+len;j++)
		ngraph+=cleanLine1.at(j);
	      for(int j = 0;j<i+len-cleanLine1.size();j++)
		ngraph+=cleanLine2.at(j);
	      addNGraph(ngraph);
	    }
	  i++;
	}
      line1=line2;
      cleanLine1=CleanLine2;
      while(line2.size()<200 && !EOF){
	EOF=getline(corpus,pullLine);
	line2+=pullLine;
      }
      cleanLine2=CleanString(line2);
    }
  std::cout<<totalBiWords<<"\n";
  normalize(totalnGraphs,totalBiWords,0);
}


Dicttree::~Dicttree(){
  for(int i=0;i<26;i++)
    {
      if(children[i]!=NULL)
	{
	  children[i]->~Dicttree();
	  free(children[i]);
	}
    }
}

std::string Dicttree::CleanString(std::string generalString)
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



bool Dicttree::isUncleanAlpha(char c)
{
  if(((int)c >=(int)'a'&&(int)c <=(int)'z') || ((int)c >=(int)'A'&(int)c <=(int)'Z')||c=='\'')
    return true;
  return false;
}



bool Dicttree::contains(std::string word){
  if(word==""&&isLeaf)
    return true;
  if(word==""&&!isLeaf)
    return false;
  if(children[((int)word.at(0)) -97]!=NULL)
    return children[((int)word.at(0)) -97]->contains(word.substr(1));
  return false;
}

double Dicttree::getFrequency(std::string ngraph){
  if(ngraph=="")
    return frequency;
  else
    {
      if(children[(int)ngraph.at(0)-97]!=NULL)
	return children[(int)ngraph.at(0)-97]->getFrequency(ngraph.substr(1));
      else
	return 0;
    }
}

void Dicttree::addWord(std::string word){
  breakFrequency=breakFrequency+1;
  if(word=="")
    {
      isWordTerminus=true;
      wordFrequency=wordFrequency+1;
      return;
    }
  if(children[((int)word.at(0)) -97]==NULL)
      children[((int)word.at(0)) -97] = new Dicttree();
  
  children[((int)word.at(0)) -97]->addWord(word.substr(1));
  return;  
}

void Dicttree::addBiWord(std::string firstWord; std::string secondWord){
  breakFrequency=breakFrequency+1;
  if(firstword=="")
    {
      isWordTerminus=true;
      wordFrequency=wordFrequency+1;
      subWordTree = new Dicttree();
      subWordTree->addWord(secondWord);
      return;
    }
  if(children[((int)firstWord.at(0)) -97]==NULL)
      children[((int)firstWord.at(0)) -97] = new Dicttree();
  
  children[((int)firstWord.at(0)) -97]->addBiWord(firstWord.substr(1));
  return;
}

void Dicttree::addNGraph(std::string nGraph){
  if(nGraph=="")
    {
      nGraphFrequency=nGraphFrequency+1;
      return;
    }
  if(children[((int)word.at(0)) -97]==NULL)
      children[((int)word.at(0)) -97] = new Dicttree();
  
  children[((int)word.at(0)) -97]->addNGraph(nGraph.substr(1));
  return;
  
}


void Dicttree::normalize(std::vector<int> totalnGraphs, int totalWords, int depth){
  frequency=frequency/totalnGraphs.at(depth);
  if(isLeaf)
    {
    if(wordFrequency>0)
	  wordFrequency=wordFrequency/totalWords;
    }
  
  for(int i=0;i<26;i++)
    {
      if(children[i]!=NULL)
	children[i]->normalize(totalnGraphs,totalWords,depth+1);
    }
  
}

void Dicttree::PrintTree(std::string prepend){
  for(int i=0;i<26;i++)
    {
      //      std::cout<<"Tracing..."<<(char)(i+97)<<'\n';
      if(children[i]!=NULL)
	{
	  std::cout << prepend << (char)(i+97);
	  if(children[i]->isLeaf){
	    std::cout<<"** "<< children[i]->frequency <<'\n';
	  }
	  else
	    {
	      std::cout<<" "<< children[i]->frequency << '\n';
	    }
	  std::string nuPrepend=prepend+' ';
	  children[i]->PrintTree(nuPrepend);
	}
    }
  return;
}

void Dicttree::PrintWordList(std::string prepend){
  for(int i=0;i<26;i++)
    {
      //      std::cout<<"Tracing..."<<(char)(i+97)<<'\n';
      if(children[i]!=NULL)
	{
	  if(children[i]->isLeaf){
	    std::cout << prepend << (char)(i+97)<<'\n';
	  }
	  std::string nuPrepend=prepend+((char)(i+97));
	  children[i]->PrintWordList(nuPrepend);
	}
    }
  return;
}

void ExportTree(std::string dictFile)
{
  std::ofstream dictStream;
  dictStream.open(dictFile);
  ExportRecurse(dictFile,"","",false);
  dictStream.close();
}

void ExportRecurse(std::ofstream dictStream, std::string partWord1, std::string partWord2, bool isSubTree)
{
  // if string is not empty, print it with node data
  if((!isSubTree && partWord1!="" )|| (isSubTree && partWord1!="" && partWord2!=""))
    {
      int word = 0;
      if isWordTerminus
	word = 1;
      dictStream << partWord1 << ";" << partWord2 ";";
      dictStream << nGraphFrequency << ";" << wordFrequency << ";" << breakFrequency << ";";
      dictStream << word << ";";
    }

  for(int i=0;i<26;i++)
    {
      if(children[i]!=NULL)
	children[i]->
    }

  
  // if this represents a word terminus, print the subtree with isSubTree active

  // if subtree, require both words to be present

  // regardless, recurse to any non-null children
}

void ImportTree(std::string dictFile)
{
  // for each line, set the data at the desired node
  
}

void ImportSetData(std::string firstWord, std::string secondWord,float nGraphFrequency,float wordFrequency, float breakFrequency, bool isWordTerminus)
{
  // if both words are null, set the data
  // if first word is not null, recurse to appropriate child node
  // if first word is null and second is not null,  recurse to subwordtree without incrementing, and with appropriate swap of words.
}
