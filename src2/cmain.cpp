/*****************************************************************************
*   Title      : cmain.cpp
*   Desc       : ASM (approximate string matching) main routine
*   Author     : HyunJin Kim
*   Ver        : 2018.02.26 - 0.1 dynamic programming is evaluated.
*                2018.04.13 - 0.2 Diagonal mode is added
*                2018.04.14 - 0.3 Zigzag mode is added
*                2018.09.17 - 0.4 simulation mode is added
*
****************************************************************************/
#define DEBUG
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
//#include <boost/chrono.hpp>
//#include <boost/lexical_cast.hpp>
//#include <boost/random.hpp>
//#include <boost/smart_ptr.hpp> 

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
  else if (mode == DIAGONAL_PRUNGING_BIT)
    evaluate_seq_diag_pruning_bit(parameter);
  
  return 0;  

}// end of int main(int argc, char *argv[]) {

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
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

  //std::ofstream summary_file(str_summary.c_str(), std::ios::out); 
  std::ofstream summary_file(str_summary.c_str(), std::ios::app); 

  char *pattern = (char *)malloc(str_pattern.size() *sizeof(char) + 1);
  //strcpy_s(pattern, str_pattern.size() * sizeof(char) + 1, str_pattern.c_str());
  str_pattern.copy(pattern, str_pattern.size() * sizeof(char) + 1);

  char *input = (char *)malloc(str_input.size() *sizeof(char)+1);
  //strcpy_s(input, str_input.size() * sizeof(char) + 1, str_input.c_str());
  str_input.copy(input, str_input.size() * sizeof(char) + 1);

  unsigned int length_pattern = static_cast<unsigned int>(strlen(pattern));
  unsigned int length_input   = static_cast<unsigned int>(strlen(input));

  float **dist_array = (float **)malloc((length_input + 1) * sizeof(float*));
  
  for (unsigned int i=0; i<length_input + 1; i++)
    dist_array[i] = (float*)malloc((length_pattern + 1) * sizeof(float));

  auto start_time = std::chrono::high_resolution_clock::now();
  // row: pattern, column: input 

  edit_distance(input, pattern, length_input, length_pattern, dist_array);
  
  auto end_time = std::chrono::high_resolution_clock::now();
 
  //std::chrono::microseconds elapsed_time
	//  = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
  std::chrono::nanoseconds elapsed_time
	  = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
  
 
  summary_file << index << ", " 
               << input << ", "
               << pattern << ", "
               << length_input << ", "
               << length_pattern << ", "
               << dist_array[length_input][length_pattern] << ", " 
               << elapsed_time.count()
               << "\n";

#ifdef DEBUG

  summary_file << std::setw(6) << " "; 
  summary_file << std::setw(6) << " "; 
  for (int i=0; i < length_pattern; i++)
    summary_file << std::setw(6) << std::right << str_pattern[i]; 
  
  summary_file << "\n";

  for (int i=0; i < length_input+1; i++)
  {
    for (int j=0; j < length_pattern+2; j++)
    {
      if ((i==0) && (j==0))
        summary_file << std::setw(6) << " "; 
      else if (j==0)
        summary_file << std::setw(6) << str_input[i-1]; 
      else
      {
        summary_file << std::setw(5) << std::setprecision(2) 
          << std::fixed << dist_array[i][j-1] << "/"; 
      }
    }
    summary_file << "\n";
  }
          
