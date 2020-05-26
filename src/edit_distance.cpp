/*****************************************************************************
*   Title      : edit_distance.cpp
*   Desc       : edit distance implementation 
*
*   Author     : HyunJin Kim
*   Ver        : 2018.02.28-0.1
*   Ver        : 2019.09.20-0.2
*   Description: This is implementation of sequential dynamic programing 
*                for approximate string matching.
*                substitution = (1/similarity) * const_sub
*                  const_sub is the average similarity between all characters. 
*                deletion, insertion: using average length, 
*                  exp(average_length/length) / exp (1)  
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include "edit_distance.hpp"

float CONST_SUB = 1.87;

// substitution: visual similarity
float SIMIL_ARRAY[26][26] = 
 // a, b,   c,   d,   e,   f,   g,   h,   i,   j,   k,   l,   m,   n,   o,   p,   q,   r,   s,   t,   u,   v,   w,   x,   y,   z    
{{0.00,2.13,2.50,2.57,3.40,1.06,3.30,1.57,1.16,1.13,1.13,1.10,1.40,1.63,3.13,2.03,2.60,1.43,2.13,1.07,2.40,1.23,1.13,1.10,1.07,1.37}, // a
 {2.13,0.00,3.03,5.60,2.27,1.83,3.53,3.70,1.43,1.43,2.13,2.77,1.23,1.77,4.20,5.07,4.67,1.27,1.40,1.63,1.93,1.40,1.10,1.07,1.20,1.13}, // b
 {2.50,3.03,0.00,3.57,4.43,1.24,2.47,1.30,1.10,1.13,1.30,1.17,1.20,2.00,5.23,2.60,2.77,1.40,2.43,1.40,2.17,1.53,1.23,1.23,1.29,1.47}, // c
 {2.57,5.60,3.57,0.00,2.87,1.43,4.10,2.73,1.73,1.27,1.33,2.30,1.13,1.93,4.90,5.10,5.10,1.33,1.27,1.37,1.83,1.55,1.07,1.23,1.13,1.23}, // d
 {3.40,2.27,4.43,2.87,0.00,1.33,2.37,1.23,1.20,1.17,1.20,1.07,1.13,1.73,4.13,2.40,2.43,1.27,2.20,1.20,1.57,1.23,1.30,1.30,1.37,1.33}, // e 
 {1.06,1.83,1.24,1.43,1.33,0.00,1.40,1.80,4.16,3.67,1.57,4.03,1.33,1.20,1.17,1.63,1.47,3.80,1.27,4.80,1.27,1.23,1.13,1.28,1.40,1.30}, // f
 {3.30,3.53,2.47,4.10,2.37,1.40,0.00,1.57,1.53,2.33,1.17,1.30,1.27,1.70,3.47,4.50,5.30,1.27,1.80,1.13,1.73,1.20,1.17,1.20,2.67,1.23}, // g 
 {1.57,3.70,1.30,2.73,1.23,1.80,1.57,0.00,1.93,1.60,2.77,2.53,2.53,5.53,1.47,2.47,2.17,2.37,1.23,1.97,3.33,1.40,1.30,1.23,1.50,1.20}, // h
 {1.16,1.43,1.10,1.73,1.20,4.16,1.53,1.93,0.00,5.17,1.90,6.13,1.63,1.50,1.33,1.60,1.55,2.70,1.07,3.90,1.67,1.67,1.30,1.40,1.50,1.13}, // i
 {1.13,1.43,1.13,1.27,1.17,3.67,2.33,1.60,5.17,0.00,1.47,4.67,1.13,1.30,1.00,1.57,1.63,2.30,1.17,3.80,1.30,1.37,1.10,1.26,2.87,1.23}, // j
 {1.13,2.13,1.30,1.33,1.20,1.57,1.17,2.77,1.90,1.47,0.00,2.80,1.20,1.53,1.03,1.53,1.47,2.00,1.40,2.13,1.26,1.97,2.07,3.55,2.03,1.50}, // k
 {1.10,2.77,1.17,2.30,1.07,4.03,1.30,2.53,6.13,4.67,2.80,0.00,1.07,1.37,1.23,1.80,1.83,3.26,1.10,4.50,1.29,1.37,1.33,1.27,1.81,1.10}, // l
 {1.40,1.23,1.20,1.13,1.13,1.33,1.27,2.53,1.63,1.13,1.20,1.07,0.00,4.67,1.30,1.33,1.17,2.23,1.20,1.07,2.27,1.33,3.40,1.10,1.20,1.10}, // m
 {1.63,1.77,2.00,1.93,1.73,1.20,1.70,5.53,1.50,1.30,1.53,1.37,4.67,0.00,2.40,1.83,1.90,3.13,1.52,1.10,4.53,1.97,1.61,1.03,1.43,1.57}, // n
 {3.13,4.20,5.23,4.90,4.13,1.17,3.47,1.47,1.33,1.00,1.03,1.23,1.30,2.40,0.00,3.60,4.10,1.13,2.27,1.13,2.83,1.27,1.13,1.07,1.17,1.27}, // o 
 {2.03,5.07,2.60,5.10,2.40,1.63,4.50,2.47,1.60,1.57,1.53,1.80,1.33,1.83,3.60,0.00,5.57,1.77,1.26,1.33,1.80,1.13,1.17,1.10,1.97,1.17}, // p 
 {2.60,4.67,2.77,5.10,2.43,1.47,5.30,2.17,1.55,1.63,1.47,1.83,1.17,1.90,4.10,5.57,0.00,1.47,1.23,1.80,2.30,1.20,1.03,1.10,2.03,1.17}, // q
 {1.43,1.27,1.40,1.33,1.27,3.80,1.27,2.37,2.70,2.30,2.00,3.26,2.23,3.13,1.13,1.77,1.47,0.00,1.60,3.37,1.97,1.67,1.03,1.43,1.27,1.30}, // r
 {2.13,1.40,2.43,1.27,2.20,1.27,1.80,1.23,1.07,1.17,1.40,1.10,1.20,1.52,2.27,1.26,1.23,1.60,0.00,1.20,1.33,1.13,1.10,1.63,1.33,2.17}, // s
 {1.07,1.63,1.40,1.37,1.20,4.80,1.13,1.97,3.90,3.80,2.13,4.50,1.07,1.10,1.13,1.33,1.80,3.37,1.20,0.00,1.60,1.40,1.00,1.47,1.50,1.27}, // t
 {2.40,1.93,2.17,1.83,1.57,1.27,1.73,3.33,1.67,1.30,1.26,1.29,2.27,4.53,2.83,1.80,2.30,1.97,1.33,1.60,0.00,4.93,2.73,1.40,3.13,1.57}, // u 
 {1.23,1.40,1.53,1.55,1.23,1.23,1.20,1.40,1.67,1.37,1.97,1.37,1.33,1.97,1.27,1.13,1.20,1.67,1.13,1.40,4.93,0.00,5.03,2.63,5.33,1.97}, // v
 {1.13,1.10,1.23,1.07,1.30,1.13,1.17,1.30,1.30,1.10,2.07,1.33,3.40,1.61,1.13,1.17,1.03,1.03,1.10,1.00,2.73,5.03,0.00,2.23,2.43,1.67}, // w 
 {1.10,1.07,1.23,1.23,1.30,1.28,1.20,1.23,1.40,1.26,3.55,1.27,1.10,1.03,1.07,1.10,1.10,1.43,1.63,1.47,1.40,2.63,2.23,0.00,3.10,1.80}, // x
 {1.07,1.20,1.29,1.13,1.37,1.40,2.67,1.50,1.50,2.87,2.03,1.81,1.20,1.43,1.17,1.97,2.03,1.27,1.33,1.50,3.13,5.33,2.43,3.10,0.00,1.93}, // y 
 {1.37,1.13,1.47,1.23,1.33,1.30,1.23,1.20,1.13,1.23,1.50,1.10,1.10,1.57,1.27,1.17,1.17,1.30,2.17,1.27,1.57,1.97,1.67,1.80,1.93,0.00}  // z 
};

float AVERAGE_LENGHTH = 5.1;

float edit_distance(char* str1, char* str2, 
                           unsigned int len1, unsigned int len2, 
                           float **d)
{
//{{{
  float cost = (float)(exp(AVERAGE_LENGHTH/len2) / exp(1));

  for(unsigned int i = 0; i < len1 + 1; i++)
    d[i][0] = cost * i;

  for(unsigned int j = 0; j < len2 + 1; j++)
    d[0][j] = cost * j;


  for(unsigned int i = 1; i < len1 + 1; i++)
  {
    for(unsigned int j = 1; j < len2 + 1; j++ )
    {
      // substition
      float min = d[i-1][j-1] 
        + (str1[i-1] == str2[j-1] ? 0 : 1/SIMIL_ARRAY[str1[i-1]-97][str2[j-1]-97]*CONST_SUB);

      // deletion
      if(min > d[i-1][j] + cost) 
        min = d[i-1][j] + cost;
          
      // insertion
      if(min > d[i][j-1] + cost)
        min = d[i][j-1] + cost;

      d[i][j] = min;
    }
  }
  return d[len1][len2];
}
//}}}


float edit_distance_diag(char* str1, char* str2, 
                                unsigned int len1, unsigned int len2, 
                                float **d)
{
//{{{
  float cost = (float)(exp(AVERAGE_LENGHTH/len2) / exp(1));

  for(unsigned int i = 0; i < len1 + 1; i++)
    d[i][0] = cost * i;

  for(unsigned int j = 0; j < len2 + 1; j++)
    d[0][j] = cost * j;

  // from upper left:
  for(unsigned int j = 1; j < len2 + 1; j++)
  {
    unsigned int max;
    if (j <= len1)
      max = j + 1;
    else
      max = len1 + 1;

    for(unsigned int i = 1; i < max; i++ )
    {
      // substition
      float min = d[i-1][j-i] 
        + (str1[i-1] == str2[j-i] ? 0 : 1/SIMIL_ARRAY[str1[i-1]-97][str2[j-i]-97]*CONST_SUB);

      // deletion
      if(min > d[i-1][j-i+1] + cost) 
        min = d[i-1][j-i+1] + cost;
          
      // insertion
      if(min > d[i][j-i] + cost)
        min = d[i][j-i] + cost;

      d[i][j-i+1] = min;
    }
  }

  // from upper right
  for(unsigned int j = 1; j < len1; j++)
  { 
    unsigned int diag_max;
    if ((len1 > len2) && (j <= len1 - len2)) 
        diag_max = len2; 
    else
      diag_max = len1 - j;
    
    for(unsigned int i = 1; i < diag_max + 1; i++ )
    {
      // substitution
      float min = d[i+j-1][len2-i] 
        + (str1[i+j-1] == str2[len2-i] ? 0 : 1/SIMIL_ARRAY[str1[i+j-1]-97][str2[len2-i]-97]*CONST_SUB);

     // deletion
      if(min > d[i+j-1][len2-i+1] + cost)
        min = d[i+j-1][len2-i+1] + cost;

      // insertion
      if(min > d[i+j][len2-i] + cost)
        min = d[i+j][len2-i] + cost;

      d[i+j][len2-i+1] = min;
    }
  }
  return d[len1][len2];
}//}}}


//////////////////////////////////////////////////////////////////////////////
float edit_distance_diag_pruning(char* str1, char* str2, 
                                 unsigned int len1, unsigned int len2, float k,
                                 float **d)
{ ///{{{
  //str1: input, str2: pattern

  int pruning_bits[30];
  register int pruning_bit = 1;  
  
  float cost = (float)(exp(AVERAGE_LENGHTH/len2) / exp(1));

  for(unsigned int i = 0; i < len1 + 1; i++)
  {
    d[i][0] = cost * i;
    if (cost * i > k)
      break;
  } 

  for(unsigned int j = 0; j < len2 + 1; j++)
  {
    d[0][j] = cost * j;
    pruning_bits[j] = 0;
  }
  pruning_bits[0] = 1;

  // from upper left:
  for(unsigned int j = 1; j < len2 + 1; j++)
  {
    unsigned int max;
    if (j <= len1)
      max = j + 1;
    else
      max = len1 + 1;

    for(unsigned int i = 1; i < max; i++ )
    {
      if (pruning_bits[j-i+1] != 0)
        break; 

      // deletion for being equal to pattern
      float min = d[i-1][j-i+1]+ cost;

      if (pruning_bits[j-i] == 0)        
      {
        // substitution: i-th input and j-th pattern character, string array starts from index 0 
        float sub = d[i-1][j-i] 
          + (str1[i-1] == str2[j-i] ? 0 : 1/SIMIL_ARRAY[str1[i-1]-97][str2[j-i]-97]*CONST_SUB);

        if(min > sub)
          min = sub;

        // insertion for being equal to pattern
        if(min > d[i][j-i] + cost)
          min = d[i][j-i] + cost;

        d[i][j-i+1] = min;
      }
      else
      {
        // for the leftmost column
        if (j-i+1== 1)
        {
          // substitution: i-th input and j-th pattern character, string array starts from index 0 
          float sub = d[i-1][0] 
            + (str1[i-1] == str2[0] ? 0 : 1/SIMIL_ARRAY[str1[i-1]-97][str2[0]-97]*CONST_SUB);

          if(min > sub)
            min = sub;

          // insertion for being equal to pattern
          if(min > d[i][0] + cost)
            min = d[i][0] + cost;
        } // End of if (j-i+1== 1)

        // no need to cacluate distance of insertion
        if (min > k)  
        {
          pruning_bits[j-i+1]=1;
          break;
        }
        else
          d[i][j-i+1] = min;
      }
    }
  }
  // End of from upper left:

  // from upper right
  for(unsigned int j = 1; j < len1; j++)
  { 
    unsigned int diag_max;
    if ((len1 > len2) && (j <= len1 - len2)) 
        diag_max = len2; 
    else
      diag_max = len1 - j;
    
    for(unsigned int i = 1; i < diag_max + 1; i++ )
    {
      if (pruning_bits[len2-i+1] != 0)
        break; 

      // deletion for being equal to pattern
      float min = d[i+j-1][len2-i+1]+ cost; 

      if (pruning_bits[len2-i] == 0)      
      {
        // substitution 
        float sub = d[i+j-1][len2-i] 
          + (str1[i+j-1] == str2[len2-i] ? 0 : 1/SIMIL_ARRAY[str1[i+j-1]-97][str2[len2-i]-97]*CONST_SUB);
        
        if(min > sub)
          min = sub;

        // insertion for being equal to pattern
        if(min > d[i+j][len2-i] + cost)
          min = d[i+j][len2-i] + cost;

        d[i+j][len2-i+1] = min;
      }
      else
      {
        if (min > k) 
        {
          pruning_bits[len2-i+1] = 1;
          break;
        }
        else 
          d[i+j][len2-i+1] = min;
      } 
    }
  }
  return d[len1][len2];
} //}}}

//////////////////////////////////////////////////////////////////////////////
float edit_distance_diag_pruning_bit(char* str1, char* str2, 
                                     unsigned int len1, unsigned int len2, float k,
                                     float **d)
{ ///{{{
  //str1: input, str2: pattern

  register int pruning_bit = 1;  
  
  float cost = (float)(exp(AVERAGE_LENGHTH/len2) / exp(1));

  for(unsigned int i = 0; i < len1 + 1; i++)
  {
    d[i][0] = cost * i;
    if (cost * i > k)
      break;
  }

  for(unsigned int j = 0; j < len2 + 1; j++)
    d[0][j] = cost * j;

  // from upper left:
  for(unsigned int j = 1; j < len2 + 1; j++)
  {
    unsigned int max;
    if (j <= len1)
      max = j + 1;
    else
      max = len1 + 1;

    for(unsigned int i = 1; i < max; i++ )
    {
      if ((pruning_bit & (1 << (j-i+1))) != 0)
        break; 

      // deletion for being equal to pattern
      float min = d[i-1][j-i+1]+ cost;

      if ((pruning_bit & (1 << (j-i))) == 0)        
      {
        // substitution: i-th input and j-th pattern character, string array starts from index 0 
        float sub = d[i-1][j-i] 
          + (str1[i-1] == str2[j-i] ? 0 : 1/SIMIL_ARRAY[str1[i-1]-97][str2[j-i]-97]*CONST_SUB);

        if(min > sub)
          min = sub;

        // insertion for being equal to pattern
        if(min > d[i][j-i] + cost)
          min = d[i][j-i] + cost;

        d[i][j-i+1] = min;
      }
      else
      {
        // for the leftmost column
        if (j-i+1== 1)
        {
          // substitution: i-th input and j-th pattern character, string array starts from index 0 
          float sub = d[i-1][0] 
            + (str1[i-1] == str2[0] ? 0 : 1/SIMIL_ARRAY[str1[i-1]-97][str2[0]-97]*CONST_SUB);

          if(min > sub)
            min = sub;

          // insertion for being equal to pattern
          if(min > d[i][0] + cost)
            min = d[i][0] + cost;
        } // End of if (j-i+1== 1)

        // no need to cacluate distance of insertion
        if (min > k)  
        {
          pruning_bit = pruning_bit | (1 << (j-i+1));
          break;
        }
        else
          d[i][j-i+1] = min;
      }
    }
  }
  // End of from upper left:

  // from upper right
  for(unsigned int j = 1; j < len1; j++)
  { 
    unsigned int diag_max;
    if ((len1 > len2) && (j <= len1 - len2)) 
        diag_max = len2; 
    else
      diag_max = len1 - j;
    
    for(unsigned int i = 1; i < diag_max + 1; i++ )
    {
      if ((pruning_bit & (1 << (len2-i+1))) != 0)
        break; 

      // deletion for being equal to pattern
      float min = d[i+j-1][len2-i+1]+ cost; 

      if ((pruning_bit & (1 << (len2-i))) == 0)      
      {
        // substitution 
        float sub = d[i+j-1][len2-i] 
          + (str1[i+j-1] == str2[len2-i] ? 0 : 1/SIMIL_ARRAY[str1[i+j-1]-97][str2[len2-i]-97]*CONST_SUB);
        
        if(min > sub)
          min = sub;

        // insertion for being equal to pattern
        if(min > d[i+j][len2-i] + cost)
          min = d[i+j][len2-i] + cost;

        d[i+j][len2-i+1] = min;
      }
      else
      {
        if (min > k) 
        {
          pruning_bit = pruning_bit | (1 << (len2-i+1));
          break;
        }
        else 
          d[i+j][len2-i+1] = min;
      } 
    }
  }
  return d[len1][len2];
} //}}}
