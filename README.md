These codes calcuated edit distance between input sequence and pattern.

There are three types of edit distances that are evaluated.

1. Levenshtein distance: 

2. Generalized edit distance based on shape similarity

3. Generalized edit distance based on keyboard character position.

The main core of this string matching is coded using C language.
However, data preprocessing and output summary are done by C++ language. 
In addition, there is need to install boost library. 

The similarity between two characters are referenced by:
Simpson IC, Mousikou P, Montoya JM, Defior S. A letter visual-similarity matrix
for Latin-based alphabets. Behavior research methods. 2013;45(2):431?439.

In keyboard character position, key positions are stored in an array, 
which using the euclidean keyboard distance  (https://github.com/wsong/Typo-Distance).
However, there is no transformation between uppercase and lowercase; you can modify the code for another application.

The English dictionary in https://github.com/dwyl/english-words/blob/master/words_alpha.txt can be used in this evaluation.

