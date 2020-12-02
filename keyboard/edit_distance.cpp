/*****************************************************************************
*   Title      : edit_distance.cpp
*   Desc       : edit distance implementation 
*
*   Author     : HyunJin Kim
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
#include <chrono>
#include <math.h>
#include <iostream>
#include "edit_distance.hpp"

float CONST_KEYBOARD = 0.1084;

char qwertyKeyboardArray[5][13] = {
    {'`','1','2','3','4','5','6','7','8','9','0','-','=' },
    {'q','w','e','r','t','y','u','i','o','p','[',']','\\'},
    {'a','s','d','f','g','h','j','k','l',';',124, 0,  0},
    {'z','x','c','v','b','n','m',',','.','/', 0,  0,  0},
    {0,    0,' ',' ',' ',' ',' ',  0,  0,  0, 0,  0,  0} };

char qwertyShiftedKeyboardArray[5][13] = {
    {'~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '+',  0 },
    {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '|'},
    {'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"',   0,  0},
    {'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',   0,   0,  0},
    {0,   0, ' ', ' ', ' ', ' ', ' ',   0,     0,   0,   0,   0,  0}
    };

typedef struct{
  int row;
  int column;
} Row_Column;

float AVERAGE_LENGHTH = 5.1;

float MAX_KEYBOARD = 0.108;

Row_Column getCharacterCoord(char in, char array[][13])
{
  Row_Column a;
  for (int r=0; r < 5; r++)
    for (int c=0; c < 13; c++)
      if (in == array[r][c])
      {
        a.row = r;
        a.column = c; 
      } 
      return a;
}


float euclideanKeyboardDistance(char c1, char c2)
{
  Row_Column coord1 = getCharacterCoord(c1, qwertyKeyboardArray);
  Row_Column coord2 = getCharacterCoord(c2, qwertyKeyboardArray);
  return (float)sqrt((double)((coord1.row - coord2.row) * (coord1.row - coord2.row) + 
                              (coord1.column - coord2.column) * (coord1.column - coord2.column)));
}

float edit_distance(char* str1, char* str2, 
                    unsigned int len1, unsigned int len2, float **d,
                    std::chrono::nanoseconds *elapsed_time)
{
  float cost = (float)(exp(AVERAGE_LENGHTH/len2) / exp(1));

  auto start_time = std::chrono::high_resolution_clock::now();

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
        + (str1[i-1] == str2[j-1] ? 0 : euclideanKeyboardDistance(str1[i-1], str2[j-1])*CONST_KEYBOARD);
 
      // deletion
      if(min > d[i-1][j] + cost) 
        min = d[i-1][j] + cost;
          
      // insertion
      if(min > d[i][j-1] + cost)
        min = d[i][j-1] + cost;

      d[i][j] = min;
    }
  }
  auto end_time = std::chrono::high_resolution_clock::now();

  *elapsed_time
	  = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);

  return d[len1][len2];
}


float edit_distance_diag(char* str1, char* str2, 
                         unsigned int len1, unsigned int len2, float **d,
                         std::chrono::nanoseconds *elapsed_time)
{
  float cost = (float)(exp(AVERAGE_LENGHTH/len2) / exp(1));

  auto start_time = std::chrono::high_resolution_clock::now();

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
        + (str1[i-1] == str2[j-i] ? 0 : euclideanKeyboardDistance(str1[i-1], str2[j-i])*CONST_KEYBOARD);

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
        + (str1[i+j-1] == str2[len2-i] ? 0 : euclideanKeyboardDistance(str1[i+j-1], str2[len2-i])*CONST_KEYBOARD);

     // deletion
      if(min > d[i+j-1][len2-i+1] + cost)
        min = d[i+j-1][len2-i+1] + cost;

      // insertion
      if(min > d[i+j][len2-i] + cost)
        min = d[i+j][len2-i] + cost;

      d[i+j][len2-i+1] = min;
    }
  }
  auto end_time = std::chrono::high_resolution_clock::now();

  *elapsed_time
	  = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);

  return d[len1][len2];
}


float edit_distance_diag_pruning(char* str1, char* str2, 
                                 unsigned int len1, unsigned int len2, float k, float **d,
                                 std::chrono::nanoseconds *elapsed_time)
{ 
  //str1: input, str2: pattern
  int pruning_bits[30];
  int pruning_traversal = 0; 
  
  float cost = (float)(exp(AVERAGE_LENGHTH/len2) / exp(1));

  auto start_time = std::chrono::high_resolution_clock::now();

  for(unsigned int i = 0; i < len1 + 1; i++)
    d[i][0] = cost * i;

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
          + (str1[i-1] == str2[j-i] ? 0 : euclideanKeyboardDistance(str1[i-1], str2[j-i])*CONST_KEYBOARD);

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
            + (str1[i-1] == str2[0] ? 0 : euclideanKeyboardDistance(str1[i-1], str2[0])*CONST_KEYBOARD);

          if(min > sub)
            min = sub;

          // insertion for being equal to pattern
          if(min > d[i][0] + cost)
            min = d[i][0] + cost;
          
          if ((min > k) &&  (d[i][0] > k))
          {
            pruning_traversal = j;
            pruning_bits[j-i+1]=1;
          }
          else
            d[i][j-i+1] = min;

        } // End of if (j-i+1== 1)
        else
        {
          if (pruning_traversal == j-1)       
          {
            // substitution: i-th input and j-th pattern character, string array starts from index 0 
            float sub = d[i-1][j-i] 
              + (str1[i-1] == str2[j-i] ? 0 : euclideanKeyboardDistance(str1[i-1], str2[j-i])*CONST_KEYBOARD);

            if(min > sub)
              min = sub;
          }
     
          // no need to calculate distance of insertion
          if (min > k)  
          {
            pruning_traversal = j;
            pruning_bits[j-i+1]=1;
          }
          else
            d[i][j-i+1] = min;
        }
        break;
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
          + (str1[i+j-1] == str2[len2-i] ? 0 : euclideanKeyboardDistance(str1[i+j-1], str2[len2-i])*CONST_KEYBOARD);
        
        if(min > sub)
          min = sub;

        // insertion for being equal to pattern
        if(min > d[i+j][len2-i] + cost)
          min = d[i+j][len2-i] + cost;

        d[i+j][len2-i+1] = min;
      }
      else
      {
        if (pruning_traversal == j-1 + len2)       
        {
          float sub = d[i+j-1][len2-i] 
            + (str1[i+j-1] == str2[len2-i] ? 0 : euclideanKeyboardDistance(str1[i+j-1], str2[len2-i])*CONST_KEYBOARD);
          if(min > sub)
            min = sub;
        }

        if (min > k) 
        {
          pruning_traversal = j+len2;
          pruning_bits[len2-i+1] = 1;
        }
        else 
          d[i+j][len2-i+1] = min;

        break; 
      } 
    }
  }
  auto end_time = std::chrono::high_resolution_clock::now();

  *elapsed_time
	  = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);

  return d[len1][len2];
} 

float edit_distance_diag_pruning_check(char* str1, char* str2, char**check,
                                          unsigned int len1, unsigned int len2, float k, float **d,
                                          std::chrono::nanoseconds *elapsed_time)
{ 
  //str1: input, str2: pattern
  // check: 1 --> more than k
  // check: 0 --> less than k
  int pruning_bits[30];
  int pruning_traversal = 0; 
  
  float cost = (float)(exp(AVERAGE_LENGHTH/len2) / exp(1));

  auto start_time = std::chrono::high_resolution_clock::now();

  for(unsigned int i = 0; i < len1 + 1; i++)
    d[i][0] = cost * i;

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

      float min = k+1; // reset as k
      // deletion for being equal to pattern
      if (d[i-1][j-i+1] <= k)
        min = d[i-1][j-i+1]+ cost;
        
      if (pruning_bits[j-i] == 0)        
      {
        // substitution: i-th input and j-th pattern character, string array starts from index 0 
        if (d[i-1][j-i] <= k)
        { 
          float sub = d[i-1][j-i] 
          + (str1[i-1] == str2[j-i] ? 0 : euclideanKeyboardDistance(str1[i-1], str2[j-i])*CONST_KEYBOARD);
          if(min > sub)
            min = sub;
        }

        // insertion for being equal to pattern
        if (d[i][j-i] <= k )
          if(min > d[i][j-i] + cost)
            min = d[i][j-i] + cost;

        d[i][j-i+1] = min;
      }
      else
      {
        // for the leftmost column
        if (j-i+1== 1)
        {
          if (d[i-1][0] <= k)
          {
            // substitution: i-th input and j-th pattern character, string array starts from index 0 
            float sub = d[i-1][0] 
            + (str1[i-1] == str2[0] ? 0 : euclideanKeyboardDistance(str1[i-1], str2[0])*CONST_KEYBOARD);
            if(min > sub)
              min = sub;
          }

          if (d[i][0] <= k)
          // insertion for being equal to pattern
            if(min > d[i][0] + cost)
              min = d[i][0] + cost;

          if ((min > k) &&  (d[i][0] > k))
          {
            pruning_traversal = j;
            pruning_bits[j-i+1]=1;
          }
          else
            d[i][j-i+1] = min;
        } // End of if (j-i+1== 1)
        else
        {
          if (pruning_traversal == j-1)       
          {
            // substitution: i-th input and j-th pattern character, string array starts from index 0 
            if (d[i-1][j-i] <= k)
            { 
              float sub = d[i-1][j-i] 
              + (str1[i-1] == str2[j-i] ? 0 : euclideanKeyboardDistance(str1[i-1], str2[j-i])*CONST_KEYBOARD);
              if(min > sub)
                min = sub;
            }
          }
     
          // no need to calculate distance of insertion
          if (min > k)  
          {
            pruning_traversal = j;
            pruning_bits[j-i+1]=1;
          }
          else
            d[i][j-i+1] = min;
        }
        break;
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

      float min = k+1; // reset as a large number
      // deletion for being equal to pattern
      if (d[i+j-1][len2-i+1] <= k)
        min = d[i+j-1][len2-i+1]+ cost; 

      if (pruning_bits[len2-i] == 0)      
      {
        // substitution 
        if (d[i+j-1][len2-i] <= k)
        {
          float sub = d[i+j-1][len2-i] 
          + (str1[i+j-1] == str2[len2-i] ? 0 : euclideanKeyboardDistance(str1[i+j-1],str2[len2-i])*CONST_KEYBOARD);
        
          if(min > sub)
            min = sub;
        }

        // insertion for being equal to pattern
        if (d[i+j][len2-i] <= k)
          if(min > d[i+j][len2-i] + cost)
            min = d[i+j][len2-i] + cost;

        d[i+j][len2-i+1] = min;
      }
      else
      {
        if (pruning_traversal == j-1 + len2)       
        {
          // substitution 
          if (d[i+j-1][len2-i] <= k)
          {
            float sub = d[i+j-1][len2-i] 
            + (str1[i+j-1] == str2[len2-i] ? 0 : euclideanKeyboardDistance(str1[i+j-1],str2[len2-i])*CONST_KEYBOARD);
          
            if(min > sub)
              min = sub;
          }
        }

        if (min > k) 
        {
          pruning_traversal = j+len2;
          pruning_bits[len2-i+1] = 1;
        }
        else 
          d[i+j][len2-i+1] = min;
        break;
      } 
    }
  }
  auto end_time = std::chrono::high_resolution_clock::now();

  *elapsed_time
	  = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);

  return d[len1][len2];
} 

