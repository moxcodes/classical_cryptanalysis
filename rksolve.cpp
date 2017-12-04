#include <string>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "dicttree.h"
#include <fstream>
#include <stdio.h>

using namespace dicttree;

std::string CleanString(std::string generalString);
std::vector<std::string>* ImportWordList(std::string filename);
void runningKeySolve(std::string puzzle,std::string progress,std::string keyprogress, int prevBounds[2], int prevKeyBounds[2],
		     Dicttree* keySTree,Dicttree* codeSTree,Dicttree* keyBTree,Dicttree* codeBTree,int depthTrigger);

Dicttree* simpleTree;
Dicttree* bigTree;
Dicttree* doubleTree;
Dicttree* outputTree;

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




//puzzle - the cipher
//progress - the plaintext so far
//keyprogress - the key so far
//KeyStree - simple subtree for the key string
//codeStree - simple subtree for the plaintext string
//KeyBtree - big tree for the key string
//codeBtree - big tree for the plaintext string

//void runningKeySolve(std::string puzzle,std::string progress,std::string keyprogress, int prevBounds[2], int prevKeyBounds[2],
//		     Dicttree* keySTree,Dicttree* codeSTree,Dicttree* keyBTree,Dicttree* codeBTree,int depthTrigger,std::string prefix)


/*void runningKeySolve(std::string puzzle,std::string progress,std::string keyprogress, int prevBounds[2], int prevKeyBounds[2],
		     Dicttree* keySTree,Dicttree* codeSTree,int depthTrigger,std::string prefix)
{
  if(depthTrigger <= 0){
    std::cout<<progress<<'\n';
    std::cout<<keyprogress<<"\n\n";
  }
  
  if(puzzle.size()==0){
    std::string output=progress.substr(0,depthTrigger) + keyprogress.substr(0,depthTrigger);
    if(!(outputTree->contains(output)))
      {
	outputTree->add(output);
	std::cout<<progress.substr(0,depthTrigger) + "  " + keyprogress.substr(0,depthTrigger)<<'\n';
      }
    return;
  }
  if(progress.size()>depthTrigger && outputTree->contains(progress.substr(0,depthTrigger) + keyprogress.substr(0,depthTrigger)))
    return;
  //condition the trees -assume root has children, is not leaf, or permanent loop...
  //note that this implementation insists on following short word paths first, otherwise is alphabetical

  //leaf in simple tree implies leaf in full tree -
  int nuBounds[2];
  if((codeSTree!=NULL) &&(codeSTree->isLeaf))
    {
      if(prevBounds[0]==prevBounds[1]
	 || doubleTree->contains(progress.substr(prevBounds[0],progress.size()-prevBounds[0])))
	{
	  nuBounds[0]=prevBounds[1];
	  nuBounds[1]=progress.size();
	  runningKeySolve(puzzle,progress,keyprogress,nuBounds,prevKeyBounds,
			  keySTree,simpleTree,depthTrigger,prefix);
	}
    }
  if(keySTree!=NULL && keySTree->isLeaf)
    {
      if(prevKeyBounds[0]==prevKeyBounds[1]
	 || doubleTree->contains(keyprogress.substr(prevKeyBounds[0],progress.size()-prevKeyBounds[0])))
	{
	  nuBounds[0]=prevKeyBounds[1];
	  nuBounds[1]=keyprogress.size();
 	  runningKeySolve(puzzle,progress,keyprogress,prevBounds,nuBounds,
			  simpleTree,codeSTree,bigTree,codeBTree,depthTrigger,prefix);
	}
    }
  if(codeBTree->isLeaf && (codeSTree==NULL || !(codeSTree->isLeaf)))
    {
      nuBounds[0]=progress.size();
      nuBounds[1]=progress.size();
      runningKeySolve(puzzle,progress,keyprogress,nuBounds,prevKeyBounds,
		      keySTree,simpleTree,keyBTree,bigTree,depthTrigger,prefix);
    }
  if(keyBTree->isLeaf && (keySTree==NULL || !(keySTree->isLeaf)))
    {
      nuBounds[0]=progress.size();
      nuBounds[1]=progress.size();
      runningKeySolve(puzzle,progress,keyprogress,prevBounds,nuBounds,
		      simpleTree,codeSTree,bigTree,codeBTree,depthTrigger,prefix);
    }

 
  //now, we need to compute

  for(int i=0;i<26;i++){
    if((prefix=="" || (prefix.at(0) -97) ==i)
       && (codeBTree->children[i]!=NULL && (keyBTree->children[((( (int)puzzle.at(0) -97) - i) +26)%26] != NULL)))
      {
	Dicttree* nuKeySTree = NULL;
	Dicttree* nuCodeSTree = NULL;
	std::string nuPrefix="";
	if(prefix!="")
	  nuPrefix=prefix.substr(1);
	if(codeSTree!=NULL && codeSTree->children[i]!=NULL)
	  nuCodeSTree = codeSTree->children[i];
	if(keySTree!=NULL && keySTree->children[((( (int)puzzle.at(0) -97) - i) +26)%26]!=NULL)
	  nuKeySTree = keySTree->children[((( (int)puzzle.at(0) -97) - i) +26)%26];
	//this is an unattractively long function call
	runningKeySolve(puzzle.substr(1),progress + ((char)(i+97)),keyprogress + (char)(((( (int)puzzle.at(0) -97) - i) +26)%26 +97),
			prevBounds,prevKeyBounds,nuKeySTree,nuCodeSTree,
			keyBTree->children[(((int)puzzle.at(0) -97) - i + 26)%26],codeBTree->children[i],depthTrigger,nuPrefix);
      }
  }
  return;
  }*/

