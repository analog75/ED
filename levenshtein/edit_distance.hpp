/*****************************************************************************
*   Title      : edit_distance.hpp
*   Desc       : edit distance declaration
*
*   Author     : HyunJin Kim
*   Ver        : 2018.02.28 - 0.1
*                2018.04.13 - 0.2: diagonal mode is added.
*                2018.04.14 - 0.3: zigzag mode is added.
*   Description: This is declaration of sequential dynamic programing 
*                for approximate string matching.
*
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

unsigned int edit_distance(char* str1, char* str2, 
                           unsigned int len1, unsigned int len2, 
                           unsigned int **d);

unsigned int edit_distance_diag(char* str1, char* str2, 
                                unsigned int len1, unsigned int len2, 
                                unsigned int **d);

unsigned int edit_distance_diag_pruning(char* str1, char* str2,
	unsigned int len1, unsigned int len2, unsigned int k,
	unsigned int **d);

unsigned int edit_distance_diag_pruning_bit(char* str1, char* str2,
	unsigned int len1, unsigned int len2, unsigned int k,
	unsigned int **d);
//unsigned int edit_distance_zigzag(char* str1, char* str2, 
//                                  unsigned int len1, unsigned int len2, 
//                                  unsigned int **d);
//
//void edit_distance_pos(char* str1, char* str2, 
//                       unsigned int len1, unsigned int len2, 
//                       unsigned int ***d);
//
//void edit_distance_pos_diag(char* str1, char* str2, 
//                            unsigned int len1, unsigned int len2, 
//                            unsigned int ***d);
//
//void edit_distance_pos_zigzag(char* str1, char* str2, 
//                              unsigned int len1, unsigned int len2, 
//                              unsigned int ***d);
