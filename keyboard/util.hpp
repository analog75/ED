/*****************************************************************************
*   Title      : util.hpp
*   Desc       : ASM (approximate string matching) util
*   Author     : HyunJin Kim
*   Description: These utils are used in overall area of this program 
*       
****************************************************************************/

#ifndef UTIL_H  // Avoid redefinition error
#define UTIL_H

#include <iostream>
#include <string>
#include "define.h"

class Option
{

  private:
    // input string
    std::string                          input; 

    // application mode
    MODE_TYPE                            mode;  

    // index
    unsigned int                         index;

    // k mismatch 
    float                                k;

    // output filename
    std::string                          output_filename; 

    // pattern string
    std::string                          pattern; 
    
    // private member function
    void disp_help();
    void set_option (const int _argc, char* const* _argv); 

  public:
    // constructor with default option name
    Option (const int _argc, char* const* _argv) :
    input("input"), 
    mode(SEQ),
    index(0),
    k(3),
    output_filename("output.txt"), 
    pattern("pattern") 
    { 
      set_option(_argc, _argv);
    }

    std::string                          Get_input() 
    { return this->input; } 

    unsigned int                         Get_index()  
    { return this->index; } 

    MODE_TYPE                            Get_mode()  
    { return this->mode; } 

    float                                Get_k()  
    { return this->k; } 

    std::string                          Get_output_filename() 
    { return this->output_filename; } 

    std::string                          Get_pattern() 
    { return this->pattern; } 
};

#endif  // End of Avoid redefinition error