#endif
  
  free(pattern);
  free(input);
  free(dist_array);


  
  return;
 } // end of void evaluate_seq(Option& _parameter)
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
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

  //std::ofstream summary_file(str_summary.c_str(), std::ios::out); 
  std::ofstream summary_file(str_summary.c_str(), std::ios::app); 

  char *pattern = (char *)malloc(str_pattern.size() *sizeof(char) + 1);
  //strcpy_s(pattern, str_pattern.size() * sizeof(char) + 1, str_pattern.c_str());
  str_pattern.copy(pattern, str_pattern.size() * sizeof(char) + 1);

  char *input = (char *)malloc(str_input.size() *sizeof(char)+1);
  //strcpy_s(input, str_input.size() * sizeof(char) + 1, str_input.c_str());
  str_input.copy(input, str_input.size() * sizeof(char) + 1);

  unsigned int length_pattern = static_cast<unsigned int>(strlen(pattern));
  unsigned int length_input   = static_cast<unsigned int>(strlen(input));

  float **dist_array = (float **)malloc((length_input + 1) * sizeof(float*));
  
  for (unsigned int i=0; i<length_input + 1; i++)
    dist_array[i] = (float*)malloc((length_pattern + 1) * sizeof(float));

  auto start_time = std::chrono::high_resolution_clock::now();
  // row: pattern, column: input 
  edit_distance_diag(input, pattern, length_input, length_pattern, dist_array);
  
  auto end_time = std::chrono::high_resolution_clock::now();
 
  //std::chrono::microseconds elapsed_time
	//  = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
  std::chrono::nanoseconds elapsed_time
	  = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);

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
 } // end of void evaluate_seq(Option& _parameter)
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////


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

  //std::ofstream summary_file(str_summary.c_str(), std::ios::out); 
  std::ofstream summary_file(str_summary.c_str(), std::ios::app); 

  char *pattern = (char *)malloc(str_pattern.size() *sizeof(char) + 1);
  //strcpy_s(pattern, str_pattern.size() * sizeof(char) + 1, str_pattern.c_str());
  str_pattern.copy(pattern, str_pattern.size() * sizeof(char) + 1);

  char *input = (char *)malloc(str_input.size() *sizeof(char)+1);
  //strcpy_s(input, str_input.size() * sizeof(char) + 1, str_input.c_str());
  str_input.copy(input, str_input.size() * sizeof(char) + 1);

  unsigned int length_pattern = static_cast<unsigned int>(strlen(pattern));
  unsigned int length_input   = static_cast<unsigned int>(strlen(input));

  float **dist_array = (float **)malloc((length_input + 1) * sizeof(float*));
  
  for (unsigned int i=0; i<length_input + 1; i++)
    dist_array[i] = (float*)malloc((length_pattern + 1) * sizeof(float));

  auto start_time = std::chrono::high_resolution_clock::now();
  // row: pattern, column: input 
  edit_distance_diag_pruning(input, pattern, length_input, length_pattern, k, dist_array);
  
  auto end_time = std::chrono::high_resolution_clock::now();
 
  //std::chrono::microseconds elapsed_time
	//  = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
  std::chrono::nanoseconds elapsed_time
	  = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);

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
 } // end of void evaluate_seq(Option& _parameter)
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
void evaluate_seq_diag_pruning_bit(Option& _parameter)
{
  std::cout << "*** approximate string matching using diagonal " 
            << "sequential dynamic programming and pruning bit is performed!! ***" 
            << std::endl;

  // get parameter
  // get pattern string 
  std::string str_pattern = _parameter.Get_pattern(); 
  // get input string 
  std::string str_input = _parameter.Get_input(); 

  std::string str_summary = _parameter.Get_output_filename();

  unsigned int index = _parameter.Get_index();

  unsigned int k= _parameter.Get_k();

  //std::ofstream summary_file(str_summary.c_str(), std::ios::out); 
  std::ofstream summary_file(str_summary.c_str(), std::ios::app); 

  char *pattern = (char *)malloc(str_pattern.size() *sizeof(char) + 1);
  //strcpy_s(pattern, str_pattern.size() * sizeof(char) + 1, str_pattern.c_str());
  str_pattern.copy(pattern, str_pattern.size() * sizeof(char) + 1);

  char *input = (char *)malloc(str_input.size() *sizeof(char)+1);
  //strcpy_s(input, str_input.size() * sizeof(char) + 1, str_input.c_str());
  str_input.copy(input, str_input.size() * sizeof(char) + 1);

  unsigned int length_pattern = static_cast<unsigned int>(strlen(pattern));
  unsigned int length_input   = static_cast<unsigned int>(strlen(input));

  float **dist_array = (float **)malloc((length_input + 1) * sizeof(float*));
  
  for (unsigned int i=0; i<length_input + 1; i++)
    dist_array[i] = (float*)malloc((length_pattern + 1) * sizeof(float));

  auto start_time = std::chrono::high_resolution_clock::now();
  // row: pattern, column: input 
  edit_distance_diag_pruning_bit(input, pattern, length_input, length_pattern, k, dist_array);
  
  auto end_time = std::chrono::high_resolution_clock::now();
 
  //std::chrono::microseconds elapsed_time
	//  = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
  std::chrono::nanoseconds elapsed_time
	  = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);

  summary_file << index << ", " 
               << input << ", "
               << pattern << ", "
               << length_input << ", "
               << length_pattern << ", "
               << dist_array[length_input][length_pattern]  << ", " 
               << elapsed_time.count()
               << "\n";

