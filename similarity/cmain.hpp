/*****************************************************************************
*   Title      : cmain.hpp
*   Desc       : ASM (approximate string matching) main header
*   Author     : HyunJin Kim
*
****************************************************************************/
#ifndef CMAIN_H // Avoid redefinition error
#define CMAIN_H 

#include "util.hpp"

void evaluate_seq(Option& _parameter);

void evaluate_seq_diag(Option& _parameter);

void evaluate_seq_diag_pruning(Option& _parameter);

#endif
