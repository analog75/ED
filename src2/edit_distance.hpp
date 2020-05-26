/*****************************************************************************
*   Title      : edit_distance.hpp
*   Desc       : edit distance declaration
*
*   Author     : HyunJin Kim
*   Ver        : 2018.02.28 - 0.1
*                2018.04.13 - 0.2: diagonal mode is added.
*                2018.04.14 - 0.3: zigzag mode is added.
*                2019.01.12 - 0.4: normalized edit distance mode is added.
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
                    unsigned int len1, unsigned int len2, 
                    float **d);

float edit_distance_diag(char* str1, char* str2, 
                         unsigned int len1, unsigned int len2, 
                         float **d);

float edit_distance_diag_pruning(char* str1, char* str2, 
                                 unsigned int len1, unsigned int len2, float k,
                                 float **d);

float edit_distance_diag_pruning_bit(char* str1, char* str2, 
                                     unsigned int len1, unsigned int len2, float k,
                                     float **d);
#endif
