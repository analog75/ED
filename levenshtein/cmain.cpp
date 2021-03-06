/*****************************************************************************
*   Title      : cmain.cpp
*   Desc       : ASM (approximate string matching) main routine
*   Author     : HyunJin Kim
*
****************************************************************************/
#include <algorithm>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <boost/lexical_cast.hpp>
#include <boost/random.hpp>
#include <boost/smart_ptr.hpp> 

#include "define.h"
#include "edit_distance.hpp"
#include "cmain.hpp"

// main function
int main(int argc, char *argv[]) {

  // set screen out format
  std::cout.setf(std::ios::fixed);
  std::cout.precision(3);

  // get command line options 
  Option parameter(argc, argv);

  MODE_TYPE mode = parameter.Get_mode();

  if (mode == SEQ)
    evaluate_seq(parameter);
  else if (mode == DIAGONAL)
    evaluate_seq_diag(parameter);
  else if (mode == DIAGONAL_PRUNGING)
    evaluate_seq_diag_pruning(parameter);
  
  return 0;  

}// end of int main(int argc, char *argv[]) {

void evaluate_seq(Option& _parameter)
{
  std::cout << "*** approximate string matching using sequential dynamic programming is performed!! ***" 
            << std::endl;

  // get parameter
  // get pattern string 
  std::string str_pattern = _parameter.Get_pattern(); 
  // get input string 
  std::string str_input = _parameter.Get_input(); 

  std::string str_summary = _parameter.Get_output_filename();

  unsigned int index = _parameter.Get_index();

  std::ofstream summary_file(str_summary.c_str(), std::ios::app); 

  char *pattern = (char *)malloc(str_pattern.size() *sizeof(char) + 1);
  
  str_pattern.copy(pattern, str_pattern.size() * sizeof(char) + 1);

  char *input = (char *)malloc(str_input.size() *sizeof(char)+1);
  str_input.copy(input, str_input.size() * sizeof(char) + 1);

  unsigned int length_pattern = static_cast<unsigned int>(strlen(pattern));
  unsigned int length_input   = static_cast<unsigned int>(strlen(input));

  unsigned int **dist_array = (unsigned int **)malloc((length_input + 1) * sizeof(unsigned int*));
  
  for (unsigned int i=0; i<length_input + 1; i++)
    dist_array[i] = (unsigned int*)malloc((length_pattern + 1) * sizeof(unsigned int));

  // row: pattern, column: input 

  std::chrono::nanoseconds elapsed_time;

  edit_distance(input, pattern, length_input, length_pattern, dist_array, &elapsed_time);
  
  summary_file << index << ", " 
               << input << ", "
               << pattern << ", "
               << length_input << ", "
               << length_pattern << ", "
               << dist_array[length_input][length_pattern] << ", " 
               << elapsed_time.count()
               << "\n";
 
  free(pattern);
  free(input);
  free(dist_array);
  
  return;
 } // end of void evaluate_seq(Option& _parameter)

void evaluate_seq_diag(Option& _parameter)
{
  std::cout << "*** approximate string matching using diagonal " 
            << "sequential dynamic programming is performed!! ***" 
            << std::endl;

  // get parameter
  // get pattern string 
  std::string str_pattern = _parameter.Get_pattern(); 
  // get input string 
  std::string str_input = _parameter.Get_input(); 

  std::string str_summary = _parameter.Get_output_filename();

  unsigned int index = _parameter.Get_index();

  std::ofstream summary_file(str_summary.c_str(), std::ios::app); 

  char *pattern = (char *)malloc(str_pattern.size() *sizeof(char) + 1);
  
  str_pattern.copy(pattern, str_pattern.size() * sizeof(char) + 1);

  char *input = (char *)malloc(str_input.size() *sizeof(char)+1);
  
  str_input.copy(input, str_input.size() * sizeof(char) + 1);

  unsigned int length_pattern = static_cast<unsigned int>(strlen(pattern));
  unsigned int length_input   = static_cast<unsigned int>(strlen(input));

  unsigned int **dist_array = (unsigned int **)malloc((length_input + 1) * sizeof(unsigned int*));
  
  for (unsigned int i=0; i<length_input + 1; i++)
    dist_array[i] = (unsigned int*)malloc((length_pattern + 1) * sizeof(unsigned int));

  std::chrono::nanoseconds elapsed_time;
  // row: pattern, column: input 
  edit_distance_diag(input, pattern, length_input, length_pattern, dist_array, &elapsed_time);
  
  summary_file << index << ", " 
               << input << ", "
               << pattern << ", "
               << length_input << ", "
               << length_pattern << ", "
               << dist_array[length_input][length_pattern]  << ", " 
               << elapsed_time.count()
               << "\n";

  free(pattern);
  free(input);
  free(dist_array);
  
  return;
 } // end of void evaluate_diag(Option& _parameter)

void evaluate_seq_diag_pruning(Option& _parameter)
{
  std::cout << "*** approximate string matching using diagonal " 
            << "sequential dynamic programming and pruning array is performed!! ***" 
            << std::endl;

  // get parameter
  // get pattern string 
  std::string str_pattern = _parameter.Get_pattern(); 
  // get input string 
  std::string str_input = _parameter.Get_input(); 

  std::string str_summary = _parameter.Get_output_filename();

  unsigned int index = _parameter.Get_index();

  unsigned int k= _parameter.Get_k();

  std::ofstream summary_file(str_summary.c_str(), std::ios::app); 

  char *pattern = (char *)malloc(str_pattern.size() *sizeof(char) + 1);
  
  str_pattern.copy(pattern, str_pattern.size() * sizeof(char) + 1);

  char *input = (char *)malloc(str_input.size() *sizeof(char)+1);
  
  str_input.copy(input, str_input.size() * sizeof(char) + 1);

  unsigned int length_pattern = static_cast<unsigned int>(strlen(pattern));
  unsigned int length_input   = static_cast<unsigned int>(strlen(input));

  unsigned int **dist_array = (unsigned int **)malloc((length_input + 1) * sizeof(unsigned int*));
  
  for (unsigned int i=0; i<length_input + 1; i++)
    dist_array[i] = (unsigned int*)malloc((length_pattern + 1) * sizeof(unsigned int));

  std::chrono::nanoseconds elapsed_time;
  edit_distance_diag_pruning(input, pattern, length_input, length_pattern, k, dist_array, &elapsed_time);
  
  summary_file << index << ", " 
               << input << ", "
               << pattern << ", "
               << length_input << ", "
               << length_pattern << ", "
               << dist_array[length_input][length_pattern]  << ", " 
               << elapsed_time.count()
               << "\n";

  free(pattern);
  free(input);
  free(dist_array);
  
  return;
 } // end of void evaluate_diag_pruning(Option& _parameter)


