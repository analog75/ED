/*****************************************************************************
*   Title      : cmain.hpp
*   Desc       : ASM (approximate string matching) main header
*   Author     : HyunJin Kim
*   Ver        : 2018.02.26 - 0.1 
*                2018.04.13 - 0.2: diagonal mode is added.
*                2018.04.14 - 0.3: zigzag mode is added.
*                2019.01.04 - 0.4: diagonal pruning mode is added.
*                2019.09.02 - 0.5: diagonal pruning bit mode is added.
*
****************************************************************************/
#ifndef CMAIN_H // Avoid redefinition error
#define CMAIN_H 

#include "util.hpp"

void evaluate_seq(Option& _parameter);

void evaluate_seq_diag(Option& _parameter);

void evaluate_seq_diag_pruning(Option& _parameter);

void evaluate_seq_diag_pruning_bit(Option& _parameter);



//void evaluate_seq_zigzag(Option& _parameter);


//void evaluate_seq_pos(Option& _parameter);

//void evaluate_seq_pos_diag(Option& _parameter);

//void evaluate_seq_pos_zigzag(Option& _parameter);

#endif
