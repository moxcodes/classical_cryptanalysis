This is an ongoing project for semi-exhaustively breaking some classical
cryptographic systems. It takes advantage of a moderately large wikipedia
corpus, contained in ./corpus, to assemble a list of acceptable digrams (two
word sequences) that are sufficiently common.


In addition, this will be used to create an n-graph to perform frequency
analysis within words and of words. These techniques will be used to do improved
scoring of possible solutions.

So far, this has been done to do digram analysis of the running key cipher. It
is useful primarily for short strings with common words, and even then you get a
torrent of erroneous solutions. I hope that the more sophisticated analysis, but
this has yet to be implemented.

All done in c++, and poorly documented (sorry...


For our purposes, we need two rather similar data structures... it may be most
efficient to do both simultaneously in one big data structure. It's a little
hard to say what the 'correct' programming answer is here.

For the sake of making progress, I'm going to just implement it to have all of
the information we need in the same tree.

each node in the tree will store: a frequency number considering word breaks, a
frequency number ignoring word breaks if below ngramdepth, a boolean that is
true if the end of the first word.

... okay, if we want biword information, we're going to have a bad day - we can
have first first last last style collisions. The only way to have a correct
representation of it is to have a separate dictionary list for each word.

node data fields should be
-ngram counts with word breaks
-ngram counts without word breaks
word end flag
possible pointer to sub dict tree if word end flag is true

tree data fields also require
counts including word breaks for
  - all n-grams
  - words

counts excluding breaks for ngrams


To implement : data format - we need a list entry for every node

<firststring>;<secondstring>;frequency1;frequency2;frequency3;(0/1) for word terminus