//Okay above is the 'complicated' version of the running key solver.
//The easiest possible that might actually generate results is to make naive probability estimates
double highscore=0;

void runningKeySolve(std::string puzzle,std::string progress,std::string keyprogress,int depthTrigger,double runningScore)
{
  //first, score each 'next step' for each of the strings
  if(highscore!=0 && runningScore < highscore)
    return;
  if(progress.size()==puzzle.size()&&(highscore==0 || runningScore >= highscore ))
    {
      std::cout<<highscore<<"\n";
      std::cout<<runningScore<<"\n\n";
      highscore=runningScore;
      std::cout<<progress<<"\n";
      std::cout<<keyprogress<<"\n\n";
      return;
    }
  if(progress.size()==puzzle.size()&&(highscore==0 || runningScore >= highscore - 100 ))
    {
      std::cout<<highscore<<"\n";
      std::cout<<runningScore<<"\n\n";
      std::cout<<progress<<"\n";
      std::cout<<keyprogress<<"\n\n";
    }
  if(progress.size()==puzzle.size())
    return;
  /*  if(depthTrigger<=0)
    {
      std::cout<<progress<<"\n";
      std::cout<<keyprogress<<"\n\n";
      }*/

  
  double plainScore[26];
  double keyScore[26];
  double fullScore[26];
  double minprob=.00000000000000000000000000000001;
  std::vector<int> maxFullScores;
  int k =0;
  for(int i=0;i<26;i++)
    {
      plainScore[i]=0;
      keyScore[i]=0;
      std::string nuProgress=progress+(char)(i+97);
      std::string nuKey = keyprogress+(char)(((puzzle.at(progress.length())-97 - i + 26)%26) + 97);
      for(int j=0;j<std::min((int)nuProgress.size(),6);j++)
	{
	  plainScore[i]+=log(std::max(simpleTree->getFrequency(nuProgress.substr(nuProgress.length()-1-j,j+1)),minprob));
	  keyScore[i]+=log(std::max(simpleTree->getFrequency(nuKey.substr(nuKey.length()-1-j,j+1)),minprob));
	}
      fullScore[i]=keyScore[i]+plainScore[i];
      k=0;
      while(k<maxFullScores.size()&&fullScore[maxFullScores[k]]>fullScore[i])
	k++;
      if(k==maxFullScores.size())
	maxFullScores.push_back(i);
      else
	maxFullScores.insert(maxFullScores.begin()+k,i);
    }

  //  for(int i=0;i<bestN;i++)
  int i=0;
  while(i<26 && fullScore[maxFullScores[i]]>= 1.5*fullScore[maxFullScores[0]])
    {
      //      if(depthTrigger<=0)
      //	std::cout<<fullScore[maxFullScores[i]]<<"\n";
	  
      runningKeySolve(puzzle,progress+(char)(maxFullScores[i] + 97),
		      keyprogress +(char)((puzzle.at(progress.length()) - 97 - maxFullScores[i] + 26)%26 + 97),
		      depthTrigger-1,runningScore+fullScore[maxFullScores[i]]);
      i++;
    }
  return;
  //now, maxFullScores has an ordered list of the best to worst next choices, so we should iterate down the tree
  
}

int main(int argc, char* argv[])
{
  //obtain wordlist
  if(argc!=7 && argc != 5){
    std::cout <<"Usage: rksolve <mystery string> <simple list filename> <advanced list filename> <double list filename>"<<'\n';
    return 0;
    }
  // std::vector<std::string>* simpleList=ImportWordList(std::string(argv[2]));
  // std::vector<std::string>* bigList=ImportWordList(std::string(argv[3]));
  // std::vector<std::string>* doubleList=ImportWordList(std::string(argv[4]));
  
  //make the english tree
  simpleTree = new Dicttree(std::string(argv[2]));
  outputTree=new Dicttree();
  //simpleTree->PrintTree("");
  //recursively probe the dictionary tree to solve the constraint - print if current attempt gets at least 5 deep or if end is reached
  // int codeBounds[2]={0,0};
  // int keyBounds[2]={0,0};
  // if(argc==6)
  if(argc==5)
    runningKeySolve(CleanString(std::string(argv[1])),"","",atoi(argv[3]),atoi(argv[4]));
  if(argc==7)
    runningKeySolve(CleanString(std::string(argv[1])),std::string(argv[5]),std::string(argv[6]),atoi(argv[3]),atoi(argv[4]));
  // if(argc==7)
  //   runningKeySolve(CleanString(std::string(argv[1])),"","",codeBounds,keyBounds,
  // 		    simpleTree,simpleTree,bigTree,bigTree,atoi(argv[5]),std::string(argv[6]));
  // //better free that wordlist
  simpleTree->~Dicttree();	
  free(simpleTree);
  return 0;
}
