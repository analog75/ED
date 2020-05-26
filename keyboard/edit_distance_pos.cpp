/*****************************************************************************
*   Title      : edit_distance_pos.cpp
*   Desc       : edit distance for each position implementation 
*
*   Author     : HyunJin Kim
*   Ver        : 2018.03.02-0.1
*   Description: This is implementation of sequential dynamic programing 
*                in each position for approximate string matching.
*
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>


//////////////////////////////////////////////////////////////////////////////
void edit_distance_pos(char* str1, char* str2, 
                       unsigned int len1, unsigned int len2, 
                       unsigned int ***d)
{
  // p: position, i: input, j: pattern
  for(unsigned int p = 0; p < len1; p++)
    for(unsigned int i = 0; i < len1 + 1 - p; i++)
      d[p][i][0] = i;

  for(unsigned int p = 0; p < len1; p++)
    for(unsigned int j = 0; j < len2 + 1; j++)
    d[p][0][j] = j;

  for(unsigned int p = 0; p < len1; p++)
  {
    for(unsigned int i = 1; i < len1 + 1 - p; i++)
    {
      for(unsigned int j = 1; j < len2 + 1; j++ )
      {
        unsigned int min = d[p][i-1][j-1] + (str1[i-1+p] == str2[j-1] ? 0 : 1);
        if(min > d[p][i-1][j]+1)
          min = d[p][i-1][j]+1;
        if(min > d[p][i][j-1]+1)
          min = d[p][i][j-1]+1;
        d[p][i][j] = min;
      }
    }
  }
  return;
}
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
void edit_distance_pos_diag(char* str1, char* str2, 
                            unsigned int len1, unsigned int len2, 
                            unsigned int ***d)
{
  // p: position, i: input, j: pattern
  for(unsigned int p = 0; p < len1; p++)
    for(unsigned int i = 0; i < len1 + 1 - p; i++)
      d[p][i][0] = i;

  for(unsigned int p = 0; p < len1; p++)
    for(unsigned int j = 0; j < len2 + 1; j++)
    d[p][0][j] = j;

  for(unsigned int p = 0; p < len1; p++)
  {
    // from upper left:
    for(unsigned int j = 1; j < len2 + 1; j++)
    {
      unsigned int max;
      if (j <= len1 - p)
        max = j + 1;
      else
        max = len1 - p + 1;

      for(unsigned int i = 1; i < max; i++ )
      {
        unsigned int min = d[p][i-1][j-i] + (str1[i-1+p] == str2[j-i] ? 0 : 1);
        if(min > d[p][i-1][j-i+1]+1)
          min = d[p][i-1][j-i+1]+1;
        if(min > d[p][i][j-i]+1)
          min = d[p][i][j-i]+1;
        d[p][i][j-i+1] = min;
      }
    }

    // from upper right
    for(unsigned int j = 1; j < len1 - p; j++)
    { 
      unsigned int diag_max;
      if ((len1 - p > len2) && (j <= len1 - p - len2)) 
          diag_max = len2; 
      else
        diag_max = len1 - p - j;
      
      for(unsigned int i = 1; i < diag_max + 1; i++ )
      {
        unsigned int min = d[p][i+j-1][len2-i] + (str1[i+j-1+p] == str2[len2-i] ? 0 : 1);
        if(min > d[p][i+j-1][len2-i+1]+1)
          min = d[p][i+j-1][len2-i+1]+1;
        if(min > d[p][i+j][len2-i]+1)
          min = d[p][i+j][len2-i]+1;
        d[p][i+j][len2-i+1] = min;
      }
    }
  }
  return;
}
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
void edit_distance_pos_zigzag(char* str1, char* str2, 
                              unsigned int len1, unsigned int len2, 
                              unsigned int ***d)
{
  // p: position, i: input, j: pattern
  for(unsigned int p = 0; p < len1; p++)
    for(unsigned int i = 0; i < len1 + 1 - p; i++)
      d[p][i][0] = i;

  for(unsigned int p = 0; p < len1; p++)
    for(unsigned int j = 0; j < len2 + 1; j++)
    d[p][0][j] = j;

  for(unsigned int p = 0; p < len1; p++)
  {
    bool uptodown = true;
    // from upper left:
    for(unsigned int j = 1; j < len2 + 1; j++)
    {
      unsigned int max;
      if (j <= len1 - p)
        max = j + 1;
      else
        max = len1 - p + 1;

      if (uptodown == true)
      {
        for(unsigned int i = 1; i < max; i++ )
        {
          unsigned int min = d[p][i-1][j-i] + (str1[i-1+p] == str2[j-i] ? 0 : 1);
          if(min > d[p][i-1][j-i+1]+1)
            min = d[p][i-1][j-i+1]+1;
          if(min > d[p][i][j-i]+1)
            min = d[p][i][j-i]+1;
          d[p][i][j-i+1] = min;
        }
        uptodown = false;
      }
      else
      {
        for(unsigned int i = max -1 ; i != 0; i-- )
        {
          unsigned int min = d[p][i-1][j-i] + (str1[i-1+p] == str2[j-i] ? 0 : 1);
          if(min > d[p][i-1][j-i+1]+1)
            min = d[p][i-1][j-i+1]+1;
          if(min > d[p][i][j-i]+1)
            min = d[p][i][j-i]+1;
          d[p][i][j-i+1] = min;
        }
        uptodown = true;
      }
    }

    // from upper right
    for(unsigned int j = 1; j < len1 - p; j++)
    { 
      unsigned int diag_max;
      if ((len1 - p > len2) && (j <= len1 - p - len2)) 
          diag_max = len2; 
      else
        diag_max = len1 - p - j;
      
      if (uptodown == true)
      {
        for(unsigned int i = 1; i < diag_max + 1; i++ )
        {
          unsigned int min = d[p][i+j-1][len2-i] + (str1[i+j-1+p] == str2[len2-i] ? 0 : 1);
          if(min > d[p][i+j-1][len2-i+1]+1)
            min = d[p][i+j-1][len2-i+1]+1;
          if(min > d[p][i+j][len2-i]+1)
            min = d[p][i+j][len2-i]+1;
          d[p][i+j][len2-i+1] = min;
        }
        uptodown = false;
      }
      else
      {
        for(unsigned int i = diag_max; i != 0; i-- )
        {
          unsigned int min = d[p][i+j-1][len2-i] + (str1[i+j-1+p] == str2[len2-i] ? 0 : 1);
          if(min > d[p][i+j-1][len2-i+1]+1)
            min = d[p][i+j-1][len2-i+1]+1;
          if(min > d[p][i+j][len2-i]+1)
            min = d[p][i+j][len2-i]+1;
          d[p][i+j][len2-i+1] = min;
        }
        uptodown = true;
      }
 
    }
  }
  return;
}
//////////////////////////////////////////////////////////////////////////////

