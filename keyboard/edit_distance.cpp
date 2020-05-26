/*****************************************************************************
*   Title      : edit_distance.cpp
*   Desc       : edit distance implementation 
*
*   Author     : HyunJin Kim
*   Ver        : 2018.02.28-0.1
*   Description: This is implementation of sequential dynamic programing 
*                for approximate string matching.
*
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

float DIST_NORM;

float DIST_ARRAY[26][26]; 

//////////////////////////////////////////////////////////////////////////////
unsigned int edit_distance(char* str1, char* str2, 
                           unsigned int len1, unsigned int len2, 
                           unsigned int **d)
{
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
  return d[len1][len2];
}
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
unsigned int edit_distance_diag(char* str1, char* str2, 
                                unsigned int len1, unsigned int len2, 
                                unsigned int **d)
{
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
  return d[len1][len2];
}
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
unsigned int edit_distance_diag_pruning(char* str1, char* str2, 
                                        unsigned int len1, unsigned int len2, unsigned int k,
                                        unsigned int **d)
{
  static int pruning_bits[30];

  for(unsigned int i = 0; i < len1 + 1; i++)
  {
    d[i][0] = i;
    if (i > k)
      break;
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

      if(min > d[i-1][j-i+1]+1)
        min = d[i-1][j-i+1]+1;

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
          unsigned int sub = d[i-1][0] 
            + (str1[i-1] == str2[0] ? 0 : 1);

          if(min > sub)
            min = sub;

          // insertion for being equal to pattern
          if(min > d[i][0] + 1)
            min = d[i][0] + 1;
        } // End of if (j-i+1== 1)

        if (min > k) 
        {
          pruning_bits[j-i+1] = 1;
          break;
        }
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
}
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
unsigned int edit_distance_diag_pruning_bit(char* str1, char* str2, 
                                            unsigned int len1, unsigned int len2, unsigned int k,
                                            unsigned int **d)
{
  static int pruning_bit =1;

  for(unsigned int i = 0; i < len1 + 1; i++)
  {
    d[i][0] = i;
    if (i > k)
      break;
  }

  for(unsigned int j = 0; j < len2 + 1; j++)
  {
    d[0][j] = j;
//    pruning_bits[j] = 0;
  }

//  pruning_bits[0] = 1;

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
      unsigned int min = d[i-1][j-i+1]+ 1;

      if(min > d[i-1][j-i+1]+1)
        min = d[i-1][j-i+1]+1;

      if ((pruning_bit & (1 << (j-i))) == 0)        
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
          unsigned int sub = d[i-1][0] 
            + (str1[i-1] == str2[0] ? 0 : 1);

          if(min > sub)
            min = sub;

          // insertion for being equal to pattern
          if(min > d[i][0] + 1)
            min = d[i][0] + 1;
        } // End of if (j-i+1== 1)

        if (min > k) 
        {
          pruning_bit = pruning_bit | (1 << (j-i+1));
          break;
        }
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
      if ((pruning_bit & (1 << (len2-i+1))) != 0)
        break; 

      // deletion for being equal to pattern
      unsigned int min = d[i+j-1][len2-i+1] + 1; 

      if ((pruning_bit & (1 << (len2-i))) == 0)      
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
}
//////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////
//unsigned int edit_distance_zigzag(char* str1, char* str2, 
//                                  unsigned int len1, unsigned int len2, 
//                                  unsigned int **d)
//{
//  for(unsigned int i = 0; i < len1 + 1; i++)
//    d[i][0] = i;
//
//  for(unsigned int j = 0; j < len2 + 1; j++)
//    d[0][j] = j;
//
//  bool uptodown = true;
//  // from upper left:
//  for(unsigned int j = 1; j < len2 + 1; j++)
//  {
//    unsigned int max;
//    if (j <= len1)
//      max = j + 1;
//    else
//      max = len1 + 1;
//
//    if (uptodown == true)
//    {
//      for(unsigned int i = 1; i < max; i++ )
//      {
//        unsigned int min = d[i-1][j-i] + (str1[i-1] == str2[j-i] ? 0 : 1);
//        if(min > d[i-1][j-i+1]+1)
//          min = d[i-1][j-i+1]+1;
//        if(min > d[i][j-i]+1)
//          min = d[i][j-i]+1;
//        d[i][j-i+1] = min;
//      }
//      uptodown = false;
//    }
//    else
//    {
//      for(unsigned int i = max-1; i != 0; i-- )
//      {
//        unsigned int min = d[i-1][j-i] + (str1[i-1] == str2[j-i] ? 0 : 1);
//        if(min > d[i-1][j-i+1]+1)
//          min = d[i-1][j-i+1]+1;
//        if(min > d[i][j-i]+1)
//          min = d[i][j-i]+1;
//        d[i][j-i+1] = min;
//      }
//      uptodown = true;
//    }
//  }
//
//  // from upper right
//  for(unsigned int j = 1; j < len1; j++)
//  { 
//    unsigned int diag_max;
//    if ((len1 > len2) && (j <= len1 - len2)) 
//        diag_max = len2; 
//    else
//      diag_max = len1 - j;
//
//    if (uptodown == true)
//    { 
//      for(unsigned int i = 1; i < diag_max + 1; i++ )
//      {
//        unsigned int min = d[i+j-1][len2-i] + (str1[i+j-1] == str2[len2-i] ? 0 : 1);
//        if(min > d[i+j-1][len2-i+1]+1)
//          min = d[i+j-1][len2-i+1]+1;
//        if(min > d[i+j][len2-i]+1)
//          min = d[i+j][len2-i]+1;
//        d[i+j][len2-i+1] = min;
//      }
//      uptodown = false;
//    }
//    else
//    {
//      for(unsigned int i = diag_max; i != 0; i-- )
//      {
//        unsigned int min = d[i+j-1][len2-i] + (str1[i+j-1] == str2[len2-i] ? 0 : 1);
//        if(min > d[i+j-1][len2-i+1]+1)
//          min = d[i+j-1][len2-i+1]+1;
//        if(min > d[i+j][len2-i]+1)
//          min = d[i+j][len2-i]+1;
//        d[i+j][len2-i+1] = min;
//      }
//      uptodown = true;
//    }
//  }
//  return d[len1][len2];
//}
////////////////////////////////////////////////////////////////////////////////
//