#ifdef DEBUG

  summary_file << std::setw(6) << " "; 
  summary_file << std::setw(6) << " "; 
  for (int i=0; i < length_pattern; i++)
    summary_file << std::setw(6) << std::right << str_pattern[i]; 
  
  summary_file << "\n";

  for (int i=0; i < length_input+1; i++)
  {
    for (int j=0; j < length_pattern+2; j++)
    {
      if ((i==0) && (j==0))
        summary_file << std::setw(6) << " "; 
      else if (j==0)
        summary_file << std::setw(6) << str_input[i-1]; 
      else
      {
        summary_file << std::setw(5) << std::setprecision(2) 
          << std::fixed << dist_array[i][j-1] << "/"; 
      }
    }
    summary_file << "\n";
  }
          
#endif



  free(pattern);
  free(input);
  free(dist_array);
  
  return;
 } // end of void evaluate_seq(Option& _parameter)
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//void evaluate_seq_pos(Option& _parameter)
//{
//  std::cout << "*** approximate string matching using sequential dynamic programming for each position is performed!! ***" 
//            << std::endl;
//
//  // get parameter
//  // get pattern string 
//  std::string pattern = _parameter.Get_pattern(); 
//  // get input string 
//  std::string input = _parameter.Get_input(); 
//
//  std::string str_summary = pattern + "_seq_pos_summary.csv";
//
//  std::ofstream summary_file(str_summary.c_str(), std::ios::out); 
//
//  const int BUFSIZE = 1000;
//  char buf[BUFSIZE];
//
//  // memory initialization with pattern file
//  std::ifstream pattern_file((pattern).c_str(), std::ios::in); 
//
//  std::string str_pattern; 
//
//  while (pattern_file.getline(buf, BUFSIZE, '\n')) 
//  {
//    str_pattern = str_pattern + buf;
//  }
//  std::cout << str_pattern.size() * sizeof(char) + 1 << std::endl;
//
// 
//  char *pattern = (char *)malloc(str_pattern.size() *sizeof(char) + 1);
//  strcpy_s(pattern, str_pattern.size() * sizeof(char) + 1, str_pattern.c_str());
//
// // memory initialization with pattern file
//  std::ifstream input_file((input).c_str(), std::ios::in); 
//
//  std::string str_input; 
//
//  while (input_file.getline(buf, BUFSIZE, '\n')) 
//  {
//    str_input = str_input + buf;
//  }
//
//  char *input = (char *)malloc(str_input.size() *sizeof(char)+1);
//  strcpy_s(input, str_input.size() * sizeof(char) + 1, str_input.c_str());
//
//  unsigned int length_pattern = static_cast<unsigned int>(strlen(pattern));
//  unsigned int length_input   = static_cast<unsigned int>(strlen(input));
//
//  unsigned int ***dist_array = (unsigned int ***)malloc((length_input) * sizeof(unsigned int**));
//
//  for (unsigned int i = 0; i < length_input; i++)
//  {
//    dist_array[i] = (unsigned int **)malloc((length_input + 1 -i) * sizeof(unsigned int*));
//    for (unsigned int j = 0; j < length_input + 1 - i; j++)
//    dist_array[i][j] = (unsigned int *)malloc((length_pattern + 1) * sizeof(unsigned int));
//  }
//
//  auto start_time = std::chrono::high_resolution_clock::now();
//  // row: pattern, column: input 
//  edit_distance_pos(input, pattern, length_input, length_pattern, dist_array);
//  
//  auto end_time = std::chrono::high_resolution_clock::now();
// 
//  //std::chrono::microseconds elapsed_time
//	//  = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
//  std::chrono::nanoseconds elapsed_time
//	  = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
//  
// 
//  summary_file << "Total Algorithm Evaluation Time, " << elapsed_time.count()
//               << ", nanosecond" << "\n";
//  
////  summary_file << "Final Edit Distance, " 
////    << dist_array[length_input][length_pattern] << "\n";
//  summary_file 
//    << "--------------------------------------------------------------" << "\n"
//    << "  Edit Distance Matrix for Each Position                      " << "\n"
//    << "  Vertical: Pattern, Horizontal: Input                        " << "\n"
//    << "  Row: Pattern, Column: Input                                 " << "\n"
//    << "  Input Size: "   << str_input.size()                           << "\n"
//    << "  Pattern Size: " << str_pattern.size()                         << "\n"
//    << "  Pattern : "     << str_pattern                                << "\n"
//    << "--------------------------------------------------------------" << "\n";
//
//  for (unsigned int p = 0; p < length_input; p++)
//  {
//    summary_file 
//      << "--------------------------------------------------------------" << "\n";
//  
//    summary_file << "Position: "<< p << ", Input: ";
//   
//    for (unsigned int t_p = p; t_p < length_input; t_p++)
//      summary_file << str_input[t_p];
//
//    summary_file << "\n";
//
//    for (unsigned int i = 1; i < length_input + 1 -p; i++)
//    {
//      for (unsigned int j = 1; j < length_pattern + 1; j++)
//      {
//        summary_file << std::setw(5) << dist_array[p][i][j] << "/ ";
//      }
//      summary_file << "\n";
//    }
//      summary_file << "\n";
//  }
//
//  free(pattern);
//  free(input);
//  free(dist_array);
//  
//  return;
// } // end of void evaluate_seq_pos(Option& _parameter)
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//void evaluate_seq_pos_diag(Option& _parameter)
//{
//  std::cout << "*** approximate string matching using diagonal "
//            << "sequential dynamic programming for each position is performed!! ***" 
//            << std::endl;
//
//  // get parameter
//  // get pattern string 
//  std::string pattern = _parameter.Get_pattern(); 
//  // get input string 
//  std::string input = _parameter.Get_input(); 
//
//  std::string str_summary = pattern + "_seq_pos_diag_summary.csv";
//
//  std::ofstream summary_file(str_summary.c_str(), std::ios::out); 
//
//  const int BUFSIZE = 1000;
//  char buf[BUFSIZE];
//
//  // memory initialization with pattern file
//  std::ifstream pattern_file((pattern).c_str(), std::ios::in); 
//
//  std::string str_pattern; 
//
//  while (pattern_file.getline(buf, BUFSIZE, '\n')) 
//  {
//    str_pattern = str_pattern + buf;
//  }
//  std::cout << str_pattern.size() * sizeof(char) + 1 << std::endl;
//
// 
//  char *pattern = (char *)malloc(str_pattern.size() *sizeof(char) + 1);
//  strcpy_s(pattern, str_pattern.size() * sizeof(char) + 1, str_pattern.c_str());
//
// // memory initialization with pattern file
//  std::ifstream input_file((input).c_str(), std::ios::in); 
//
//  std::string str_input; 
//
//  while (input_file.getline(buf, BUFSIZE, '\n')) 
//  {
//    str_input = str_input + buf;
//  }
//
//  char *input = (char *)malloc(str_input.size() *sizeof(char)+1);
//  strcpy_s(input, str_input.size() * sizeof(char) + 1, str_input.c_str());
//
//  unsigned int length_pattern = static_cast<unsigned int>(strlen(pattern));
//  unsigned int length_input   = static_cast<unsigned int>(strlen(input));
//
//  unsigned int ***dist_array = (unsigned int ***)malloc((length_input) * sizeof(unsigned int**));
//
//  for (unsigned int i = 0; i < length_input; i++)
//  {
//    dist_array[i] = (unsigned int **)malloc((length_input + 1 -i) * sizeof(unsigned int*));
//    for (unsigned int j = 0; j < length_input + 1 - i; j++)
//    dist_array[i][j] = (unsigned int *)malloc((length_pattern + 1) * sizeof(unsigned int));
//  }
//
//  auto start_time = std::chrono::high_resolution_clock::now();
//  // row: pattern, column: input 
//  edit_distance_pos_diag(input, pattern, length_input, length_pattern, dist_array);
//  
//  auto end_time = std::chrono::high_resolution_clock::now();
// 
//  //std::chrono::microseconds elapsed_time
//	//  = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
//  std::chrono::nanoseconds elapsed_time
//	  = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
//  
// 
//  summary_file << "Total Algorithm Evaluation Time, " << elapsed_time.count()
//               << ", nanosecond" << "\n";
//  
////  summary_file << "Final Edit Distance, " 
////    << dist_array[length_input][length_pattern] << "\n";
//  summary_file 
//    << "--------------------------------------------------------------" << "\n"
//    << "  Edit Distance Matrix for Sequential Diagonal in Each Position " << "\n"
//    << "  Vertical: Pattern, Horizontal: Input                        " << "\n"
//    << "  Row: Pattern, Column: Input                                 " << "\n"
//    << "  Input Size: "   << str_input.size()                           << "\n"
//    << "  Pattern Size: " << str_pattern.size()                         << "\n"
//    << "  Pattern : "     << str_pattern                                << "\n"
//    << "--------------------------------------------------------------" << "\n";
//
//  for (unsigned int p = 0; p < length_input; p++)
//  {
//    summary_file 
//      << "--------------------------------------------------------------" << "\n";
//  
//    summary_file << "Position: "<< p << ", Input: ";
//   
//    for (unsigned int t_p = p; t_p < length_input; t_p++)
//      summary_file << str_input[t_p];
//
//    summary_file << "\n";
//
//    for (unsigned int i = 1; i < length_input + 1 -p; i++)
//    {
//      for (unsigned int j = 1; j < length_pattern + 1; j++)
//      {
//        summary_file << std::setw(5) << dist_array[p][i][j] << "/ ";
//      }
//      summary_file << "\n";
//    }
//      summary_file << "\n";
//  }
//
//  free(pattern);
//  free(input);
//  free(dist_array);
//  
//  return;
// } // end of void evaluate_seq_pos_diag(Option& _parameter)
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//void evaluate_seq_pos_zigzag(Option& _parameter)
//{
//  std::cout << "*** approximate string matching using zigzag"
//            << "sequential dynamic programming for each position is performed!! ***" 
//            << std::endl;
//
//  // get parameter
//  // get pattern string 
//  std::string pattern = _parameter.Get_pattern(); 
//  // get input string 
//  std::string input = _parameter.Get_input(); 
//
//  std::string str_summary = pattern + "_seq_pos_zigzag_summary.csv";
//
//  std::ofstream summary_file(str_summary.c_str(), std::ios::out); 
//
//  const int BUFSIZE = 1000;
//  char buf[BUFSIZE];
//
//  // memory initialization with pattern file
//  std::ifstream pattern_file((pattern).c_str(), std::ios::in); 
//
//  std::string str_pattern; 
//
//  while (pattern_file.getline(buf, BUFSIZE, '\n')) 
//  {
//    str_pattern = str_pattern + buf;
//  }
//  std::cout << str_pattern.size() * sizeof(char) + 1 << std::endl;
//
// 
//  char *pattern = (char *)malloc(str_pattern.size() *sizeof(char) + 1);
//  strcpy_s(pattern, str_pattern.size() * sizeof(char) + 1, str_pattern.c_str());
//
// // memory initialization with pattern file
//  std::ifstream input_file((input).c_str(), std::ios::in); 
//
//  std::string str_input; 
//
//  while (input_file.getline(buf, BUFSIZE, '\n')) 
//  {
//    str_input = str_input + buf;
//  }
//
//  char *input = (char *)malloc(str_input.size() *sizeof(char)+1);
//  strcpy_s(input, str_input.size() * sizeof(char) + 1, str_input.c_str());
//
//  unsigned int length_pattern = static_cast<unsigned int>(strlen(pattern));
//  unsigned int length_input   = static_cast<unsigned int>(strlen(input));
//
//  unsigned int ***dist_array = (unsigned int ***)malloc((length_input) * sizeof(unsigned int**));
//
//  for (unsigned int i = 0; i < length_input; i++)
//  {
//    dist_array[i] = (unsigned int **)malloc((length_input + 1 -i) * sizeof(unsigned int*));
//    for (unsigned int j = 0; j < length_input + 1 - i; j++)
//    dist_array[i][j] = (unsigned int *)malloc((length_pattern + 1) * sizeof(unsigned int));
//  }
//
//  auto start_time = std::chrono::high_resolution_clock::now();
//  // row: pattern, column: input 
//  edit_distance_pos_zigzag(input, pattern, length_input, length_pattern, dist_array);
//  
//  auto end_time = std::chrono::high_resolution_clock::now();
// 
//  //std::chrono::microseconds elapsed_time
//	//  = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
//  std::chrono::nanoseconds elapsed_time
//	  = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
//  
// 
//  summary_file << "Total Algorithm Evaluation Time, " << elapsed_time.count()
//               << ", nanosecond" << "\n";
//  
////  summary_file << "Final Edit Distance, " 
////    << dist_array[length_input][length_pattern] << "\n";
//  summary_file 
//    << "--------------------------------------------------------------" << "\n"
//    << "  Edit Distance Matrix for Sequential Zigzag in Each Position " << "\n"
//    << "  Vertical: Pattern, Horizontal: Input                        " << "\n"
//    << "  Row: Pattern, Column: Input                                 " << "\n"
//    << "  Input Size: "   << str_input.size()                           << "\n"
//    << "  Pattern Size: " << str_pattern.size()                         << "\n"
//    << "  Pattern : "     << str_pattern                                << "\n"
//    << "--------------------------------------------------------------" << "\n";
//
//  for (unsigned int p = 0; p < length_input; p++)
//  {
//    summary_file 
//      << "--------------------------------------------------------------" << "\n";
//  
//    summary_file << "Position: "<< p << ", Input: ";
//   
//    for (unsigned int t_p = p; t_p < length_input; t_p++)
//      summary_file << str_input[t_p];
//
//    summary_file << "\n";
//
//    for (unsigned int i = 1; i < length_input + 1 -p; i++)
//    {
//      for (unsigned int j = 1; j < length_pattern + 1; j++)
//      {
//        summary_file << std::setw(5) << dist_array[p][i][j] << "/ ";
//      }
//      summary_file << "\n";
//    }
//      summary_file << "\n";
//  }
//
//  free(pattern);
//  free(input);
//  free(dist_array);
//  
//  return;
// } // end of void evaluate_seq_pos_zigzag(Option& _parameter)
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////


