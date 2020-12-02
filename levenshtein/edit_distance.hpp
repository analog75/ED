/*****************************************************************************
*   Title      : edit_distance.hpp
*   Desc       : edit distance declaration
*
*   Author     : HyunJin Kim
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
                           unsigned int **d,
                           std::chrono::nanoseconds *elapsed_time);

unsigned int edit_distance_diag(char* str1, char* str2, 
                                unsigned int len1, unsigned int len2, 
                                unsigned int **d,
                                std::chrono::nanoseconds *elapsed_time);

unsigned int edit_distance_diag_pruning(char* str1, char* str2,
	unsigned int len1, unsigned int len2, unsigned int k,
	unsigned int **d,
  std::chrono::nanoseconds *elapsed_time);
