#ifndef DICTTREE_H
#define DICTTREE_H

#include <vector>

namespace dicttree {
  class Dicttree;
}

class dicttree::Dicttree
{
 public:
  
  Dicttree * children[26];
  Dicttree * subWordTree;
  float ngraphFrequency; //in future, I will use this to hold the frequency of a word in the english language
  float wordFrequency;
  float breakFrequency;
  bool isWordTerminus;

  Dicttree();
  Dicttree(std::string wordfile, int ngramDepth);
  ~Dicttree();
  std::string CleanString(std::string generalString);
  bool isUncleanAlpha(char c);
  bool contains(std::string word);
  double getFrequency(std::string ngraph);

  void addWord(std::string word, bool fromStart,bool isHead);
  void addBiWord(std::string firstWord, std::string secondWord);
  void addNGraph(std::string nGraph);

  void normalize(std::vector<int> totalnGraphs, int totalWords, int depth);

  //utility debugging print functions
  void PrintTree(std::string prepend);
  void PrintWordList(std::string prepend);
  void PrintWordListFreq(std::string prepend);
  //Import/export functions
  void ExportTree(std:string dictFile);
  void ImportTree(std:string dictFile);
};

#endif
