/*****************************************************************************
*   Title      : util.cpp
*   Desc       : ASM (approximate string matching) util
*   Author     : HyunJin Kim
*   Description: These utils are used in overall area of this program 
*       
****************************************************************************/
// This header file is for unix or linux compiler 
// Therefore, Windows user should avoid the location of "unistd.h"

// Multi-Platform Design
//#ifdef WIN32
  #include "getopt.h"
//#else
//  #include <unistd.h> 
//#endif  

#include <iostream>
#include <string>
#include <string.h>
#include <boost/lexical_cast.hpp>
#include "util.hpp"

// Define error message
void error_msg(); 

void Option::set_option(const int _argc, char* const* _argv) 
{
  int       opt;           // variable for command options
  int       tmp_opt = 1;   // number of arguments for other options

  if (_argc == 1)
  { disp_help(); return; } 
  
  // ":" needs mandatory second argument, however, "::" is optional
  // "?" means an unprivided option
  while ((opt = getopt(_argc, _argv, "i:k:m:n:o:p:")) != -1) 
  {
    std::string what_mode;
    switch (opt) {
      case 'i':
        if ((optind - tmp_opt) != 2)
          error_msg();
        this->input = optarg;
        break;

      case 'k':
        if ((optind - tmp_opt) != 2)
          error_msg();
        this->k = atof(optarg);
        break;

      case 'm':
        if ((optind - tmp_opt) != 2)
          error_msg();
         what_mode = optarg;
        if (what_mode == "seq")
          this->mode = SEQ;
        else if (what_mode == "diagonal")
          this->mode = DIAGONAL;
        else if (what_mode == "pruning")
          this->mode = DIAGONAL_PRUNGING;
        else if (what_mode == "pruning_check")
          this->mode = DIAGONAL_PRUNGING_CHECK;
        break;

      case 'n':
        if ((optind - tmp_opt) != 2)
          error_msg();
        this->index = atoi(optarg);
        break;

      case 'o':
        if ((optind - tmp_opt) != 2)
          error_msg();
        this->output_filename= optarg;
        break;

      case 'p':
        if ((optind - tmp_opt) != 2)
          error_msg();
        this->pattern = optarg;
        break;

      default:
          error_msg();
    }
    tmp_opt = optind;
  } // while ((opt = getopt(_argc, _argv, "i:k:m:o:p:")) != -1) 

 
//  if (_argc == 1) // no option if argc is equal to optind 
//    disp_help();
}
// End of Option:: Set_option(int _argc, char* _argv[])  


// Body of Option:: disp_help()
void Option:: disp_help() {
  char *msg[] = {
"        usage: seq [options] ",
        "",
        "options:",
"         -i : input string ",
"               (-i input string)",
"               <input> as default input string",
"         -k : k in approximate string matching",
"               (-k float)",
"               <3> as default value",
"         -m : application mode",
"              (-m [seq, or ...])",
"               <seq> as default mode",
"         -o : output file name ",
"               (-o output filename)",
"               <output.txt> as default output text file",
"         -p : pattern string ",
"               (-p pattern string)",
"               <pattern> as default pattern string",
        NULL
	};

  for (int i=0; msg[i]; i++)
      std::cout << msg[i] << std::endl;
  exit(-1);
}
// End of Option:: disp_help() 

// Define error message
void error_msg() {
  std::cout << "error: no available argument" << std::endl;
  std::cout << "please correct your command" << std::endl; 
  exit(-1);    
}

