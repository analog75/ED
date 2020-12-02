/*****************************************************************************
*   Title      : edit_distance.hpp
*   Desc       : edit distance declaration
*
*   Author     : HyunJin Kim
*   Description: This is declaration of sequential dynamic programing 
*                for approximate string matching.
*
****************************************************************************/

#ifndef EDIT_H  // Avoid redefinition error
#define EDIT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

extern float DIST_NORM;

extern float DIST_ARRAY[26][26]; 

float edit_distance(char* str1, char* str2, 
                    unsigned int len1, unsigned int len2, float **d,
                    std::chrono::nanoseconds *elapsed_time);

float edit_distance_diag(char* str1, char* str2, 
                         unsigned int len1, unsigned int len2, float **d,
                         std::chrono::nanoseconds *elapsed_time);

float edit_distance_diag_pruning(char* str1, char* str2, 
                                 unsigned int len1, unsigned int len2, float k, 
                                 float **d, std::chrono::nanoseconds *elapsed_time);

#endif
