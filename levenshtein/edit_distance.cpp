/*****************************************************************************
*   Title      : edit_distance.cpp
*   Desc       : edit distance implementation 
*
*   Author     : HyunJin Kim
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <iostream>

float DIST_NORM;

float DIST_ARRAY[26][26]; 

unsigned int edit_distance(char* str1, char* str2, 
                           unsigned int len1, unsigned int len2, 
                           unsigned int **d, 
                           std::chrono::nanoseconds *elapsed_time)
{
  auto start_time = std::chrono::high_resolution_clock::now();

  for(unsigned int i = 0; i < len1 + 1; i++)
    d[i][0] = i;

  for(unsigned int j = 0; j < len2 + 1; j++)
    d[0][j] = j;

  for(unsigned int i = 1; i < len1 + 1; i++)
  {
    for(unsigned int j = 1; j < len2 + 1; j++ )
    {
      unsigned int min = d[i-1][j-1] + (str1[i-1] == str2[j-1] ? 0 : 1);
      if(min > d[i-1][j]+1)
        min = d[i-1][j]+1;
      if(min > d[i][j-1]+1)
        min = d[i][j-1]+1;
      d[i][j] = min;
    }
  }
  auto end_time = std::chrono::high_resolution_clock::now();

  *elapsed_time
	  = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
 
  return d[len1][len2];
}
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
unsigned int edit_distance_diag(char* str1, char* str2, 
                                unsigned int len1, unsigned int len2, 
                                unsigned int **d, 
                                std::chrono::nanoseconds *elapsed_time)
{
  auto start_time = std::chrono::high_resolution_clock::now();

  for(unsigned int i = 0; i < len1 + 1; i++)
    d[i][0] = i;

  for(unsigned int j = 0; j < len2 + 1; j++)
    d[0][j] = j;

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
      unsigned int min = d[i-1][j-i] + (str1[i-1] == str2[j-i] ? 0 : 1);
      if(min > d[i-1][j-i+1]+1)
        min = d[i-1][j-i+1]+1;
      if(min > d[i][j-i]+1)
        min = d[i][j-i]+1;
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
      unsigned int min = d[i+j-1][len2-i] + (str1[i+j-1] == str2[len2-i] ? 0 : 1);
      if(min > d[i+j-1][len2-i+1]+1)
        min = d[i+j-1][len2-i+1]+1;
      if(min > d[i+j][len2-i]+1)
        min = d[i+j][len2-i]+1;
      d[i+j][len2-i+1] = min;
    }
  }
  auto end_time = std::chrono::high_resolution_clock::now();

  *elapsed_time
	  = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);

  return d[len1][len2];
}
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
unsigned int edit_distance_diag_pruning(char* str1, char* str2, 
                                        unsigned int len1, unsigned int len2, unsigned int k, unsigned int **d,
                                        std::chrono::nanoseconds *elapsed_time)
{
  //str1: input, str2: pattern
  static int pruning_bits[30];
  int pruning_traversal = 0; 

  auto start_time = std::chrono::high_resolution_clock::now();

  for(unsigned int i = 0; i < len1 + 1; i++)
  {
    d[i][0] = i;
  }

  for(unsigned int j = 0; j < len2 + 1; j++)
  {
    d[0][j] = j;
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
      if (pruning_bits[j-i+1] == 1)
        break; 
     
      // deletion for being equal to pattern
      unsigned int min = d[i-1][j-i+1]+ 1;

      if (pruning_bits[j-i] == 0)        
      {
        unsigned int sub = d[i-1][j-i] + (str1[i-1] == str2[j-i] ? 0 : 1);
       
        if(min > sub)
          min = sub;

        // insertion for being equal to pattern
        if(min > d[i][j-i] + 1)
          min = d[i][j-i] + 1;

        d[i][j-i+1] = min;
      }
      else
      {
        // for the leftmost column
        if (j-i+1== 1)
        {
          // substitution: i-th input and j-th pattern character, string array starts from index 0 
          unsigned int sub = d[i-1][0] + (str1[i-1] == str2[0] ? 0 : 1);

          if(min > sub)
            min = sub;

          // insertion for being equal to pattern
          if(min > d[i][0] + 1)
            min = d[i][0] + 1;

          if ((min > k) && (d[i][0] > k))
          {
            pruning_traversal = j;
            pruning_bits[j-i+1] = 1;
          }
          else
            d[i][j-i+1] = min; 
          
          break;
        } // End of if (j-i+1== 1)
        else
        {
          if (pruning_traversal == j-1)       
          {
            // substitution: i-th input and j-th pattern character, string array starts from index 0 
            unsigned int sub = d[i-1][j-i] + (str1[i-1] == str2[j-i] ? 0 : 1);

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
      if (pruning_bits[len2-i+1] == 1)
        break; 

      // deletion for being equal to pattern
      unsigned int min = d[i+j-1][len2-i+1] + 1; 

      if (pruning_bits[len2-i] == 0)      
      {
        // substitution 
        unsigned int sub = d[i+j-1][len2-i] + (str1[i+j-1] == str2[len2-i] ? 0 : 1);
       
        if(min > sub)
          min = sub;

        // insertion for being equal to pattern
        if(min > d[i+j][len2-i] + 1)
          min = d[i+j][len2-i] + 1;

        d[i+j][len2-i+1] = min;
      }
      else
      {
        if (pruning_traversal == j-1 + len2)       
        {
          // substitution 
          unsigned int sub = d[i+j-1][len2-i] + (str1[i+j-1] == str2[len2-i] ? 0 : 1);

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



